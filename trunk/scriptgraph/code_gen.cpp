#include "../stdafx.h"
#include "../tokens.h"
#include "nodes.h"
#include "../generators/provider.h"

std::string NumberExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    char* buffer = new char[32];
#ifdef _MSC_VER
    sprintf(buffer, "%g", this->Val);
#else
    snprintf(buffer, 32, "%g", this->Val);
#endif
    std::string ret = "vr(";
    ret += buffer;
    ret += ")";
    return ret;
}

std::string IntegerExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    char* buffer = new char[32];
#ifdef _MSC_VER
    sprintf(buffer, "%d", this->Val);
#else
    snprintf(buffer, 32, "%d", this->Val);
#endif
    std::string ret = "vr(";
    ret += buffer;
    ret += ")";
    return ret;
}

std::string StringExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "vr(\"";
    ret += this->Val;
    ret += "\")";
    return ret;
}

std::string MacroExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "m";
    ret += this->name;
    ret += "()";
    return ret;
}

std::string VariableExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	std::string ret;
	
	if(this->scope == ASSIGN_SCOPE_GLOBAL)
		ret += "Gv";
	else if(this->scope == ASSIGN_SCOPE_LOCAL)
		ret += "Lv";
	else if(this->scope == ASSIGN_SCOPE_RESERVED)
		ret += "Rv";	
	else std::cout << "Error: Ambigious variable scope referenced when generating code: " << this->Name << "\n";
    ret += Name;
    return ret;
}


std::string AssignExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret;

	ret = "";
	
	int i=0;
	for(;i<depth;i++)
		ret += " ";
		
	if(this->scope == ASSIGN_SCOPE_GLOBAL)
		ret += "Gv";
	else if(this->scope == ASSIGN_SCOPE_LOCAL)
		ret += "Lv";
	ret += Name;
	ret += " ";
	ret += this->op;
	ret += " (";
	ret += this->EXPR->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
	ret += ")";

    return ret;
}


std::string ArrayAssignExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret;
    if(this->Name != ""){
        ret = "";
        
        int i=0;
        for(;i<depth;i++)
            ret += " ";

        ret += "(*(ArAcc(&(";
		if(this->scope == ASSIGN_SCOPE_GLOBAL)
			ret += "Gv";
		else if(this->scope == ASSIGN_SCOPE_LOCAL)
			ret += "Lv";
	  ret += this->Name;
	  ret += "), ";
	  i=0;
	  for(;i<this->Access->num_dimensions;i++)
	  {
		  ret += this->Access->subscript_expressions[i]->CodeGen(GeneratedCodeOpType,WantOpType,symtab, depth+1);
		  ret += ".Short()";
		  if((i+1)<this->Access->num_dimensions)
			  ret +=",";
	  }
	  ret += ")))";


        ret += " ";
        ret += this->Op;
        ret += " (";
        ret += this->Assign->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
        ret += ")";
    }else{
        std::cout << "Error: ArrayAssignAST Functionality not implemented.\n";
    }
    return ret;
}







std::string CallExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "";
        int i=0;
        for(;i<depth;i++)
            ret += " ";
    ret += "f";
    ret += this->Callee;
    ret+="(";
    int j=0;
    for(;j<this->Args->size();j++){
		ret += "(Variant*)&";
        ret += (*this->Args)[j]->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
        if((j+1)<this->Args->size())ret +=",";
    }
    ret+=")";
    return ret;
}


std::string BinaryExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "(";
    if(this->Op == "TO")
		ret += "(";
    ret += this->LHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
    if(this->Op == "TO")
		ret += "<= RvSwitch)";

    if(this->Op == "AND")
        ret += "&&";
    else if(this->Op == "OR")
        ret += "||";
    else if(this->Op == "=")//Uses different operator in C++ (in the overloaded variant class)
        ret += "|";
    else if(this->Op == "<>")//Uses different operator in C++
        ret += "!=";
    else if(this->Op == "TO")
		ret += "&& (RvSwitch <= ";
    else
        ret += this->Op;
    ret += this->RHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
    if(this->Op == "TO")
		ret += ")";
    ret += ")";
    return ret;
}


