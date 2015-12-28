// COMPILER.CPP : Main Interface to compiler.
// Liscenced under MIT - (c) twitchyliquid64 and FALCON dev team.

#include "stdafx.h"
#include "tokens.h"
#include "lexer/lexer.h"
#include "scriptgraph/nodes.h"
#include "compiler.h"
#include "parser/parser.h"
#include "generators/provider.h"

/*
 * Add_Sourcefile
 * 
 * Called when a new sourcefile needs to be added to the list of compiled objects
 * Param 1: [in] Pass the filename.
 * Returns 0 on success.
*/
int Compiler::Add_Sourcefile(char* filename){
	
    this->has_compiled = false;

    if(this->isVerboseMode)
        std::cout << "Lexing: " << filename;
    this->sourcefiles.push_back(filename);
  
    Lexer lexer((char*)"");
    lexer.Lex_File(filename);
    std::list<token>* filetoks = lexer.Export_Tokens();
    //lexer.Print_Tokens(); //Debug

	//Now we walk through the list of tokens from this sourcefile,
	//and put them on our combined VECTOR of tokens from all sourcefiles.
    std::list<token>::iterator i;
    int count = 0;
    this->tokenvect.reserve(lexer.Tok_Count());
    for( i = filetoks->begin(); i != filetoks->end(); ++i)
    {
        count++;
        token* tok = new token;
        tok->type = i->type;
        tok->data = i->data;
        this->tokenvect.push_back(tok);
        //std::cout << this->tokenvect[count-1]->data << " ";
    }
 
 
     if(this->isVerboseMode)
        std::cout << " -> DONE. Generated " << lexer.Tok_Count() << " tokens.\n";
    return 0;
}


/*
 * Set_Verbose
 * 
 * Turns on verbose explanations of what is currently processing. Outputs to STDOUT.
 * Params: none. Returns none.
*/
void Compiler::Set_Verbose(void){
    
    this->isVerboseMode = true;
    
}
/*
 * UnSet_Verbose
 * 
 * Turns off verbose explanations of what is currently processing.
 * Params: none. Returns none.
*/
void Compiler::UnSet_Verbose(void){
    
    this->isVerboseMode = false;
    
}

/*
 * Set_CWD
 * 
 * Used to indicate the working directory of compilation. This is needed so that the
 * compiled result can be put in the same directory as the script, and also needed so that
 * #include directives can be resolved.
 * Param 1: [in] File path of working directory.
 * Returns 0 on success.
*/
int Compiler::Set_CWD(char* filename){
  strcat( filename, "\\");
  this->cwd = filename;
  return 0;
}












