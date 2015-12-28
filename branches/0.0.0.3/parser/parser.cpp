/* Recursive Decent Parser - Processes code to AST:
    Licensed under MIT.

    Expression parsing IS DELEGATED to parser_exp.cpp
    
    6/3/12: Crash FIXED.
*/

#include "../stdafx.h"
#include "../tokens.h"
#include "../scriptgraph/nodes.h"
#include "parser.h"

///Recursive entry point for parsing.
int ParseCode(std::vector<token*>* code, int* code_position, CollectionExprAST** ASTParent, std::string domain, char context)
{

	//Build our level in the AST. (these things are on heap to prevent stack overflow)
	std::vector<ExprAST*>* CodeCollection = new std::vector<ExprAST*>;
	*ASTParent = new CollectionExprAST(CodeCollection); //Our collection of nodes.
	CollectionExprAST* NodeCollection = *ASTParent;
	std::vector<ExprAST*>* codebranch = (std::vector<ExprAST*>*)NodeCollection->GetCollection();

	int NestedLevel = 0; //Used for keeping track of nested control blocks.


	for(;(*code_position)<code->size();(*code_position)++)
	{
		switch((*code)[(*code_position)]->type)
		{
			case EOL_TOK:
				break;
				
			case CONSTANT_KEYWORD_TOK: case GLOBAL_KEYWORD_TOK: case LOCAL_KEYWORD_TOK: case DIM_KEYWORD_TOK:
				if(!Parser_HandleVarDef(code, code_position, codebranch, domain))
					return 0;//Error
				break;
				
			case UNKNOWN_TOK:
				if((*code)[(*code_position)+1]->type == LFT_BRACKET_TOK)//Function call
				{
					//Collect expression
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression

					if(!Parser_CollectExpression(code,code_position,expression))
						return 0; //error, unwind recursion.			
							
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					codebranch->push_back(exprbranch);
				}
				
			case VARIANT_VARIABLE_TOK:
				if (((*code)[(*code_position)+1]->type == EQUAL_TOK) || ((*code)[(*code_position)+1]->type == REL_ASSIGN_TOK))	//Assignment or relative assignment
				{
					std::string op = (*code)[(*code_position)+1]->data;
					std::string varname = (*code)[(*code_position)]->data;
					(*code_position) += 2; //Get past two tokens
					
					//Collect expression
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression

					if(!Parser_CollectExpression(code,code_position,expression))
						return 0; //error, unwind recursion.			
							
						
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, ASSIGN_SCOPE_AMBIGUOUS, domain, op);	//Create the assignment node.
					codebranch->push_back(assignbranch);	//Add the assignment to the collection (node) of this recursion.
				}
				break;
				
		}
	}

 
	return 1;
}







int Parser_HandleVarDef(std::vector<token*>* code, int* code_position, std::vector<ExprAST*>* codebranch, std::string domain)
{
	int scope = ASSIGN_SCOPE_AMBIGUOUS; //Def from nodes.h
	int is_enum = false;
	std::string enum_op = "+";
	std::string last_var = "";
	std::string step = "1";
	
	//Jump past nonsense
	if((*code)[(*code_position)]->type == CONSTANT_KEYWORD_TOK)(*code_position)++;
	
	//Get scope type.
	switch((*code)[(*code_position)]->type){
		case GLOBAL_KEYWORD_TOK:
			scope = ASSIGN_SCOPE_GLOBAL;
			(*code_position)++;
			break;
		case LOCAL_KEYWORD_TOK:
			if(domain=="")scope = ASSIGN_SCOPE_GLOBAL;
			else scope = ASSIGN_SCOPE_LOCAL;
			(*code_position)++;
			break;
		case DIM_KEYWORD_TOK:
			if(domain=="")scope = ASSIGN_SCOPE_GLOBAL;
			(*code_position)++;
			break;
	}

	//Jump past nonsense
	if((*code)[(*code_position)]->type == CONSTANT_KEYWORD_TOK)(*code_position)++;
	
	//Check if Enumeration
	if((*code)[(*code_position)]->type == ENUM_KEYWORD_TOK)		//Shit just got real
	{
		(*code_position)++;
		is_enum = true;
		if((*code)[(*code_position)]->type == FOR_TOK)//STEP keyword
		{	//User defined step?
			(*code_position)++;
			if((*code)[(*code_position)]->type == MATH_OP_TOK)
			{
				enum_op = (*code)[(*code_position)]->data;
				(*code_position)++;
				step = (*code)[(*code_position)]->data;
				(*code_position)++;
			}else{
				step = (*code)[(*code_position)]->data;
				(*code_position)++;
			}
		}
	}
	
	
	
	
	//Now that that nosense is out of the way, time to start collecting expressions.
	while((*code)[(*code_position)]->type != EOL_TOK)
	{
		//Straight assignments.
		if((*code)[(*code_position)+1]->type == EQUAL_TOK)
		{
			std::string varname = (*code)[(*code_position)]->data;
			(*code_position) += 2; //Get past two tokens
			//Collect expression
			std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
			expression->reserve(150); //Max num of tokens in expression
			if(!Parser_CollectExpression(code,code_position,expression))
				return 0; //error, unwind recursion.
			std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
			ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
			ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, scope, domain, "=");	//Create the assignment node.
			codebranch->push_back(assignbranch);	//Add the assignment to the collection (node) of this recursion.
			last_var = varname;
		}
		
		//Reference with default initialisation
		else if(((*code)[(*code_position)+1]->type == PARAM_SEPARATOR_TOK) || (*code)[(*code_position)+1]->type == EOL_TOK)
		{
			std::string varname = (*code)[(*code_position)]->data;
			(*code_position)++; //Get past two tokens
			//Create spot for expression
			std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
			expression->reserve(150); //Max num of tokens in expression
			
			if(!is_enum)
			{				//zero initialised
				token* tok = new token;
				tok->type = INT32_TOK;
				tok->data = "0";
				expression->push_back(tok);
			}else{
				if(last_var=="")
				{//First one: zero initialise.
					token* tok = new token;
					tok->type = INT32_TOK;
					tok->data = "0";
					expression->push_back(tok);
				}else{//Initialise based on other shit.
					token* ref = new token;
					ref->type = VARIANT_VARIABLE_TOK;
					ref->data = last_var;
					expression->push_back(ref);
					token* stepv = new token;
					stepv->type = INT32_TOK;
					stepv->data = step;
					expression->push_back(stepv);
					token* oper = new token;
					oper->type = MATH_OP_TOK;
					oper->data = enum_op;
					expression->push_back(oper);
				}
			}
			
			std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
			ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
			ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, scope, domain, "=");	//Create the assignment node.
			codebranch->push_back(assignbranch);	//Add the assignment to the collection (node) of this recursion.
			last_var = varname;
		}
		
		if((*code)[(*code_position)]->type != EOL_TOK)(*code_position)++;
	}
	

	return 1;
}