std::string UnaryExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "(";

    if(this->Op == "NOT")
        ret += "!";
    else
        ret += this->Op;

    ret += this->LHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
    ret += ")";
    return ret;
}


std::string DotOperatorAccessExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "var_resolve( \"";

	ret += name;
	ret += "\", &";

    ret += this->Wrapref->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
    ret += ")";
    return ret;
}


std::string CommonAssignExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret = "";

    ret += this->Assignto->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
    ret += " = ";
    ret += this->Initialiser->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
    return ret;
}




std::string CollectionExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    std::string ret;
    int j=0;
    for(;j<this->Code->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->Code)[j]->CodeGen(&temp,WantOpType,symtab, depth+1);
        ret += ";\n";
    }
    return ret;
}


std::string IfExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "if(";
    ret += this->condition->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "){\n";
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->IsTrueCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
        ret += ";\n";
    }
    if(this->IsFalseCode->size())
    {
    ret += "\n";
    j=0;
    for(;j<depth;j++)
      ret += " ";
    ret += "}else {\n\n";
        for(j=0;j<this->IsFalseCode->size();j++){
            int temp;
            int i=0;
            for(;i<(depth+3);i++)
                ret += " ";
            ret += (*this->IsFalseCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
            ret += ";\n";
        }
    }
    
    if(this->next_condition)
    {
      ret += "\n";
      j=0;
      for(;j<depth;j++)
        ret += " ";
      ret += "}else ";
      ret += this->next_condition->CodeGen(&temp,WantOpType,symtab, depth);
    }
    else{
    	j=0;
    	for(;j<depth;j++)
    		ret += " ";
    	ret += "}";
    }
    return ret;
}


std::string WhileExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "while(";
    ret += this->condition->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "){\n";
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->IsTrueCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
        ret += ";\n";
    }
    j=0;
    for(;j<depth;j++)
    ret += " ";
    ret += "}";
    return ret;
}


std::string ForToExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "for(";
    ret += this->initialassignnode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "; ";
    ret += this->condition->CodeGen(&temp,WantOpType,symtab, depth+1);
	ret += "; ";
	ret += this->incrementalassignnode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "){\n";
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->IsTrueCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
        ret += ";\n";
    }
    j=0;
    for(;j<depth;j++)
    ret += " ";
    ret += "}";
    return ret;
}


std::string ForInExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "{ vr RvIterationCounter = 0;\n";
        int i=0;
        for(;i<(depth+2);i++)
            ret += " ";
    ret += "for(";
    ret += this->actual_assignnode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "; RvIterationCounter < vr((int)(";
    ret += this->iterativenode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += ".arrayaccess->totalelements)); ){\n";
    
	i=0;
    for(;i<(depth+2);i++)
        ret += " ";
    ret += this->initialassignnode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += " = ";
    ret += this->iterativenode->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "[RvIterationCounter.Int()];   RvIterationCounter+=1;\n\n";
    
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+5);i++)
            ret += " ";
        ret += (*this->IsTrueCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+7);
        ret += ";\n";
    }
    j=0;
    for(;j<depth+2;j++)
    ret += " ";
    ret += "}}";
    return ret;
}




std::string DoUntilExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "do{\n";
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        int temp;
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->IsTrueCode)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
        ret += ";\n";
    }
    j=0;
    for(;j<depth;j++)
    ret += " ";
    ret += "}while(";
    ret += this->condition->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += ")\n";
    return ret;
}