/* 
 * Compile
 * 
 * Called to compile all given sourcefiles and produce a compiled in-memory (AST)
 * representation.
 * All sourcefiles should be added by calling Add_Sourcefile() by this stage.
 * Params: none.
 * Returns 0 on success
*/
int Compiler::Compile(void){

  if(this->has_compiled)
      return 1;

  this->has_compiled = true;


  //Disambiguates the whole optional '$' prefix for variables
  Tokens_Disambiguate_Pass(&this->tokenvect);

  //Parse User functions
  int poscount = 0;
  for(;poscount<this->tokenvect.size();poscount++)
  {
	  if(this->tokenvect[poscount]->type == FUNC_DEC_TOK)
	  {//We found a UDF, lets index its attributes and parse it.
        Functionentry* temp = new Functionentry;
		temp->name = this->tokenvect[poscount]->data;
        temp->num_params = 0;
        temp->num_opt_params = 0;
        int ParameterHasByRef = 0; //set if the parameter currently being read has a ByRef attribute.
        int ParameterHasOption = 0;
        std::string Read_ParamName = "";		//set if the parameter name already read in.
        this->UDFnodes.push_back(temp);
        
        poscount+=2;//get past name and opening '(' tokens
        while(this->tokenvect[poscount]->type != EOL_TOK)
		{
			if(this->tokenvect[poscount]->type == PARAM_SEPARATOR_TOK){ //End of parameter?
			  temp->parameters_name.push_back(Read_ParamName);//push last params' details to structure
			  temp->parameters_byref.push_back((bool)ParameterHasByRef);
              temp->num_params++;
              ParameterHasByRef = 0;//Reset vars ready for next iteration
              Read_ParamName = "";
              ParameterHasOption = 0;
              poscount++;
            }
            
            if((this->tokenvect[poscount]->type == EQUAL_TOK) && (!ParameterHasOption))//must be an optional argument
            {
				poscount++;
				temp->num_opt_params++;
				
				//now we collect the expression(which is the optional value) and parse it
				std::vector<token*>* expression = new std::vector<token*>;
				expression->reserve(150);
				if(!Parser_CollectExpression(&this->tokenvect,&poscount,expression))
					 return 0; //error
				std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
				temp->parameters_optional_values.push_back( AssembleASTBranch(&shunted));
				
				poscount--;
			}
			else if(this->tokenvect[poscount]->type == GENERAL_STATEMENT_TOK)//argument has BYREF attribute
				ParameterHasByRef = 1;
			else if(Read_ParamName == "")//Argument name has not been read in, so read it in
				Read_ParamName = this->tokenvect[poscount]->data;
				
			poscount++;
		}
		
		if((Read_ParamName != "") && (Read_ParamName != ")"))//there is one more param to push
		{
			temp->parameters_name.push_back(Read_ParamName);//push last params details to structure
			temp->parameters_byref.push_back((bool)ParameterHasByRef);
            temp->num_params++;
		}
        
        //Compile the function to an AST
        if(this->isVerboseMode)
			std::cout << "Compiling function :: " << temp->name << "\n";
		std::vector<ExprAST*>* CodeCollection = new std::vector<ExprAST*>;
		CollectionExprAST* node = new CollectionExprAST(CodeCollection);
		std::vector<ExprAST*>* codebranch = (std::vector<ExprAST*>*)node->GetCollection();
		int val = ParseCode( &this->tokenvect, &poscount, codebranch, temp->name, USERFUNC_CONTEXT);
		temp->code = node;
		temp->SymbolTable = NULL;

		if(!val)
		{
			std::cout << "-::::::: Error Condition raised in parser (Userfunc) :::::::-\n";
			return 0;
		}
	  }
  };

  int pos = 0;
  CollectionExprAST* node;

    //Build root level for the AST. (these things are on heap to prevent stack overflow)
    //Basically, we are now compiling code not in functions.
    std::vector<ExprAST*>* CodeCollection = new std::vector<ExprAST*>;
    node = new CollectionExprAST(CodeCollection); //Our collection of nodes.
    std::vector<ExprAST*>* codebranch = (std::vector<ExprAST*>*)node->GetCollection();

  //Build the AST.
  int val = ParseCode( &this->tokenvect, &pos, codebranch, (char*)"", '\0');
  this->code = node;
  
  //Build the symbol table.
  this->SymbolTable.primarydomain = "";//building in global scope
  if(val)
    this->code->SymboltableBuild(0, &this->SymbolTable);
  else{
	  std::cout << "-::::::: Error Condition raised in parser :::::::-\n";
	  return 0;
  }

  //All the code has been compiled, and the global symbol table built. Now the
  //Local symboltables must be built, and the variable scope resolved.
  int count=0;
  for(;count<this->UDFnodes.size();count++)
  {
      if(this->isVerboseMode)
		  std::cout << "Building static symbols for function: " << this->UDFnodes[count]->name << "\n";
	  Symboltable_ClearLocalEntries(&this->SymbolTable);//Clear local variables from the previous iteration. This
	  this->SymbolTable.primarydomain = this->UDFnodes[count]->name;//Prepare to generate a symboltable in the user functions' domain.

	  int z = 0;
	  for(;z < this->UDFnodes[count]->parameters_name.size(); z++)
	  {		//Add function params to symboltable
            SymtableEntry* entry = new SymtableEntry;
            entry->scope = ASSIGN_SCOPE_LOCAL;
            entry->varname = this->UDFnodes[count]->parameters_name[z];
            entry->domain = this->UDFnodes[count]->name;
            entry->discovered_depth = 0;
            this->SymbolTable.entries.push_back(entry);
	  }
	  
	  this->UDFnodes[count]->code->SymboltableBuild(0, &this->SymbolTable);//Build symboltable for function
	  this->UDFnodes[count]->SymbolTable = Symboltable_Duplicate(&this->SymbolTable);//Save the local symboltable
	  //this->UDFnodes[count]->code->DebugPrint(0);//debug
  }
  //revert global symboltable now generating the local symboltables are done.
  Symboltable_ClearLocalEntries(&this->SymbolTable);
  this->SymbolTable.primarydomain = "";
  
  //Now all scope is resolved, and all code is parsed into the AST. Now the codegen functions can be called.
  return val;
}











