#include "../stdafx.h"
#include "../tokens.h"
#include "nodes.h"

/*
 * 	TODO:
 *  - Add Code for scoping variable assignment correctly - DONE
 *  - Don't forget the above with Array declarations - DONE
*/



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
void MacroExprAST::SymboltableBuild(int level, Symtable* sym)
{
    return;
}


void VariableExprAST::SymboltableBuild(int level, Symtable* sym)
{
	if(this->scope == ASSIGN_SCOPE_AMBIGUOUS)//We are not sure wether node references local or global var. Disambiguate here.
	{
		if(sym->primarydomain == "")//Global code - no problem
		{
			this->scope = ASSIGN_SCOPE_GLOBAL;
			return;
		}else
		{//local function code - could be a LOCAL or GLOBAL reference
		
			if(SymbolTable_Exists(sym, this->Name, sym->primarydomain))//check if locally declared var
			{
				this->scope = ASSIGN_SCOPE_LOCAL;
				return;
			}
			else if(SymbolTable_Exists(sym, this->Name, ""))//check if globally declared var
			{
				this->scope = ASSIGN_SCOPE_GLOBAL;
				return;
			}else std::cout << "Error: Undefined variable encountered whilst attempting to de-ambiguate scope: " << this->Name << "\n";
		
		}
	}
    return;
}


void ArrayAssignExprAST::SymboltableBuild(int level, Symtable* sym)
{
	

	if(sym->primarydomain == "")//Global code - no problem
	{
		this->scope = ASSIGN_SCOPE_GLOBAL;
	}else
	{//local function code - could be a LOCAL or GLOBAL reference
	
		if(SymbolTable_Exists(sym, this->Name, sym->primarydomain))//check if locally declared var
		{
			this->scope = ASSIGN_SCOPE_LOCAL;
		}
		else if(SymbolTable_Exists(sym, this->Name, ""))//check if globally declared var
		{
			this->scope = ASSIGN_SCOPE_GLOBAL;
		}else std::cout << "Error: Undefined variable encountered whilst attempting to de-ambiguate scope (array assignment): " << this->Name << "\n";
	
	}

	
    this->Assign->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->SymboltableBuild(level+1, sym);
    }
    return;
}




void AssignExprAST::SymboltableBuild(int level, Symtable* sym)
{
	if(this->scope == ASSIGN_SCOPE_AMBIGUOUS)//We are not sure wether node references local or global var. Disambiguate here.
	{
		if(sym->primarydomain == "")//Global code - no problem
		{
			this->scope = ASSIGN_SCOPE_GLOBAL;
			this->ScopeName = "";
		}else
		{//local function code - could be a LOCAL or GLOBAL reference
		
			if(SymbolTable_Exists(sym, this->Name, ""))//check if globally declared var
			{
				this->scope = ASSIGN_SCOPE_GLOBAL;
				this->ScopeName = "";
			}else{
				this->scope = ASSIGN_SCOPE_LOCAL;
			}
		}
	}else if(this->scope == ASSIGN_SCOPE_GLOBAL)
		this->ScopeName = "";
	
    if(this->op == "=")
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

void DotOperatorAccessExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->Wrapref->SymboltableBuild(level+1, sym);
}

void CommonAssignExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->Assignto->SymboltableBuild(level+1, sym);
    this->Initialiser->SymboltableBuild(level+1, sym);
}



void CallExprAST::SymboltableBuild(int level, Symtable* sym)
{
    int j=0;
    for(;j<this->Args->size();j++){
        (*this->Args)[j]->SymboltableBuild(level+1, sym);
    }
}





void CollectionExprAST::SymboltableBuild(int level, Symtable* sym)
{
    int j=0;
    for(;j<this->Code->size();j++){
        (*this->Code)[j]->SymboltableBuild(level+1, sym);
    }
}

void IfExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->condition->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->SymboltableBuild(level+1, sym);
    }
    Symtable* temp = Symboltable_Duplicate(sym);
    for(j=0;j<this->IsFalseCode->size();j++){
        (*this->IsFalseCode)[j]->SymboltableBuild(level+1, temp);
    }
    Symtable* temp1 = 0;
    if(this->next_condition){
	  temp1 = Symboltable_Duplicate(sym);
      this->next_condition->SymboltableBuild(level+1, temp);
	}
	//Bugfix #1
	Symboltable_BackPropergateNewSymbols(sym, temp);
	if(this->next_condition)Symboltable_BackPropergateNewSymbols(sym, temp1);
}



void WhileExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->condition->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->SymboltableBuild(level+1, sym);
    }
}

void ForToExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->initialassignnode->SymboltableBuild(level+1, sym);
    this->condition->SymboltableBuild(level+1, sym);
    this->incrementalassignnode->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->SymboltableBuild(level+1, sym);
    }
}


void ForInExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->actual_assignnode->SymboltableBuild(level+1, sym);
    this->initialassignnode->SymboltableBuild(level+1, sym);
    this->iterativenode->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->SymboltableBuild(level+1, sym);
    }
}


void DoUntilExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->condition->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->SymboltableBuild(level+1, sym);
    }
}


void CaseExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->condition->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->Code->size();j++){
        (*this->Code)[j]->SymboltableBuild(level+1, sym);
    }
    if(this->next != NULL){
		Symtable* temp = Symboltable_Duplicate(sym);
		this->next->SymboltableBuild(level+1, temp);
		Symboltable_BackPropergateNewSymbols(sym, temp);//Bugfix #1
	}
}

void SelectExprAST::SymboltableBuild(int level, Symtable* sym)
{
	this->next->SymboltableBuild(level+1, sym);
}

void SwitchExprAST::SymboltableBuild(int level, Symtable* sym)
{
	this->switchcond->SymboltableBuild(level+1, sym);
	this->next->SymboltableBuild(level+1, sym);
}


void ArrayAccessExprAST::SymboltableBuild(int level, Symtable* sym)
{
    this->Wrapref->SymboltableBuild(level+1, sym);
    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->SymboltableBuild(level+1, sym);
    }
}


void ArrayDecExprAST::SymboltableBuild(int level, Symtable* sym)
{
	if(this->scope == ASSIGN_SCOPE_AMBIGUOUS)//We are not sure wether node references local or global var. Disambiguate here.
	{
		if(sym->primarydomain == "")//Global code - no problem
		{
			this->scope = ASSIGN_SCOPE_GLOBAL;
			this->ScopeName = "";
		}else
		{//local function code - could be a LOCAL or GLOBAL reference
		
			if(SymbolTable_Exists(sym, this->Name, ""))//check if globally declared var
			{
				this->scope = ASSIGN_SCOPE_GLOBAL;
				this->ScopeName = "";
			}else{
				this->scope = ASSIGN_SCOPE_LOCAL;
			}
		}
	}else if(this->scope == ASSIGN_SCOPE_GLOBAL)
		this->ScopeName = "";



        if(!SymbolTable_Exists(sym, this->Name, this->ScopeName))
        {
            SymtableEntry* entry = new SymtableEntry;
            entry->scope = this->scope;
            entry->varname = this->Name;
            entry->domain = this->ScopeName;
            entry->discovered_depth = level;
            sym->entries.push_back(entry);
        }

    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->SymboltableBuild(level+1, sym);
    }
}



void GeneralStatementExprAST::SymboltableBuild(int level, Symtable* sym)
{
	if(this->Assign)
		this->Assign->SymboltableBuild(level+1, sym);
};



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


Symtable* Symboltable_Duplicate(Symtable* Sym)
{
	Symtable* ret = new Symtable;
	
	ret->primarydomain = Sym->primarydomain;

    int i=0;
    for(;i < Sym->entries.size(); i++)
    {
		SymtableEntry* newent = new SymtableEntry;
		newent->discovered_depth = Sym->entries[i]->discovered_depth;
		newent->scope = Sym->entries[i]->scope;
		newent->varname = Sym->entries[i]->varname;
		newent->domain = Sym->entries[i]->domain;
		ret->entries.push_back(newent);
    }

	return ret;
}


void Symboltable_ClearLocalEntries(Symtable* Sym)//deletes all local symbols
{
	int i=0;
    for(;i < Sym->entries.size(); i++)
    {

		if(Sym->entries[i]->domain != ""){
			delete Sym->entries[i];
			Sym->entries.erase(Sym->entries.begin()+i);
		}

    }

}

void Symboltable_BackPropergateNewSymbols(Symtable* Primary, Symtable* Subset)
{
    int i=0;
    for(;i < Subset->entries.size(); i++)
    {
		if(SymbolTable_Exists(Primary, Subset->entries[i]->varname, Subset->entries[i]->domain))
		{
			;//It exists, dont do anything to that symbol.
		}else {
			//Symbol is new; back propergate it.
			SymtableEntry* newent = new SymtableEntry;
			newent->discovered_depth = Subset->entries[i]->discovered_depth;
			newent->scope = Subset->entries[i]->scope;
			newent->varname = Subset->entries[i]->varname;
			newent->domain = Subset->entries[i]->domain;
			Primary->entries.push_back(newent);
		}
	}
}
