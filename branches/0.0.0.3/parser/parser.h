


int ParseCode(std::vector<token*>* code, int* code_position, CollectionExprAST** ASTParent, std::string domain, char context);
int Parser_CollectExpression(std::vector<token*>* code, int* code_position, std::vector<token*>* expression);
int Parser_HandleVarDef(std::vector<token*>* code, int* code_position, std::vector<ExprAST*>* codebranch, std::string domain);
void PrintExpression(std::vector<token*>* expression);

std::vector<token*> ShuntExpression(std::vector<token*>* exp);
ExprAST* AssembleASTBranch(std::vector<token*>* RPN);


void PrintShunter(std::vector<token*>* expression);