std::string ArrayAccessExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
	std::string ret = "(*(ArAcc(&(";
	ret += this->Wrapref->CodeGen(&temp,WantOpType,symtab, depth+2);
	ret += "), ";
	int i=0;
	for(;i<this->Access->num_dimensions;i++)
	{
		ret += this->Access->subscript_expressions[i]->CodeGen(&temp,WantOpType,symtab, depth+1);
		ret += ".Int()";
		if((i+1)<this->Access->num_dimensions)
			ret +=",";
	}
	ret += ")))";
	return ret;
}

std::string ArrayDecExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
      int temp;
	char buffer[32];
	std::string ret = "";
       int i=0;
        for(;i<depth;i++)
            ret += " ";
        
	if(this->scope == ASSIGN_SCOPE_GLOBAL)
		ret += "Gv";
	else if(this->scope == ASSIGN_SCOPE_LOCAL)
		ret += "Lv";
	ret += this->Name;

	ret += " = vr(ARRAY_ARRAYTYPE, ";
	itoa(this->Access->num_dimensions, buffer, 32);
	ret += buffer;
	ret += ", ";
	i=0;
	for(;i<this->Access->num_dimensions;i++)
	{
		ret += "(";
		ret += this->Access->subscript_expressions[i]->CodeGen(&temp,WantOpType,symtab, depth+1);
		ret += ").Int()";
		if((i+1)<this->Access->num_dimensions)
			ret +=",";
	}
	ret += ")";
	return ret;
}

std::string CaseExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp, j;
    std::string ret = "if(";
    ret += this->condition->CodeGen(&temp,WantOpType,symtab, depth+1);
    ret += "){\n";
	
	ret += this->CaseCodeGen(&temp,WantOpType,symtab, depth);
    
    
    if(this->next != NULL)
    {
      ret += "\n";
      j=0;
      for(;j<depth;j++)
        ret += " ";
      ret += "}else ";
      ret += this->next->CodeGen(&temp,WantOpType,symtab, depth);
    }
    else{
    	j=0;
    	for(;j<depth;j++)
    		ret += " ";
    	ret += "}";
    }
    
    return ret;
}

std::string CaseExprAST::CaseCodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	int temp;
	std::string ret;
	int j=0;
    for(;j<this->Code->size();j++){
        int i=0;
        for(;i<(depth+3);i++)
            ret += " ";
        ret += (*this->Code)[j]->CodeGen(&temp,WantOpType,symtab, depth+2);
        ret += ";\n";
    }
    
    if(this->ContinueCase){
		CaseExprAST* temp2 = (CaseExprAST*)this->next;
		ret += temp2->CaseCodeGen(&temp,WantOpType,symtab, depth);
	}
	return ret;
}


std::string SelectExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "";

    ret += "\n";

    int i=0;
    for(;i<(depth+3);i++)
        ret += " ";
    ret += this->next->CodeGen(&temp,WantOpType,symtab, depth+2);
    ret += ";\n";


    return ret;
}




std::string SwitchExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "{\n";

	int i=0;
    for(;i<(depth+3);i++)
        ret += " ";
    
    ret += "vr RvSwitch = ";
    ret += this->switchcond->CodeGen(&temp,WantOpType,symtab, depth+2);
    ret += ";";

    ret += "\n";

    i=0;
    for(;i<(depth+3);i++)
        ret += " ";
    ret += this->next->CodeGen(&temp,WantOpType,symtab, depth+2);
    ret += ";\n";

	i=0;
    for(;i<(depth);i++)
        ret += " ";
        
    ret += "}";

    return ret;
}



std::string GeneralStatementExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
    int temp;
    std::string ret = "";

    ret += "\n";

    int i=0;
    for(;i<(depth+3);i++)
        ret += " ";
    if (this->Name == "CONTINUELOOP")
		ret += "continue";
    else if (this->Name == "EXITLOOP")
		ret += "break";
    else if (this->Name == "RETURN")
		ret += "return";
    ret += " ";
    if(this->Assign)
		ret += this->Assign->CodeGen(&temp,WantOpType,symtab, depth+2);
    ret += ";\n";


    return ret;
}
