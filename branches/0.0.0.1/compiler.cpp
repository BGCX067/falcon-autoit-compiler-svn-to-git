// COMPILER.CPP : Main Interface to compiler.
// Liscenced under MIT.

#include "stdafx.h"
#include "tokens.h"
#include "lexer/lexer.h"
#include "scriptgraph/nodes.h"
#include "compiler.h"
#include "parser/parser.h"
#include "generators/provider.h"

int Compiler::Add_Sourcefile(char* filename){
	
	if(this->isVerboseMode)
		std::cout << "Lexing: " << filename;
	this->sourcefiles.push_back(filename);
  
	Lexer lexer("");
	lexer.Lex_File(filename);
	std::list<token>* filetoks = lexer.Export_Tokens();
	//lexer.Print_Tokens(); //Debug
  
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



void Compiler::Set_Verbose(void){
	
	this->isVerboseMode = true;
	
}
void Compiler::UnSet_Verbose(void){
	
	this->isVerboseMode = false;
	
}


int Compiler::Set_CWD(char* filename){
  strcat( filename, "\\");
  this->cwd = filename;
  return 0;
}

/* Entry point for code compilation. Tokens are assumed to be
inserted into the instance by now */
int Compiler::Compile(void){

  int pos = 0;
  CollectionExprAST* node;

  int val = ParseCode( &this->tokenvect, &pos, &node, "", '\0');
  this->code = node;
  return val;
}

void Compiler::PrintAST(void){
	if(this->isVerboseMode)
		std::cout << "Abstract Syntax Tree :: (Instantaneous representation)\n";
	this->code->DebugPrint(0);
	
}


int Compiler::Warning(char* component, char* message){
	
	std::cout << "Warning " << component << ": " << message << '\n';
	return 0;
	
}

int Compiler::Error(char* component, char* message){
	
	std::cout << "Error! " << component << ": " << message << '\n';
	return 0;

}


void Compiler::GenerateCode(void){
	this->Compile();
	std::cout << emit_core_code();
	int* temp = new int;
	Symtable* symtab = new Symtable;
	std::cout << this->code->CodeGen( temp, AST_TYPE_UNDEF, symtab, 0);
}

void Compiler::GenerateCode_ToFile(char* filename){
	this->Compile();
	//Previous bug did not call the compile method.
	char directory[MAX_PATH];	//Actually represents the full path.
	strcpy( directory, this->cwd.c_str());
	strcat( directory, filename);

	if(this->isVerboseMode)
		std::cout << "Source Output: " << directory << '\n';
	
	std::fstream file( directory, std::ios::out);
	
	int temp;
	Symtable symtab;
	file << emit_core_code();
	file << "\n\n" << emit_entry_point();
	file << this->code->CodeGen( &temp, AST_TYPE_UNDEF, &symtab, 0);
	file << emit_entry_point_closeure();
	file.close();
}



void Compiler::DumpTokens_ToFile(char* filename)
{
	char directory[MAX_PATH];	//Actually represents the full path.
	strcpy( directory, this->cwd.c_str());
	strcat( directory, filename);
	
	if(this->isVerboseMode)
		std::cerr << "Token Dump: " << directory << '\n';
		
	Lexer lexer((char*)this->sourcefiles[0].c_str());
	lexer.Lex_File((char*)this->sourcefiles[0].c_str());
	lexer.Dump_Tokens(directory);
	
}
