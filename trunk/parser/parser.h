

//Parser.cpp
int ParseCode(std::vector<token*>* code, int* code_position, std::vector<ExprAST*>* codebranch, std::string domain, char context, ExprAST* parentblock=NULL);
int Parser_CollectExpression(std::vector<token*>* code, int* code_position, std::vector<token*>* expression, bool allow_to_operator=false, bool allow_ll_equal_operator = true);
int Parser_HandleVarDef(std::vector<token*>* code, int* code_position, std::vector<ExprAST*>* codebranch, std::string domain);
int Parser_CollectSubscripts(std::vector<token*>* code, int* code_position, ArrayAccess* arraystruct);

//Debug func
void PrintExpression(std::vector<token*>* expression);
void PrintShunter(std::vector<token*>* expression);

//Parser_expr.cpp
std::vector<token*> ShuntExpression(std::vector<token*>* exp);
ExprAST* AssembleASTBranch(std::vector<token*>* RPN);
bool Expression_IsTo(std::vector<token*>* expression);

//Tokens_disambiguate.cpp
int Tokens_Disambiguate_Pass(std::vector<token*>* tokenvect);


//Contexts for the recursive decent parser.
#define IF_CONTEXT 'I'
#define WHILE_CONTEXT 'W'
#define SELECT_CONTEXT 'S'
#define FOR_CONTEXT 'R'
#define SWITCH_CONTEXT 'Q'
#define USERFUNC_CONTEXT 'F'
#define DO_CONTEXT 'D'

