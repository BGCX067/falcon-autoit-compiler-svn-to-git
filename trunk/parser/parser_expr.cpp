#include "../stdafx.h"
#include "../tokens.h"
#include "../scriptgraph/nodes.h"
#include "parser.h"



#define OPERATORPREC_COUNT 36 //Increment this by two for every new entry to the op table.
char OperatorsPrec[][2] =       {                               {'A'},{(char)1},        //AND
                                                                {'O'},{(char)1},        //OR
                                                                {'='},{(char)2},
                                                                {'D'},{(char)2},
                                                                {'<'},{(char)2},
                                                                {'>'},{(char)2},
                                                                {'L'},{(char)2},        //<=
                                                                {'G'},{(char)2},        //>=
                                                                {'T'},{(char)2},        //<>
                                                                {'R'},{(char)2},		//TO
                                                                {'&'},{(char)3},
                                                                {'+'},{(char)4},
                                                                {'-'},{(char)4},
                                                                {'*'},{(char)5},
                                                                {'/'},{(char)5},
                                                                {'^'},{(char)6},
                                                                {'U'},{(char)7},		//Unary minus
                                                                {'!'},{(char)7}         //NOT
                                                        };
                                                        
                                                        
//Returns 0 if not found.
int GetPrecedence(char c){
        int iterator = 0;
        for(; iterator<OPERATORPREC_COUNT;iterator+=2){
                if(OperatorsPrec[iterator][0] == c)return (int)OperatorsPrec[iterator+1][0];
        }
        return 0;
}

char Operator(std::string in){

        if(in=="AND")return 'A'; if(in=="OR")return 'O';
        if(in=="+")return '+'; if(in=="-")return '-'; if(in=="/")return '/'; if(in=="*")return '*';
        if(in=="&")return '&';
        if(in=="<")return '<'; if(in==">")return '>'; if(in=="=")return '='; if(in=="<=")return 'L'; if(in==">=")return 'G';
        if(in=="^")
                        return '^';
        if(in=="!")
                        return '!';
        if(in=="==")
                        return 'D';
        if(in=="<>")
                        return 'T';
        if(in=="--")
                        return 'U';
        if(in=="NOT")
                        return '!';
        if(in=="TO")
                        return 'R';

        return (char)0;
}

bool op_left_assoc(const char c)
{
    switch(c)    {
        // left to right
                case '*': case '/': case '%': case '+': case '-': case '&': case '^':
            return true;
        // right to left
                case '=': case '!': case 'A': case 'O': case 'U':
            return false;
    }
    return false;
}


int HexToDec(const char *szHex, int &nDec)
{
        szHex+=2; //Not allowed 0x part of 0xffff
        // Really crappy hex conversion
        int i = (int)strlen(szHex) - 1;

        nDec = 0;
        int nMult = 1;
        for (int j = 0; j < 8; ++j)
        {
                if (i < 0)
                        break;

                if (szHex[i] >= '0' && szHex[i] <= '9')
                        nDec += (szHex[i] - '0') * nMult;
                else if (szHex[i] >= 'A' && szHex[i] <= 'F')
                        nDec += (((szHex[i] - 'A'))+10) * nMult;
                else if (szHex[i] >= 'a' && szHex[i] <= 'f')
                        nDec += (((szHex[i] - 'a'))+10) * nMult;
                else
                {
                        nDec = 0;                                       // Set value as 0
                        return false;
                }

                --i;
                nMult = nMult * 16;
        }

        if (i != -1)
        {
                nDec = 0;
                return false;
        }
        else
                return true;
        
}





