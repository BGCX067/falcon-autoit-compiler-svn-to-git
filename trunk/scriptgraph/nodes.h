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
#define ASSIGN_SCOPE_RESERVED 3

struct SymtableEntry{
    int scope;
    int discovered_depth;
    std::string varname;
    std::string domain;
};

struct Symtable{
    std::vector<SymtableEntry*> entries;
    std::string primarydomain;
};

//Available in scanner_walk.cpp
extern bool SymbolTable_Exists(Symtable* Sym, std::string name, std::string domain);//defined in scanner_walk.cpp
extern Symtable* Symboltable_Duplicate(Symtable* Sym);
extern void Symboltable_ClearLocalEntries(Symtable* Sym);
extern void Symboltable_BackPropergateNewSymbols(Symtable* Primary, Symtable* Subset);

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


/// MacroExprAST - Expression class for referencing a macro, like CRLF
class MacroExprAST : public ExprAST {
  std::string name;
public:
    MacroExprAST(std::string val) : name(val) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_VAR;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;
public:
  int scope;
  VariableExprAST(const std::string &name) : Name(name), scope(ASSIGN_SCOPE_AMBIGUOUS) {}
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
  std::string op;
  ExprAST* EXPR;
  int scope;
  std::string ScopeName;
public:
  AssignExprAST(const std::string &name,  ExprAST *expr, int Scope=ASSIGN_SCOPE_AMBIGUOUS, std::string scopeName="", std::string Op="=") : Name(name), EXPR(expr),
  scope(Scope), ScopeName(scopeName), op(Op){}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*>* Args;
public:
  CallExprAST(std::string callee, std::vector<ExprAST*>* args)
    : Callee(callee), Args(args) {}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
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

/// DotOperatorAccessExprAST - For resolving the DOT (.) Operator.
class DotOperatorAccessExprAST : public ExprAST {
public:
  ExprAST* Wrapref;
  std::string name;

  DotOperatorAccessExprAST(std::string namein,  ExprAST* wrapref) : name(namein), Wrapref(wrapref) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// CommonAssignExprAST - For assigning to an attribute: EG $test.op = 4
class CommonAssignExprAST : public ExprAST {
public:
  ExprAST* Initialiser;
  ExprAST* Assignto;

  CommonAssignExprAST(  ExprAST* assignto, ExprAST* initialiser) : Assignto(assignto), Initialiser(initialiser) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
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

/// IfExprAST - Expression class for IF statements.
class IfExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* IsTrueCode;
  std::vector<ExprAST*>* IsFalseCode;
  IfExprAST* next_condition;
  ExprAST *condition;

  IfExprAST(std::vector<ExprAST*>* truecode, std::vector<ExprAST*>* falsecode, ExprAST *cond, IfExprAST* next=NULL) : 
  IsTrueCode(truecode), IsFalseCode(falsecode), condition(cond), next_condition(next) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->IsTrueCode;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};



/// DoUntilExprAST - Expression class for DO-UNTIL loops.
class DoUntilExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* IsTrueCode;
  ExprAST *condition;

  DoUntilExprAST(std::vector<ExprAST*>* truecode, ExprAST *cond=NULL) : 
  IsTrueCode(truecode), condition(cond) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->IsTrueCode;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};




/// WhileExprAST - Expression class for WHILE loops.
class WhileExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* IsTrueCode;
  ExprAST *condition;

  WhileExprAST(std::vector<ExprAST*>* truecode, ExprAST *cond) : 
  IsTrueCode(truecode), condition(cond) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->IsTrueCode;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};


/// ForToExprAST - Expression class for counting for loops.
class ForToExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* IsTrueCode;
  ExprAST *condition;
  ExprAST *initialassignnode;
  ExprAST *incrementalassignnode;

  ForToExprAST(std::vector<ExprAST*>* truecode, ExprAST *cond, ExprAST *initialassign, ExprAST *incrementalassign) : 
  IsTrueCode(truecode), condition(cond), initialassignnode(initialassign), incrementalassignnode(incrementalassign) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->IsTrueCode;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};


/// ForInExprAST - Expression class for iterative for loops
class ForInExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* IsTrueCode;
  ExprAST *initialassignnode;
  ExprAST *actual_assignnode;
  ExprAST *iterativenode;

  ForInExprAST(std::vector<ExprAST*>* truecode, ExprAST *ainitialassign, ExprAST *initialassign, ExprAST *iterativeassign) : 
  IsTrueCode(truecode), actual_assignnode(ainitialassign), initialassignnode(initialassign), iterativenode(iterativeassign) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->IsTrueCode;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};




/// CaseExprAST - Generic Subclass for chaining the contents of a CASE block.
class CaseExprAST : public ExprAST {
public:
  std::vector<ExprAST*>* Code;
  ExprAST *condition;
  CaseExprAST* next;
  bool ContinueCase;

  CaseExprAST(std::vector<ExprAST*>* code, ExprAST *cond) : Code(code), condition(cond), next(NULL), ContinueCase(false) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  std::string CaseCodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return this->Code;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};


/// SelectExprAST - Expression class for SELECT blocks.
class SelectExprAST : public ExprAST {
public:
  CaseExprAST* next;

  SelectExprAST(void) : next(NULL) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return NULL;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};


/// SwitchExprAST - Expression class for SWITCH blocks.
class SwitchExprAST : public ExprAST {
public:
  CaseExprAST* next;
  ExprAST* switchcond;

  SwitchExprAST(ExprAST* condition) : switchcond(condition), next(NULL) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return NULL;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};



struct ArrayAccess{
	int num_dimensions;
    std::vector<ExprAST*> subscript_expressions;
};

/// ArrayAccessExprAST - Expression class for a reference to an array, given an expression.
class ArrayAccessExprAST : public ExprAST {
public:
  ArrayAccess* Access;
  ExprAST* Wrapref;

  ArrayAccessExprAST(ArrayAccess* access, ExprAST* wrapref) : 
  Access(access), Wrapref(wrapref) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return NULL;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// ArrayDecExprAST - Expression class for an array declaration
class ArrayDecExprAST : public ExprAST {
  std::string Name;
  ArrayAccess* Access;
  int scope;
  std::string ScopeName;
public:
  ArrayDecExprAST(const std::string &name, ArrayAccess* access, int Scope=ASSIGN_SCOPE_AMBIGUOUS, std::string scopeName="") : Name(name),
  scope(Scope), ScopeName(scopeName), Access(access){}
  std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_UNDEF;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};



/// ArrayAssignExprAST - Expression class for an array assignment
class 	ArrayAssignExprAST : public ExprAST {
public:
  ArrayAccess* Access;
  std::string Name;
  std::string Op;
  int scope;
  ExprAST* Assign;

  ArrayAssignExprAST(std::string name, ArrayAccess* access, ExprAST* assign, std::string op="=") : 
  Access(access), Assign(assign), Name(name), Op(op), scope(ASSIGN_SCOPE_AMBIGUOUS) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return NULL;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};

/// GeneralStatementExprAST - Expression Class for single-line general statements.
class 	GeneralStatementExprAST : public ExprAST {
public:
  std::string Name;
  ExprAST* Assign;

  GeneralStatementExprAST(std::string name, ExprAST* assign=NULL) : 
  Assign(assign), Name(name) {}
  virtual std::string CodeGen(int* GeneratedCodeOpType,int WantOpType,Symtable* symtab, int depth);
  int GetType(void){return AST_TYPE_COLLECTION;};
  void* GetCollection(void){return NULL;};
  void DebugPrint(int level);
  void SymboltableBuild(int level, Symtable*);
};
