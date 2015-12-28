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
void MacroExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << '@' << this->name << '\n';
    std::cout.flush();
}
void VariableExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << '$' << this->Name << " Scope: " << this->scope << '\n';
    std::cout.flush();
}





void AssignExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << "Assignment: " << this->op << " $" << this->Name << " Scope: " << this->scope << " Scopename: '" << this->ScopeName << "'\n";
    this->EXPR->DebugPrint(level+1);
}

void ArrayAssignExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << "Array Assignment: " << this->Op << " $" << this->Name << " Scope: " << this->scope << '\n';
    this->Assign->DebugPrint(level+1);

    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->DebugPrint(level+2);
    }
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

void DotOperatorAccessExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << "DOT Operator: " << this->name << " \n";
    this->Wrapref->DebugPrint(level+1);
}


void CommonAssignExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << "Generic Assignment:  \n";
    this->Assignto->DebugPrint(level+1);
    std::cout << '\n';
    this->Initialiser->DebugPrint(level+1);
}




void CallExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
        
    std::cout << "Func Call: " << this->Callee << " - with " << this->Args->size() << " arguments.\n";
    int j=0;
    for(;j<this->Args->size();j++){
        (*this->Args)[j]->DebugPrint(level+1);
    }
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

void IfExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "IF BLOCK::" << '\n' << '\n';
    this->condition->DebugPrint(level+1);

    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->DebugPrint(level+2);
    }
    if(this->IsFalseCode->size())
    {
        for(i=0;i<level;i++)
            std::cout << "  ";
        std::cout << "ELSE:\n";
        int j=0;
        for(;j<this->IsFalseCode->size();j++){
            (*this->IsFalseCode)[j]->DebugPrint(level+2);
        }
    }
	if(this->next_condition)
		this->next_condition->DebugPrint(level);
}


void WhileExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "WHILE BLOCK::" << '\n' << '\n';
    this->condition->DebugPrint(level+1);

    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->DebugPrint(level+2);
    }

}


void ForToExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "ForTo BLOCK::" << '\n' << '\n';
    
    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Condition: \n";
    this->condition->DebugPrint(level+1);

    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Initial Assignment: \n";
    this->initialassignnode->DebugPrint(level+1);

    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Increment value: \n";
    this->incrementalassignnode->DebugPrint(level+1);
	std::cout << "\n\n";

    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->DebugPrint(level+2);
    }

}


void ForInExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "ForIn BLOCK::" << '\n' << '\n';
    

    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Assignment Identifier: \n";
    this->initialassignnode->DebugPrint(level+1);

    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Assignment Node: \n";
    this->actual_assignnode->DebugPrint(level+1);

    for(i=0;i<level;i++)
        std::cout << "  ";
    std::cout << "Iterative Identifier: \n";
    this->iterativenode->DebugPrint(level+1);
	std::cout << "\n\n";

    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->DebugPrint(level+2);
    }

}


void DoUntilExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "DO-UNTIL BLOCK::" << '\n' << '\n';
    this->condition->DebugPrint(level+1);

    int j=0;
    for(;j<this->IsTrueCode->size();j++){
        (*this->IsTrueCode)[j]->DebugPrint(level+2);
    }

}



void CaseExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "CASE BLOCK::" << '\n' << '\n';
    this->condition->DebugPrint(level+1);

    int j=0;
    for(;j<this->Code->size();j++){
        (*this->Code)[j]->DebugPrint(level+2);
    }
    if(this->next != NULL)this->next->DebugPrint(level+2);
}

void SelectExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "SELECT:\n";
    this->next->DebugPrint(level+2);
}


void SwitchExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    std::cout << "SWITCH: \n";
    this->switchcond->DebugPrint(level+2);
    std::cout << "\n";
    this->next->DebugPrint(level+2);
}





void ArrayAccessExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    
    this->Wrapref->DebugPrint(level);

    std::cout << " ArAccess: " << '\n';

    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->DebugPrint(level+2);
    }

}

void ArrayDecExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";
    

    std::cout << " Array Declaration: " << this->Name << " Scope: " << this->scope << " '" << this->ScopeName << "'\n";

    int j=0;
    for(;j<this->Access->num_dimensions;j++){
        this->Access->subscript_expressions[j]->DebugPrint(level+2);
    }

}



void GeneralStatementExprAST::DebugPrint(int level)
{
    int i=0;
    for(;i<level;i++)
        std::cout << "  ";

	std::cout << this->Name << "\n";
	if(this->Assign)
		this->Assign->DebugPrint(level+1);

}