/*
 * PrintAST
 * 
 * Debug Function which recursively prints the compiled representation (AST) to
 * STDOUT.
 * Params: none. Returns none.
*/
void Compiler::PrintAST(void){
    if(this->isVerboseMode)
        std::cout << "Abstract Syntax Tree :: (Instantaneous representation)\n";
    this->code->DebugPrint(0);
    
}

/*
 * Warning
 * 
 * Called by components to issue warnings.
 * NOTE: most things print their own warnings by talking straight to STDOUT.
 * Param 1: [in] The name of the component
 * Param 2: [in] The warning message
 * Returns 0 on success.
*/
int Compiler::Warning(char* component, char* message){
    
    std::cout << "Warning " << component << ": " << message << '\n';
    return 0;
    
}

/*
 * Error
 * 
 * Called by components to report a fatal erroneous status.
 * Param 1: [in] Name of component
 * Param 2: [in] The error message
 * Returns 0 on success.
*/
int Compiler::Error(char* component, char* message){
    
    std::cout << "Error! " << component << ": " << message << '\n';
    return 0;

}

/*
 * GenerateCode_ToSTDOUT
 * 
 * DEPRECEATED!!!! This call will not generate correct code for user functions.
 * Use GenerateCode_ToFile() instead.
*/
void Compiler::GenerateCode_ToSTDOUT(void){
    this->Compile();
    int* temp = new int;
    std::cout << emit_core_code();
    std::cout << emit_global_definitions(&this->SymbolTable) << "\n\n" << emit_entry_point();;
    std::cout << this->code->CodeGen( temp, AST_TYPE_UNDEF, &this->SymbolTable, 0);
    std::cout << emit_entry_point_closeure();
}

