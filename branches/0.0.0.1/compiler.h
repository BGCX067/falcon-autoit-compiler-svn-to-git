/* Header for the main engine/interface of this compiler */


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
        void GenerateCode(void);
        void GenerateCode_ToFile(char*);
        void DumpTokens_ToFile(char*);
        std::vector<token*> tokenvect;          //List of tokens.
        CollectionExprAST* code;
private:
        std::string cwd;
        std::vector<std::string> sourcefiles;
        bool isVerboseMode;
};
