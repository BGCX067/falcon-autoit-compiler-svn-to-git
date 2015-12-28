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
				
			case VARIANT_VARIABLE_TOK:
				if ((*code)[(*code_position)+1]->type == EQUAL_TOK)	//Assignment
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
					ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, ASSIGN_SCOPE_AMBIGUOUS, domain);	//Create the assignment node.
					codebranch->push_back(assignbranch);	//Add the assignment to the collection (node) of this recursion.
				}
				break;
				
		}
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
			case NUMHEX_TOK: case INT32_TOK: INT64_TOK: case DOUBLE_TOK: case STRINGLITERAL_TOK:
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
