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
int ParseCode(std::vector<token*>* code, int* code_position, std::vector<ExprAST*>* codebranch, std::string domain, char context, ExprAST* parentblock)
{

    for(;(*code_position)<code->size();(*code_position)++)
    {
        switch(context)//Dealing with recursion context
        {
        case '\0':
            break;

	  case FOR_CONTEXT:
            if((*code)[(*code_position)]->type == FOR_TOK)
            {
				if((*code)[(*code_position)]->data == "NEXT")
				{
					(*code_position)++;
					return 1;
				}
			}
			break;

	  case USERFUNC_CONTEXT:
			if((*code)[(*code_position)]->type == FUNC_END_TOK){//finished parsing a user function, so our recursive context is finished. Return.
				(*code_position)++;
				return 1;
			}
			break;

	  case DO_CONTEXT:
            if((*code)[(*code_position)]->type == DO_TOK)
            {
                if((*code)[(*code_position)]->data == "UNTIL")//finished parsing a DO-UNTIL block, so our recursive context is finished. Return.
                {
                    (*code_position)++;
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression
					
                    token* ref = new token; //This is just hackz to make the DO structure.
                    ref->type = COMP_OP_TOK;
                    ref->data = "NOT";
                    expression->push_back(ref);
                    ref = new token;
                    ref->type = LFT_BRACKET_TOK;
                    ref->data = "(";
                    expression->push_back(ref); 
                    
					if(!Parser_CollectExpression(code,code_position,expression))//Collect conditional expression
					  return 0; //error, unwind recursion.
                    ref = new token;
                    ref->type = RHT_BRACKET_TOK;
                    ref->data = ")";
                    expression->push_back(ref); 
                    
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					DoUntilExprAST* par = (DoUntilExprAST*)parentblock;
					par->condition = conditionbranch;
                    return 1;
				}
			}	  
			break;


	  case SWITCH_CONTEXT:
            if((*code)[(*code_position)]->type == SWITCH_TOK)
            {
                if((*code)[(*code_position)]->data == "ENDSWITCH")//finished parsing a SWITCH block, so our recursive context is finished. Return.
                {
                    (*code_position)++;
                    return 1;
				}
			}
			if(((*code)[(*code_position)]->type == CASE_TOK) && ((*code)[(*code_position)]->data == "CASE"))
			{
				(*code_position)++;
				if((*code)[(*code_position)]->type  == IF_TOK)//CASE ELSE
				{
					(*code_position)++;
					ExprAST* conditionbranch = new IntegerExprAST(1);
					std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
					CaseExprAST* par = (CaseExprAST*)parentblock;
					par->next = new CaseExprAST(tcode, conditionbranch);
					//Recurse to parse content of control block
					if(!ParseCode(code,code_position,tcode, domain, SWITCH_CONTEXT, (ExprAST*)par->next))
						  return 0;//Error
				}else{
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression
					if(!Parser_CollectExpression(code,code_position,expression, true))
					  return 0; //error, unwind recursion.
					  
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

					//If it is a primary expression, it needs to be compared with the post-switch nonterminal.
					if(!Expression_IsTo( expression))
					{//If it is NOT a (expression) TO (expression) type switch expression.
						VariableExprAST* var = new VariableExprAST(std::string("Switch"));
						var->scope = ASSIGN_SCOPE_RESERVED;
						ExprAST* oper = new BinaryExprAST("=", var, conditionbranch);
						conditionbranch = oper;
					}
					  
					//CASE expression collected, now recurse for rest.
					std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
					CaseExprAST* par = (CaseExprAST*)parentblock;
					par->next = new CaseExprAST(tcode, conditionbranch);
					//Recurse to parse content of control block
					if(!ParseCode(code,code_position,tcode, domain, SWITCH_CONTEXT, (ExprAST*)par->next))
						  return 0;//Error
				}
				return 1;
			}
			break;

	  case SELECT_CONTEXT:
            if((*code)[(*code_position)]->type == SELECT_TOK)
            {
                if((*code)[(*code_position)]->data == "ENDSELECT")//End of SELECT block, thus return from recursion context.
                {
                    (*code_position)++;
                    return 1;
				}
			}
			if(((*code)[(*code_position)]->type == CASE_TOK) && ((*code)[(*code_position)]->data == "CASE"))
			{//Case Blocks create a new recursion context & node INSIDE the current CASE node. When they hit the endswitch/select, all case contexts cascade.
				(*code_position)++;
				if((*code)[(*code_position)]->type  == IF_TOK)//CASE ELSE
				{
					(*code_position)++;
					ExprAST* conditionbranch = new IntegerExprAST(1);
					std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
					CaseExprAST* par = (CaseExprAST*)parentblock;
					par->next = new CaseExprAST(tcode, conditionbranch);
					//Recurse to parse content of control block
					if(!ParseCode(code,code_position,tcode, domain, SELECT_CONTEXT, (ExprAST*)par->next))
						  return 0;//Error
				}else{
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression
					if(!Parser_CollectExpression(code,code_position,expression))
					  return 0; //error, unwind recursion.
					  
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					  
					//CASE expression collected, now recurse for rest.
					std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
					CaseExprAST* par = (CaseExprAST*)parentblock;
					par->next = new CaseExprAST(tcode, conditionbranch);
					//Recurse to parse content of control block
					if(!ParseCode(code,code_position,tcode, domain, SELECT_CONTEXT, (ExprAST*)par->next))
						  return 0;//Error
				}
				return 1;
			}
		break;


	  case WHILE_CONTEXT:
            if((*code)[(*code_position)]->type == WHILE_TOK)//End of the WHILELOOP context. Unwind recursion.
            {
                if((*code)[(*code_position)]->data == "WEND")
                {
                    (*code_position)++;
                    return 1;
				}
			}
		break;

        case IF_CONTEXT:
            if((*code)[(*code_position)]->type == IF_TOK)
            {
                if((*code)[(*code_position)]->data == "ENDIF")
                {
                    (*code_position)++;
                    return 1;
                }else if((*code)[(*code_position)]->data == "ELSE")
                {
                    (*code_position)++;
                    IfExprAST* ifbl = (IfExprAST*)parentblock;
                    codebranch = ifbl->IsFalseCode;
                }else if((*code)[(*code_position)]->data == "ELSEIF")
                {
                    (*code_position)++;
                    std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
                    expression->reserve(150); //Max num of tokens in expression
                    if(!Parser_CollectExpression(code,code_position,expression))
                      return 0; //error, unwind recursion.
                      
                    std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
                    ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

                    (*code_position)++; //get past THEN keyword
                    std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
                    std::vector<ExprAST*>* falsecode = new std::vector<ExprAST*>;
                    ExprAST* ifbranch = new IfExprAST(truecode, falsecode, conditionbranch);

                    //Recurse to parse content of control block
                      if(!ParseCode(code,code_position,truecode, domain, IF_CONTEXT, ifbranch))
                          return 0;//Error

                    IfExprAST* ifbl = (IfExprAST*)parentblock;
                    ifbl->next_condition = (IfExprAST*)ifbranch;
					return 1;
                
                };
            };
            break;
        }



        switch((*code)[(*code_position)]->type)
        {
            case EOL_TOK:
                break;
                
                
                
			case GENERAL_STATEMENT_TOK: case FUNC_MOD_TOK: case STATEMENT_MODIFIER_TOK: //Generic statements
			{
				std::string statementn = (*code)[(*code_position)]->data;
				(*code_position)++;
				if((*code)[(*code_position)]->type == EOL_TOK)
				{//Statement with no operand
					ExprAST* sbranch = (ExprAST*) new GeneralStatementExprAST(statementn);
					codebranch->push_back(sbranch);
					(*code_position)--;
				}else{
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression
					if(!Parser_CollectExpression(code,code_position,expression))
					  return 0; //error, unwind recursion.
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* operandbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					ExprAST* sbranch = (ExprAST*) new GeneralStatementExprAST(statementn, operandbranch);
					codebranch->push_back(sbranch);
				}
			}
			break;
			case WHILE_TOK:
			{
				  (*code_position)++;
				  std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
				  expression->reserve(150); //Max num of tokens in expression
				  if(!Parser_CollectExpression(code,code_position,expression))
					return 0; //error, unwind recursion.

				  std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
				  ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

					std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
					ExprAST* ifbranch = new WhileExprAST(truecode, conditionbranch);

						//Recurse to parse content of control block
					  if(!ParseCode(code,code_position,truecode, domain, WHILE_CONTEXT, ifbranch))
						  return 0;//Error

					codebranch->push_back(ifbranch);
				break;
			}



			case DO_TOK:
			{
				  (*code_position)++;
					std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
					ExprAST* dobranch = new DoUntilExprAST(truecode);

						//Recurse to parse content of control block
					  if(!ParseCode(code,code_position,truecode, domain, DO_CONTEXT, dobranch))
						  return 0;//Error

					codebranch->push_back(dobranch);
				break;
			}




		
            //Skip any function declarations
            case FUNC_DEC_TOK:
				(*code_position)++;
				while ((*code)[(*code_position)]->type != FUNC_END_TOK)
					(*code_position)++;
				break;

            case SELECT_TOK:     //SELECT STATEMENT
            {
				(*code_position)+=3;//Get past SELECT, NEWLINE, CASE tokens
				SelectExprAST* parbranch = new SelectExprAST();
				//Parse first case statement
				std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
                expression->reserve(150); //Max num of tokens in expression
                if(!Parser_CollectExpression(code,code_position,expression))
                  return 0; //error, unwind recursion.
                  
				std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
				ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
                  
                //CASE expression collected, now recurse for rest.
                std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
				parbranch->next = new CaseExprAST(tcode, conditionbranch);
				//Recurse to parse content of control block
                if(!ParseCode(code,code_position,tcode, domain, SELECT_CONTEXT, (ExprAST*)parbranch->next))
                      return 0;//Error
                codebranch->push_back(parbranch);
			};
			break;


            case SWITCH_TOK:     //SELECT STATEMENT
            if((*code)[(*code_position)]->data == "SWITCH"){
				(*code_position)++;//Get past SWITCH
				//Parse input
				std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
                expression->reserve(150); //Max num of tokens in expression
                if(!Parser_CollectExpression(code,code_position,expression))
                  return 0; //error, unwind recursion.
                  
				std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
				ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
				SwitchExprAST* parbranch = new SwitchExprAST(conditionbranch);
				
				//Parse first CASE.
				(*code_position)+=2;
				std::vector<token*>* expression2 = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
                expression2->reserve(150); //Max num of tokens in expression
                if(!Parser_CollectExpression(code,code_position,expression2, true))
                  return 0; //error, unwind recursion.
                
				std::vector<token*> shunted2 = ShuntExpression(expression2);//Parse expression
				ExprAST* conditionbranch2 = AssembleASTBranch(&shunted2);//Build AST branch from the parsed expression.

				//If it is a primary expression, it needs to be compared with the post-switch nonterminal.
                if(!Expression_IsTo( expression2))
                {//If it is NOT a (expression) TO (expression) type switch expression.
					VariableExprAST* var = new VariableExprAST(std::string("Switch"));
					var->scope = ASSIGN_SCOPE_RESERVED;
					ExprAST* oper = new BinaryExprAST("=", var, conditionbranch2);
					conditionbranch2 = oper;
				}
                
                //CASE expression collected, now recurse for rest.
                std::vector<ExprAST*>* tcode = new std::vector<ExprAST*>;
				parbranch->next = new CaseExprAST(tcode, conditionbranch2);
				//Recurse to parse content of control block
                if(!ParseCode(code,code_position,tcode, domain, SWITCH_CONTEXT, (ExprAST*)parbranch->next))
                      return 0;//Error
                codebranch->push_back(parbranch);
			};
			break;



			case FOR_TOK:
			{
				bool inflag = false;
				ExprAST* forbranch = NULL;
				std::vector<ExprAST*>* truecode = NULL;
				
				if((*code)[(*code_position)]->data == "FOR")	//For statement - now we parse it!
				{
					(*code_position)++;//Advance past FOR token
					//Collect iterator counter expression.
					std::vector<token*>* iteratorcounter = new std::vector<token*>;
					iteratorcounter->reserve(150); //Max num of tokens in expression
					if(!Parser_CollectExpression(code,code_position,iteratorcounter, false, false))
					  return 0; //error, unwind recursion.

					if((*code)[(*code_position)]->data == "IN")
						inflag = true;
					  
					(*code_position)++;//Advance past '=' token (or even the IN token)
					
					//Collect initial value expression
					std::vector<token*>* initval_exp = new std::vector<token*>;
					initval_exp->reserve(150); //Max num of tokens in expression
					if(!Parser_CollectExpression(code,code_position,initval_exp, false, false))
					  return 0; //error, unwind recursion.
					  
					  
					if(!inflag)//only valid if it is not a FOR-IN type iterative loop.
					{
						(*code_position)++;//Advance past 'TO' token

						//Collect final value expression
						std::vector<token*>* finval_exp = new std::vector<token*>;
						finval_exp->reserve(150); //Max num of tokens in expression
						if(!Parser_CollectExpression(code,code_position,finval_exp, false, false))
						  return 0; //error, unwind recursion.
						  
						std::vector<token*>* stepval_exp = new std::vector<token*>;
						stepval_exp->reserve(150);
						if((*code)[(*code_position)]->data == "STEP")//is there a STEP statement?
						{
							(*code_position)++;
							if(!Parser_CollectExpression(code,code_position,stepval_exp, false, false))
							  return 0; //error, unwind recursion.
						}else{
							//Default: Stepvalue is 1.
							token* stepv = new token;
							stepv->type = INT32_TOK;
							stepv->data = "1";
							stepval_exp->push_back(stepv);
						}

						//Now we build a branch for each FOR expression in turn.
						std::vector<ExprAST*>* tempcd = new std::vector<ExprAST*>;
						int temp_codepos = 0;
						int cnt = 0;
						token* temp;
						std::vector<token*>* initialassign = new std::vector<token*>;
						for(;cnt<iteratorcounter->size();cnt++){temp = new token;temp->type=(*iteratorcounter)[cnt]->type;temp->data=(*iteratorcounter)[cnt]->data;initialassign->push_back(temp);};
						temp = new token;
						temp->type = EQUAL_TOK;
						temp->data = "=";
						initialassign->push_back(temp);
						for(cnt=0;cnt<initval_exp->size();cnt++){temp = new token;temp->type=(*initval_exp)[cnt]->type;temp->data=(*initval_exp)[cnt]->data;initialassign->push_back(temp);};
						temp = new token;
						temp->type = EOL_TOK;
						temp->data = "n";
						initialassign->push_back(temp);
						if(!ParseCode(initialassign,&temp_codepos,tempcd, domain, FOR_CONTEXT, NULL))
							  return 0;//Error
						ExprAST* assignnode = (*tempcd)[0];

						//create condition check node
						tempcd = new std::vector<ExprAST*>;
						temp_codepos = 0;
						cnt = 0;
						std::vector<token*>* condition = new std::vector<token*>;
						for(;cnt<iteratorcounter->size();cnt++){temp = new token;temp->type=(*iteratorcounter)[cnt]->type;temp->data=(*iteratorcounter)[cnt]->data;condition->push_back(temp);};
						temp = new token;
						temp->type = COMP_OP_TOK;
						temp->data = "<=";
						condition->push_back(temp);
						for(cnt=0;cnt<finval_exp->size();cnt++){temp = new token;temp->type=(*finval_exp)[cnt]->type;temp->data=(*finval_exp)[cnt]->data;condition->push_back(temp);};
						std::vector<token*> shunted = ShuntExpression(condition);//Parse expression
						ExprAST* conditionnode = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

						//create increment node
						tempcd = new std::vector<ExprAST*>;
						temp_codepos = 0;
						cnt = 0;
						std::vector<token*>* inc = new std::vector<token*>;
						for(;cnt<iteratorcounter->size();cnt++){temp = new token;temp->type=(*iteratorcounter)[cnt]->type;temp->data=(*iteratorcounter)[cnt]->data;inc->push_back(temp);};
						temp = new token;
						temp->type = REL_ASSIGN_TOK;
						temp->data = "+=";
						inc->push_back(temp);
						for(cnt=0;cnt<stepval_exp->size();cnt++){temp = new token;temp->type=(*stepval_exp)[cnt]->type;temp->data=(*stepval_exp)[cnt]->data;inc->push_back(temp);};
						temp = new token;
						temp->type = EOL_TOK;
						temp->data = "n";
						inc->push_back(temp);
						if(!ParseCode(inc,&temp_codepos,tempcd, domain, FOR_CONTEXT, NULL))
							  return 0;//Error
						ExprAST* incnode = (*tempcd)[0];
						
						truecode = new std::vector<ExprAST*>;
						forbranch = new ForToExprAST(truecode, conditionnode, assignnode, incnode);

					}else { //FOR-IN iterative loops
						
						//Create initial assignment expression
						std::vector<ExprAST*>* tempcd = new std::vector<ExprAST*>;
						int temp_codepos = 0;
						int cnt = 0;
						token* temp;
						std::vector<token*>* initialassign = new std::vector<token*>;
						for(;cnt<iteratorcounter->size();cnt++){temp = new token;temp->type=(*iteratorcounter)[cnt]->type;temp->data=(*iteratorcounter)[cnt]->data;initialassign->push_back(temp);};
						temp = new token;
						temp->type = EQUAL_TOK;
						temp->data = "=";
						initialassign->push_back(temp);
						for(cnt=0;cnt<initval_exp->size();cnt++){temp = new token;temp->type=(*initval_exp)[cnt]->type;temp->data=(*initval_exp)[cnt]->data;initialassign->push_back(temp);};
						temp = new token;
						temp->type = LFT_ARRAY_PARANTH_TOK;
						temp->data = "[";
						initialassign->push_back(temp);
						temp = new token;
						temp->type = INT32_TOK;
						temp->data = "0";
						initialassign->push_back(temp);
						temp = new token;
						temp->type = RHT_ARRAY_PARANTH_TOK;
						temp->data = "]";
						initialassign->push_back(temp);
						temp = new token;
						temp->type = EOL_TOK;
						temp->data = "n";
						initialassign->push_back(temp);
						if(!ParseCode(initialassign,&temp_codepos,tempcd, domain, FOR_CONTEXT, NULL))
							  return 0;//Error
						ExprAST* assignnode = (*tempcd)[0];
						
						//Create expression for what we are iterating
						tempcd = new std::vector<ExprAST*>;
						temp_codepos = 0;
						cnt = 0;
						std::vector<token*>* iterval = new std::vector<token*>;
						for(;cnt<initval_exp->size();cnt++){temp = new token;temp->type=(*initval_exp)[cnt]->type;temp->data=(*initval_exp)[cnt]->data;iterval->push_back(temp);};
						std::vector<token*> shunted = ShuntExpression(iterval);//Parse expression
						ExprAST* iternode = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.						

						//Create expression for what we are assigning to.
						tempcd = new std::vector<ExprAST*>;
						temp_codepos = 0;
						cnt = 0;
						std::vector<token*>* assident = new std::vector<token*>;
						for(;cnt<iteratorcounter->size();cnt++){temp = new token;temp->type=(*iteratorcounter)[cnt]->type;temp->data=(*iteratorcounter)[cnt]->data;assident->push_back(temp);};
						shunted = ShuntExpression(assident);//Parse expression
						ExprAST* assnode = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
						
						truecode = new std::vector<ExprAST*>;
						forbranch = new ForInExprAST(truecode, assignnode, assnode, iternode);
						
					}

					
					if(!ParseCode(code,code_position,truecode, domain, FOR_CONTEXT, forbranch))
                      return 0;//Error
                      
                    codebranch->push_back(forbranch);
				}
			}
			break;

			case CASE_TOK:		//CASE STATEMENT
			{
				if((*code)[(*code_position)]->data == "CONTINUECASE")
				{//Apply the continue case operation to a CaseExprAST node. (Parent)
					CaseExprAST* par = (CaseExprAST*)parentblock;
					par->ContinueCase = true;
				}
			};
			break;

            case IF_TOK:        //IF STATEMENT
            { //collect conditional expression.
              (*code_position)++;
              std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
              expression->reserve(150); //Max num of tokens in expression
              if(!Parser_CollectExpression(code,code_position,expression))
                return 0; //error, unwind recursion.

              std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
              ExprAST* conditionbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

              (*code_position)++; //get past THEN keyword
              if((*code)[(*code_position)]->type == EOL_TOK) //Multiline IF statement.
              {
                std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
                std::vector<ExprAST*>* falsecode = new std::vector<ExprAST*>;
                ExprAST* ifbranch = new IfExprAST(truecode, falsecode, conditionbranch);

                    //Recurse to parse content of control block
                  if(!ParseCode(code,code_position,truecode, domain, IF_CONTEXT, ifbranch))
                      return 0;//Error

                codebranch->push_back(ifbranch);


              } else {    //Single line IF statement
                switch((*code)[(*code_position)]->type)
                {
					case GENERAL_STATEMENT_TOK: case FUNC_MOD_TOK: case STATEMENT_MODIFIER_TOK://Statement - so we actually have to recurse
						{
							std::vector<token*>* expr = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
							expr->reserve(150); //Max num of tokens in expression
							if(!Parser_CollectExpression(code,code_position,expr))
							  return 0; //error, unwind recursion.
							token* ref = new token; //All code passed to parser must be terminated by EOL token.
							ref->type = EOL_TOK;
							ref->data = "";
							expr->push_back(ref);
							//things are complicated because we must treat the rest of the line as a statement, not an expression.
							//therefore, we recurse the Parser, building a new context as we go.
							std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
							std::vector<ExprAST*>* falsecode = new std::vector<ExprAST*>;
							int code_position2 = 0;
							if(!ParseCode(expr,&code_position2,truecode, "", '\0'))
								  return 0;//Error

							ExprAST* ifbranch = new IfExprAST(truecode, falsecode, conditionbranch);

							codebranch->push_back(ifbranch);
						}
						break;
					default: //For just an expression, dont recurse, just parse manually.
						{
							  std::vector<token*>* expr = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
							  expr->reserve(150); //Max num of tokens in expression
							  if(!Parser_CollectExpression(code,code_position,expr))
								return 0; //error, unwind recursion.

							  std::vector<token*> shunt = ShuntExpression(expr);//Parse expression
							  ExprAST* exprb = AssembleASTBranch(&shunt);//Build AST branch from the parsed expression.
							std::vector<ExprAST*>* truecode = new std::vector<ExprAST*>;
							std::vector<ExprAST*>* falsecode = new std::vector<ExprAST*>;
							truecode->push_back(exprb);

							ExprAST* ifbranch = new IfExprAST(truecode, falsecode, conditionbranch);

							codebranch->push_back(ifbranch);
						}
						break;
				}
              }
            }    
            


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
                
            case VARIANT_VARIABLE_TOK: case ARRAY_VARIABLE_TOK:
                if (((*code)[(*code_position)+1]->type == EQUAL_TOK) || ((*code)[(*code_position)+1]->type == REL_ASSIGN_TOK))    //Assignment or relative assignment
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
                    ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, ASSIGN_SCOPE_AMBIGUOUS, domain, op);    //Create the assignment node.
                    codebranch->push_back(assignbranch);    //Add the assignment to the collection (node) of this recursion.
                }else if (((*code)[(*code_position)+1]->type == OOP_DOT_TOK) || ((*code)[(*code_position)+1]->type == LFT_ARRAY_PARANTH_TOK))
				{//Either an assignment, or function call. EG: $var.foo = 4 or $var.foo()				
					
					//Collect expression up to the EQUAL sign.
					std::vector<token*>* expression = new std::vector<token*>;
					expression->reserve(150);

					if(!Parser_CollectExpression(code,code_position,expression, false, false))
						return 0; //error, unwind recursion.
											
					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* assigntobranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					
						
					if ((*code)[(*code_position)]->type == EQUAL_TOK)//Assignment.
					{
						(*code_position)++;
						std::vector<token*>* expression2 = new std::vector<token*>; //collect the initialisation data.
						expression2->reserve(150);
						if(!Parser_CollectExpression(code,code_position,expression2))
							return 0; //error, unwind recursion.

						shunted = ShuntExpression(expression2);
						ExprAST* initbranch = AssembleASTBranch(&shunted);

						//Make assignment node.
						CommonAssignExprAST* nod = new CommonAssignExprAST(assigntobranch, initbranch);
						codebranch->push_back(nod); 
						
					}else{//Dot Operator function call.
						
						//Make function call node based on the already collected expression.
						
					}
						
				}
				
/*
				else if ((*code)[(*code_position)+1]->type == LFT_ARRAY_PARANTH_TOK)
				{//Array Assignment
					std::string varname = (*code)[(*code_position)]->data;
					(*code_position)++;

					ArrayAccess* As = new ArrayAccess;
					Parser_CollectSubscripts(code, code_position, As);
			
					std::string op = (*code)[(*code_position)]->data;
					(*code_position)++;

					//Collect expression
					std::vector<token*>* expression = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
					expression->reserve(150); //Max num of tokens in expression

					if(!Parser_CollectExpression(code,code_position,expression))
						return 0; //error, unwind recursion.

					std::vector<token*> shunted = ShuntExpression(expression);//Parse expression
					ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
					ArrayAssignExprAST* nodeass = new ArrayAssignExprAST(varname, As, exprbranch, op);
					codebranch->push_back(nodeass); 
					
				}
*/
		    
		    
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
    if((*code)[(*code_position)]->type == ENUM_KEYWORD_TOK)        //Shit just got real
    {
        (*code_position)++;
        is_enum = true;
        if((*code)[(*code_position)]->type == FOR_TOK)//STEP keyword
        {    //User defined step?
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
            ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, scope, domain, "=");    //Create the assignment node.
            codebranch->push_back(assignbranch);    //Add the assignment to the collection (node) of this recursion.
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
            {                //zero initialised
                token* tok = new token;
                tok->type = STRINGLITERAL_TOK;
                tok->data = "";
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
            ExprAST* assignbranch = new AssignExprAST(varname, exprbranch, scope, domain, "=");    //Create the assignment node.
            codebranch->push_back(assignbranch);    //Add the assignment to the collection (node) of this recursion.
            last_var = varname;
	  }else{ //Arrays with default initialisation!

            std::string varname = (*code)[(*code_position)]->data;
            (*code_position)++; //Get past two tokens
		ArrayAccess* As = new ArrayAccess;
		Parser_CollectSubscripts(code, code_position, As);

		ExprAST* assignbranch = new ArrayDecExprAST( varname, As, scope, domain);
		codebranch->push_back(assignbranch);
		
		//POTENTIAL ERROR! For enums with arrays .. dont know the actual behaviour
		last_var = "";

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

int Parser_CollectExpression(std::vector<token*>* code, int* code_position, std::vector<token*>* expression, bool allow_to_operator, bool allow_ll_equal_operator)
{
    long int nestedlevel = 0;
    bool breakflag = false;
    
    while(true)
    {

        switch ((*code)[(*code_position)]->type)
        {
            case VARIANT_VARIABLE_TOK: case ARRAY_VARIABLE_TOK: case MACRO_TOK:
            case COMP_OP_TOK: case MATH_OP_TOK: case DOUBLE_EQUAL_TOK:
            case NUMHEX_TOK: case INT32_TOK: case INT64_TOK: case DOUBLE_TOK: case STRINGLITERAL_TOK:
            case FUNC_UNKNOWN_TOK: case FUNC_BUILTIN_TOK: case OOP_DOT_TOK: case UNKNOWN_TOK:
            //Recently added
            case GENERAL_STATEMENT_TOK: case FUNC_MOD_TOK: case STATEMENT_MODIFIER_TOK://Should ONLY fire when singleline IF - otherwise ERROR
                expression->push_back((*code)[(*code_position)]);
                break;
                
             case EQUAL_TOK:
                if ((nestedlevel<1) && (!allow_ll_equal_operator))
					breakflag=true;
				else
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

			case FOR_TOK:
				if(((*code)[(*code_position)]->data == "TO") && allow_to_operator)
				{
					expression->push_back((*code)[(*code_position)]);
				}else
					breakflag = true;
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
        {    //Out of bounds error
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





int Parser_CollectSubscripts(std::vector<token*>* code, int* code_position, ArrayAccess* arraystruct)
{
	arraystruct->num_dimensions = 0;
	arraystruct->subscript_expressions.reserve(64);

    while((*code)[(*code_position)]->type == LFT_ARRAY_PARANTH_TOK)
    {
		arraystruct->num_dimensions++;
		(*code_position)++;

        std::vector<token*>* expr = new std::vector<token*>; //(these things are on heap to prevent stack overflow)
        expr->reserve(150); //Max num of tokens in expression
        if(!Parser_CollectExpression(code,code_position,expr))
          return 0; //error, unwind recursion.
        std::vector<token*> shunted = ShuntExpression(expr);//Parse expression
        ExprAST* sub_expr = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.

		arraystruct->subscript_expressions.push_back(sub_expr);
		(*code_position)++;

		if((*code_position)>= code->size())
			break;

    }

	//(*code_position)--;
	return 1;
}
