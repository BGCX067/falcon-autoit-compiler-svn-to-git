
#include "../stdafx.h"
#include "../tokens.h"
#include "nodes.h"



void NumberExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << this->Val << '\n';
	std::cout.flush();
}
void IntegerExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << this->Val << '\n';
	std::cout.flush();
}
void StringExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << this->Val << '\n';
	std::cout.flush();
}
void VariableExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << '$' << this->Name << '\n';
	std::cout.flush();
}





void AssignExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << "Assignment: $" << this->Name << " Scope: " << this->scope << " Scopename: '" << this->ScopeName << "'\n";
	this->EXPR->DebugPrint(level+1);
}

void BinaryExprAST::DebugPrint(int level)
{
	std::cout.flush();
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << "OP: " << this->Op << '\n';
	this->LHS->DebugPrint(level+1);
	this->RHS->DebugPrint(level+1);
}

void UnaryExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	std::cout << "OP: " << this->Op << " (Unary)\n";
	this->LHS->DebugPrint(level+1);
}




void CollectionExprAST::DebugPrint(int level)
{
	int i=0;
	for(;i<level;i++)
		std::cout << "  ";
		
	int j=0;
	for(;j<this->Code->size();j++){
		(*this->Code)[j]->DebugPrint(level+1);
	}
}
