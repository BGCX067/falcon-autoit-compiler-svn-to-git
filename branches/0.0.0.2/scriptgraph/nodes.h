/* Declaration of C++ Classes for the nodes on the AST. */

#define AST_TYPE_UNDEF 0
#define AST_TYPE_INT 1
#define AST_TYPE_NUM 2
#define AST_TYPE_STR 3
#define AST_TYPE_VAR 4
#define AST_TYPE_COLLECTION 8

#define ASSIGN_SCOPE_AMBIGUOUS 0
#define ASSIGN_SCOPE_GLOBAL 1
#define ASSIGN_SCOPE_LOCAL 2

struct SymtableEntry{
	int scope;
	int discovered_depth;
	std::string varname;
	std::string domain;
};

struct Symtable{
	std::vector<SymtableEntry*> entries;
};
bool SymbolTable_Exists(Symtable* Sym, std::string name, std::string domain);//defined in scanner_walk.cpp


/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth) {return std::string("");};
  virtual void DebugPrint(int level){return;};
  virtual int GetType(void){return AST_TYPE_UNDEF;};
  virtual void* GetCollection(void){return NULL;};
  virtual void SymboltableBuild(int level, Symtable*){;};
};




/* Primitives */
/// NumberExprAST - Expression class for numeric decimals.
class NumberExprAST : public ExprAST {
  double Val;
public:
  NumberExprAST(double val) : Val(val) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_NUM;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// IntegerExprAST - Expression class for numeric integers.
class IntegerExprAST : public ExprAST {
  long long Val;
public:
  IntegerExprAST(long long val) : Val(val) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_INT;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// StringExprAST - Expression class for numeric integers.
class StringExprAST : public ExprAST {
  char* Val;
public:
  StringExprAST(char* val) : Val(val) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_STR;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;
public:
  VariableExprAST(const std::string &name) : Name(name) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_VAR;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};






/* Non-Expression Operations */
/// AssignExprAST - Class for assignments
/// Remember that a scopeName of "" means GLOBAL SCOPE. The Scope attribute is pretty much unused.
class AssignExprAST : public ExprAST {
  std::string Name;
  ExprAST* EXPR;
  int scope;
  std::string ScopeName;
public:
  AssignExprAST(const std::string &name,  ExprAST *expr, int Scope=ASSIGN_SCOPE_AMBIGUOUS, std::string scopeName="") : Name(name), EXPR(expr),
  scope(Scope), ScopeName(scopeName){}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
public:
  CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
    : Callee(callee), Args(args) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
};



/* Expression operations */
/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
  std::string Op;
  ExprAST *LHS, *RHS;
public:
  BinaryExprAST(std::string op, ExprAST *lhs, ExprAST *rhs) 
    : Op(op), LHS(lhs), RHS(rhs) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// UnaryExprAST - Expression class for a unnary operator.
class UnaryExprAST : public ExprAST {
  std::string Op;
  ExprAST *LHS;
public:
  UnaryExprAST(std::string op, ExprAST *lhs) 
    : Op(op), LHS(lhs) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_NUM;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};






/* Code Collection Nodes */

/// CollectionExprAST - Expression class for multiple branches.
class CollectionExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* Code;
  CollectionExprAST(std::vector<ExprAST*>* code) : Code(code) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->Code;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

