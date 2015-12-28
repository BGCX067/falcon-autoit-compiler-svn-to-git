/* Header for the main engine/interface of this compiler */


struct Functionentry
{
        std::string name;
        int num_params;
        int num_opt_params;
        std::vector<std::string> parameters_name;
        std::vector<bool> parameters_byref;
        std::vector<ExprAST*> parameters_optional_values;//Empty nodes are NOT pushed for arguments that are not optional type.
        CollectionExprAST* code;
		Symtable* SymbolTable;
};


class Compiler
{
public:
        int Set_CWD(char* filename);            //Sets the build directory.
        int Add_Sourcefile(char* filename);     //Adds sourcefile to compile.
        int Compile(void);                      //Compiles code to AST and analyses.
        void Set_Verbose(void);					//isVerboseMode = true
        void UnSet_Verbose(void);				//isVerboseMode = false
        int Error(char*, char*);				//Called by components on errors.
        int Warning(char*, char*);				//Called on warnings.
        void PrintAST(void);					//Called to dump a visual representation of the AST to stdout.
        void GenerateCode_ToSTDOUT(void);
        void GenerateCode_ToFile(char*);
        void DumpTokens_ToFile(char*);
		void DumpSymTable_ToFile(char* filename);
        std::vector<token*> tokenvect;          //List of tokens.
        std::vector<Functionentry*> UDFnodes;	//List of functions, attributes, and code branches.
        CollectionExprAST* code;
        Symtable SymbolTable;
private:
		bool has_compiled;
        std::string cwd;
        std::vector<std::string> sourcefiles;
        bool isVerboseMode;
};