std::vector<token*> ShuntExpression(std::vector<token*>* exp)
{
    std::vector<token*> RPNExpression;
    RPNExpression.reserve(150);
    std::vector<token*> RPNstack;
    RPNstack.reserve(150);
    std::vector<token*> OPstack;
    OPstack.reserve(150);
    token LastToken;
    int iterator = 0;
    int lasttoktype = 0;
    
    for(;iterator<exp->size();iterator++)
    {
        token* Currenttoken = (*exp)[iterator];
        
        switch((*exp)[iterator]->type)
        {    //Convert all tokens to their AST node equivalents.
            case NUMHEX_TOK:
                {    int temp = 0;
                    HexToDec( Currenttoken->data.c_str(), temp);
			  if (temp < 0) //Avoiding ambiguity issue in compilation.
			  {
				char* temp3 = new char[255];
            		sprintf(temp3, "%i", temp);
                  	StringExprAST* node = new StringExprAST(temp3);
                  	(*exp)[iterator]->ASTnode = node;
                  	(*exp)[iterator]->type = AST_REF_TOKEN;
			  }
			  else{
                    	IntegerExprAST* node = new IntegerExprAST(temp);
                    	(*exp)[iterator]->ASTnode = node;
                    	(*exp)[iterator]->type = AST_REF_TOKEN;
			  }
                }
                RPNstack.push_back((*exp)[iterator]);
                  break;

             case MACRO_TOK:
                {
                    MacroExprAST* node = new MacroExprAST((*exp)[iterator]->data);
                    (*exp)[iterator]->ASTnode = node;
                    (*exp)[iterator]->type = AST_REF_TOKEN;
                }
                RPNstack.push_back((*exp)[iterator]);
                  break;

             case INT32_TOK:
               {
                  IntegerExprAST* node = new IntegerExprAST(atoi( Currenttoken->data.c_str()));
                  (*exp)[iterator]->ASTnode = node;
                  (*exp)[iterator]->type = AST_REF_TOKEN;
               }
              RPNstack.push_back((*exp)[iterator]);
               break;
             case INT64_TOK:
               {
#ifdef _MSC_VER
                  IntegerExprAST* node = new IntegerExprAST(_atoi64( Currenttoken->data.c_str()));
#else
                  IntegerExprAST* node = new IntegerExprAST(atoll( Currenttoken->data.c_str()));
#endif
                  (*exp)[iterator]->ASTnode = node;
                  (*exp)[iterator]->type = AST_REF_TOKEN;
               }
              RPNstack.push_back((*exp)[iterator]);
               break;
            case DOUBLE_TOK:
              {
                  NumberExprAST* node = new NumberExprAST(atof( Currenttoken->data.c_str()));
                  (*exp)[iterator]->ASTnode = node;
                  (*exp)[iterator]->type = AST_REF_TOKEN;
              }
              RPNstack.push_back((*exp)[iterator]);
              break;
            case STRINGLITERAL_TOK:
              {
                  char* temp = new char[strlen(Currenttoken->data.c_str())+1];
                  strcpy(temp, Currenttoken->data.c_str());
                  StringExprAST* node = new StringExprAST(temp);
                  (*exp)[iterator]->ASTnode = node;
                  (*exp)[iterator]->type = AST_REF_TOKEN;
              }
              RPNstack.push_back((*exp)[iterator]);
              break;
              
			case VARIANT_VARIABLE_TOK: case ARRAY_VARIABLE_TOK:
              {
                  VariableExprAST* node = new VariableExprAST(Currenttoken->data);
                  (*exp)[iterator]->ASTnode = node;
                  (*exp)[iterator]->type = AST_REF_TOKEN;
              }
              RPNstack.push_back((*exp)[iterator]);
              break;
         
			case OOP_DOT_TOK:
         
				{
					ExprAST* temp = (ExprAST*)RPNstack.back()->ASTnode;
					RPNstack.pop_back();
		
					ExprAST* temp2 = new DotOperatorAccessExprAST((*exp)[iterator]->data, temp);
				
					token* temp3 = new token;
					temp3->type = AST_REF_TOKEN;
					temp3->ASTnode = temp2;
					
					RPNstack.push_back(temp3);
				}
				break;


			case LFT_ARRAY_PARANTH_TOK:
				{
					ArrayAccess* As = new ArrayAccess;
					Parser_CollectSubscripts(exp, &iterator, As);
					ExprAST* temp = (ExprAST*)RPNstack.back()->ASTnode;
					RPNstack.pop_back();
					ExprAST* temp2 = new ArrayAccessExprAST(As, temp);
                  		(*exp)[iterator-1]->ASTnode = temp2;
                  		(*exp)[iterator-1]->type = AST_REF_TOKEN;
					RPNstack.push_back((*exp)[iterator-1]);
					iterator--;
				}
				break;


            case UNKNOWN_TOK:
                if((*exp)[iterator+1]->type == LFT_BRACKET_TOK)//Function call
                {
                    std::string funcname = (*exp)[iterator]->data;
                    std::vector<ExprAST*>* Args = new std::vector<ExprAST*>;
                    int pos2 = iterator;
                    iterator+=2;
                    
                    while((*exp)[iterator]->type != RHT_BRACKET_TOK)
                    {
                        std::vector<token*>* expr = new std::vector<token*>;
                        expr->reserve(150);
                        Parser_CollectExpression(exp, &iterator, expr);
                        if(expr->size() > 0)
                        {
                            std::vector<token*> shunted = ShuntExpression(expr);//Parse expression
                            ExprAST* exprbranch = AssembleASTBranch(&shunted);//Build AST branch from the parsed expression.
				    Args->push_back(exprbranch);
                        }else
					std::cout << "ERR: Parser_expr::func parse - invalid internal control flow.\n";
                        if((*exp)[iterator]->type == PARAM_SEPARATOR_TOK)
                            iterator++;
                    }
                    
                    CallExprAST* node = new CallExprAST(funcname, Args);
                    (*exp)[pos2]->ASTnode = node;
                    (*exp)[pos2]->type = AST_REF_TOKEN;
                    RPNstack.push_back((*exp)[pos2]);
                }else{
                    std::cout << "Error: Unknown token when parsing expression!\n";
                }
                break;
         
            //Handle all the brackets.
            case LFT_BRACKET_TOK:
                    OPstack.push_back((*exp)[iterator]);
                    break;
            case RHT_BRACKET_TOK:
                    while( (OPstack.size()>1) && (OPstack.back()->type!=LFT_BRACKET_TOK) )
                    {
                            token* temp = OPstack.back();
                            OPstack.pop_back();
                            RPNstack.push_back(temp);
                            //TODO: Errorcheck here to see if OPstack empty: no match left bracket.
                    };

                    //TODO: Errorcheck here to see if last token is '(': should be.
                    OPstack.pop_back(); //Get rid of that left bracket.
                    break;
       
            //Handle all the operators.
            case MATH_OP_TOK: case EQUAL_TOK: case COMP_OP_TOK: case DOUBLE_EQUAL_TOK: case FOR_TOK:

                    while(OPstack.size()){
                            char opcode;
                            int prec;
                            if((Currenttoken->data == "-") &&(
							   (lasttoktype == MATH_OP_TOK) ||
							   (lasttoktype == EQUAL_TOK) ||
							   (lasttoktype == COMP_OP_TOK) ||
							   (lasttoktype == DOUBLE_EQUAL_TOK))) //Detect and set (higher) precedence for unary minus case.
                            {
								Currenttoken->data = "--";
								(*exp)[iterator]->data = "--";
								//std::cout << "UNARY DETECTED!\n";//debug bugfix #1
							}
							opcode = Operator(Currenttoken->data);
							prec = GetPrecedence(opcode);
							
                            int stackopPrec = GetPrecedence(Operator(OPstack.back()->data));
                            if((Operator(OPstack.back()->data)) && 
                                    ((op_left_assoc(opcode)) && (prec <= stackopPrec)) ||
                                    (!op_left_assoc(opcode) && (prec < stackopPrec)))
                                    {
                                            RPNstack.push_back(OPstack.back());
                                            OPstack.pop_back();
                            }else{ break;}
                    }
                    OPstack.push_back((*exp)[iterator]);
                    break;


            //Other tokens should not be in here, but this will trap them if they are.
            default:
                    std::cout << "Warning: Unhandled token sent through expression shunter - type: " << (*exp)[iterator]->type << "\n";
                    break;
       
       
        }
        lasttoktype = Currenttoken->type;
    }
    //Dump operator stack onto the RPN stack.
    while(OPstack.size()){
            RPNstack.push_back(OPstack.back());
            OPstack.pop_back();          
    }
    //Now we need to reverse the stack in order to pop off the tokens in the right order
    //for the vector (RPNExpression) sake.
    while(RPNstack.size()){
            OPstack.push_back(RPNstack.back()); //We are reversing RPNstack -> OPstack (which was empty)
            RPNstack.pop_back();         
    }
    //Save everything to our expression output.
    while(OPstack.size()){
            RPNExpression.push_back(OPstack.back());
            OPstack.pop_back();          
    }
    return RPNExpression;
}