/*
 * GenerateCode_ToFile
 * 
 * Generates C++ code that represents the given script(s), and dumps the code
 * to file.
 * Param 1: [in] Filename or RELATIVE (to CWD) path of file to dump to.
 * Returns none.
*/
void Compiler::GenerateCode_ToFile(char* filename){
    this->Compile();
    //Previous bug did not call the compile method.
    char* directory = filename;

    if(this->isVerboseMode)
        std::cout << "Source Output: " << directory << '\n';
    
    std::fstream file( directory, std::ios::out);
    
    int temp;
    Symtable symtab;
    file << emit_core_code();
    file << emit_global_definitions(&this->SymbolTable);
    file << "\n\n";
    
    //emit user function prototypes
    file << "//User function prototypes\n";
	int count=0;
	for(;count<this->UDFnodes.size();count++)
	{
		file << "Variant f" << this->UDFnodes[count]->name << "(";
		int z = 0;
		for(;z < this->UDFnodes[count]->parameters_name.size(); z++)
		{
			file << "Variant*";
			if((z+1) < this->UDFnodes[count]->parameters_name.size())
				file << ",";
		}
		file << ");\n\n";
	}
  
    //emit user functions themselves
    file << "//User functions\n";
	count=0;
	for(;count<this->UDFnodes.size();count++)
	{
		int temp;
		Symtable symtab;
		file << "Variant f" << this->UDFnodes[count]->name << "(";
		int z = 0;
		for(;z < this->UDFnodes[count]->parameters_name.size(); z++)
		{
			file << "Variant* LRv" << this->UDFnodes[count]->parameters_name[z] << " = NULL";
			if((z+1) < this->UDFnodes[count]->parameters_name.size())
				file << ",";
		}
		file << ")\n{";
		
		z = 0;
		for(;z < this->UDFnodes[count]->SymbolTable->entries.size(); z++)
		{//now we declare all the vars that are used in this function locally
			if(this->UDFnodes[count]->SymbolTable->entries[z]->domain == this->UDFnodes[count]->name)
				file << "\n    Variant Lv" << this->UDFnodes[count]->SymbolTable->entries[z]->varname << ";";
		}
		
		//Prepare arguments for accessing, assigning the default argument values if nesessary.
		z = 0;
		for(;z < this->UDFnodes[count]->parameters_name.size(); z++)//Deal with possible optional values.
			if(z >= (this->UDFnodes[count]->num_params - this->UDFnodes[count]->num_opt_params))
			{
				file << "\n    //Determining real value for variable $" << this->UDFnodes[count]->parameters_name[z] << ";";
				file << "\n    if(LRv" << this->UDFnodes[count]->parameters_name[z] << " == NULL)";
				file << "\n        Lv" << this->UDFnodes[count]->parameters_name[z] << " = ";
				file << this->UDFnodes[count]->parameters_optional_values[z - (this->UDFnodes[count]->num_params - this->UDFnodes[count]->num_opt_params)]->CodeGen( &temp, AST_TYPE_UNDEF, &symtab, 0) << ";";
				file << "\n    else";
				file << "\n        Lv" << this->UDFnodes[count]->parameters_name[z] << " = (*LRv" << this->UDFnodes[count]->parameters_name[z] << ");";
			}
			else
				file << "\n    Lv" << this->UDFnodes[count]->parameters_name[z] << " = (*LRv" << this->UDFnodes[count]->parameters_name[z] << ");";

		file << "\n\n" << this->UDFnodes[count]->code->CodeGen( &temp, AST_TYPE_UNDEF, &symtab, 0);
		file << "\n    return vr(0);\n};\n\n";
	}
    
    //generate the 'global' code as part of the entrypoint.
    file << "\n\n" << emit_entry_point();
    file << this->code->CodeGen( &temp, AST_TYPE_UNDEF, &symtab, 0);
    file << emit_entry_point_closeure();
    file.close();
}


/*
 * DumpTokens_ToFile
 * 
 * Dumps a sequential representation of the first given sourcefile.
 * Param 1: [in] filename (NOT PATH) of file to dump to
 * Returns none.
*/
void Compiler::DumpTokens_ToFile(char* filename)
{
    
    if(this->isVerboseMode)
        std::cerr << "Token Dump: " << filename << '\n';
        
    Lexer lexer((char*)this->sourcefiles[0].c_str());
    lexer.Lex_File((char*)this->sourcefiles[0].c_str());
    lexer.Dump_Tokens(filename);
    
}

/*
 * Dump_SymTable_ToFile
 * 
 * Dumps the Global Symboltable to file. This does not include local
 * symbols
 * Param 1: [in] Filename (NOT PATH) to dump table to.
 * Returns none.
*/
void Compiler::DumpSymTable_ToFile(char* filename)
{
    
    if(this->isVerboseMode)
        std::cerr << "SymbolTable Dump: " << filename << '\n';
        

    int i=0;
    
    std::fstream file( filename, std::ios::out);

    file << "SYMBOL TABLE :=====================~\n\n";
    for(;i < this->SymbolTable.entries.size();i++)
    {
        std::string domain = this->SymbolTable.entries[i]->domain;
        if(domain=="")domain = "GLOBAL";
        file << "Var: " << this->SymbolTable.entries[i]->varname << " in domain: " << domain << '\n';
    }
    file.close();
}
