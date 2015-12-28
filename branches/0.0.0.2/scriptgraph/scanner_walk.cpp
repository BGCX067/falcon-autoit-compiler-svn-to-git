
#include "../stdafx.h"
#include "../tokens.h"
#include "nodes.h"



void NumberExprAST::SymboltableBuild(int level, Symtable* sym)
{
	return;
}
void IntegerExprAST::SymboltableBuild(int level, Symtable* sym)
{
	return;
}
void StringExprAST::SymboltableBuild(int level, Symtable* sym)
{
	return;
}
void VariableExprAST::SymboltableBuild(int level, Symtable* sym)
{
	return;
}





void AssignExprAST::SymboltableBuild(int level, Symtable* sym)
{
	if(!SymbolTable_Exists(sym, this->Name, this->ScopeName))
	{
		SymtableEntry* entry = new SymtableEntry;
		entry->scope = this->scope;
		entry->varname = this->Name;
		entry->domain = this->ScopeName;
		entry->discovered_depth = level;
		sym->entries.push_back(entry);
	}

	this->EXPR->SymboltableBuild(level+1, sym);
}

void BinaryExprAST::SymboltableBuild(int level, Symtable* sym)
{
	this->LHS->SymboltableBuild(level+1, sym);
	this->RHS->SymboltableBuild(level+1, sym);
}

void UnaryExprAST::SymboltableBuild(int level, Symtable* sym)
{
	this->LHS->SymboltableBuild(level+1, sym);
}




void CollectionExprAST::SymboltableBuild(int level, Symtable* sym)
{
	int j=0;
	for(;j<this->Code->size();j++){
		(*this->Code)[j]->SymboltableBuild(level+1, sym);
	}
}


bool SymbolTable_Exists(Symtable* Sym, std::string name, std::string domain)
{
	int i=0;
	for(;i < Sym->entries.size(); i++)
	{
		if((Sym->entries[i]->varname == name) && (Sym->entries[i]->domain == domain))
			return true;
	}
	return false;
}