/*
 * Pretty important function.
 * Collects the tokens of a single expression based on grammar rules
 * This works because of AutoIT's recursive structure for expressions
 * means depth counting will work, and expressions are delimited by keywords or assignments.
 * 
 * Parser_CollectExpression ( pointer to tokenvector, pointer to position in code, pointer to vector to write to)
 * returns 1 on success.
*/

int Parser_CollectExpression(std::vector<token*>* code, int* code_position, std::vector<token*>* expression)
{
	long int nestedlevel = 0;
	bool breakflag = false;
	
	while(true)
	{

		switch ((*code)[(*code_position)]->type)
		{
			case VARIANT_VARIABLE_TOK: case ARRAY_VARIABLE_TOK: case MACRO_TOK:
			case COMP_OP_TOK: case MATH_OP_TOK: case EQUAL_TOK: case DOUBLE_EQUAL_TOK:
			case NUMHEX_TOK: case INT32_TOK: case INT64_TOK: case DOUBLE_TOK: case STRINGLITERAL_TOK:
			case FUNC_UNKNOWN_TOK: case FUNC_BUILTIN_TOK: case OOP_DOT_TOK: case UNKNOWN_TOK:
				expression->push_back((*code)[(*code_position)]);
				break;
				
				
			case LFT_ARRAY_PARANTH_TOK: case LFT_BRACKET_TOK:
				nestedlevel++;
				expression->push_back((*code)[(*code_position)]);
				break;
				
			case RHT_ARRAY_PARANTH_TOK: case RHT_BRACKET_TOK:
				nestedlevel--;
				if(nestedlevel<0){
					breakflag=true;
				}else{
					expression->push_back((*code)[(*code_position)]);
				}
				break;
				
			case PARAM_SEPARATOR_TOK:
				if (nestedlevel<1){
					breakflag=true;
				}else{
					expression->push_back((*code)[(*code_position)]);
				}
				break;
			
			default:
				if ((*code)[(*code_position)]->type > 300){
					//Max expression size exceeded error.
					std::cout << "ERROR: (Exp parser) Max tok length (150) in exp exceeded: " << (*code_position) << '\n';
					return 0;
				}
				breakflag = true;
				break;
			
		}
		if(breakflag)break;
		(*code_position)++;
		if((*code_position) >= code->size())
		{	//Out of bounds error
			std::cout << "Out of Bounds error! (Exp Parser)\n";
			return 0;
		}
	}
	return 1;
}


///Debug function for testing that the CollectExpression function operates as expected.
void PrintExpression(std::vector<token*>* expression)
{
	int i = 0;
	for(;i<expression->size();i++)
	{
		std::cout << (*expression)[i]->type << ' ';
	}
	std::cout << '\n';
}
