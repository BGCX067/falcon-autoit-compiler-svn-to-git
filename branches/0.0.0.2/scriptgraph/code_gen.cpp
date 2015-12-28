
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

std::string VariableExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	std::string ret = "Gv";
	ret += Name;
	return ret;
}


std::string AssignExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	std::string ret;
	if(this->Name != ""){
		ret = "";
		
		int i=0;
		for(;i<depth;i++)
			ret += " ";
		
		ret += emit_symbol_dereference_global(Name);
		ret += " = (";
		ret += this->EXPR->CodeGen(GeneratedCodeOpType, WantOpType, symtab, depth+1);
		ret += ")";
	}else{
		std::cout << "Error: AssignAST Functionality not implemented.\n";
	}
	return ret;
}

std::string CallExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	return "";
}


std::string BinaryExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	std::string ret = "(";
	ret += this->LHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
	ret += this->Op;
	ret += this->RHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
	ret += ")";
	return ret;
}


std::string UnaryExprAST::CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth)
{
	std::string ret = "(";
	ret += this->Op;
	ret += this->LHS->CodeGen(GeneratedCodeOpType, AST_TYPE_NUM, symtab, depth+1);
	ret += ")";
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