token* Duplicate_Token(token* tok)
{
    token* rettok = new token;
    rettok->type = tok->type;
    rettok->data = tok->data;
    rettok->ASTnode = tok->ASTnode;
    return rettok;
}





//Given a sequence of tokens, consisting of type AST_REF_TOKEN and operators,
//It will generate a branch of the AST, illustrative of those operators.
ExprAST* AssembleASTBranch(std::vector<token*>* RPN)
{
    std::vector<token*> RPNExpression;
    RPNExpression.reserve(150);
    ExprAST* head = NULL;
    
    ExprAST* operand1;
    ExprAST* operand2;
    token* temp;
        
    int i=0;
    for(;i<RPN->size();i++)
    {
        token* CurrentToken = (*RPN)[i];
        
        switch(CurrentToken->type)
        {
            case AST_REF_TOKEN:
                RPNExpression.push_back(CurrentToken);
                break;
               
                
            case MATH_OP_TOK: case EQUAL_TOK: case COMP_OP_TOK: case DOUBLE_EQUAL_TOK: case FOR_TOK:
                operand1 = (ExprAST*)RPNExpression.back()->ASTnode;
                RPNExpression.pop_back();

                if((CurrentToken->data == "--") && (CurrentToken->type == MATH_OP_TOK))	//Special case - UNARY MINUS (its a bitch)
				{
                    temp = new token;
                    temp->type = AST_REF_TOKEN;
                    temp->ASTnode = new UnaryExprAST("-", operand1);
				}
				else if((RPNExpression.size()>0) && (CurrentToken->data != "NOT"))		//Unary NOT (its also a bitch)
                {
                    operand2 = (ExprAST*)RPNExpression.back()->ASTnode;
                    RPNExpression.pop_back();
                    temp = new token;
                    temp->type = AST_REF_TOKEN;
                    temp->ASTnode = new BinaryExprAST(CurrentToken->data, operand2, operand1);
                }
                else if((RPNExpression.size()>0) && (CurrentToken->type == EQUAL_TOK))
                {
                    operand2 = (ExprAST*)RPNExpression.back()->ASTnode;
                    RPNExpression.pop_back();
                    temp = new token;
                    temp->type = AST_REF_TOKEN;
                    temp->ASTnode = new BinaryExprAST(CurrentToken->data, operand2, operand1);
				}else {
                    temp = new token;
                    temp->type = AST_REF_TOKEN;
                    temp->ASTnode = new UnaryExprAST(CurrentToken->data, operand1);
                }
                head = (ExprAST*)temp->ASTnode;
                RPNExpression.push_back(temp);
                break;
                
            default:
                std::cout << "Error! Unhandled token when building AST!!!\n";
        }
    }    
    
    if(head == NULL)//No operation, just literal
    {
        ExprAST* literal = (ExprAST*)RPNExpression.back()->ASTnode;
        head = literal;
    }
    return head;
}




//Returns true if the expression has the TO operator in it.
bool Expression_IsTo(std::vector<token*>* expression)
{
    int i = 0;
    for(;i<expression->size();i++)
    {
        if((*expression)[i]->type == FOR_TOK)
			if((*expression)[i]->data == "TO")
				return true;
	}
	return false;
}

//Debug function. Prints an RPN vector.
void PrintShunter(std::vector<token*>* expression)
{
    int i = 0;
    for(;i<expression->size();i++)
    {
        if((*expression)[i]->type == AST_REF_TOKEN)
        {
            std::cout << "Literal ";
        }
        else if((*expression)[i]->type == MATH_OP_TOK)
        {
            std::cout << (*expression)[i]->data << ' ';
        }else{
            std::cout << (*expression)[i]->type << ' ';
        }
    }
    std::cout << '\n';
}

