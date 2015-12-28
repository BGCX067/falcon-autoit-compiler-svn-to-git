class Lexer
{
public:
        Lexer(char* filename);                                  //Constructor - Opens file, setups state
        char scriptbase_filename[MAX_PATH];             //Holds filename
        void Insert_Token(int type, char* data);//Inserts a token into end of list
        int Lex_Code( char* code);                              //Lexes an arbitrary block of code
        void Print_Tokens(void);                                //Output tokens to stdout for debug purposes.
        void Lex_File(char* filename);                  //Lexes an entire file.
        int Tok_Count(void);                                    //Returns the number of tokens stored.
        void Dump_Tokens(char*filename);
        std::list<token>* Export_Tokens(void);  //Returns a pointer to the token list.
        void Import_Tokens(std::list<token>);   //Gets and stores a token list.
private:
        int filehnd;                                                    //Filehnd for base scriptfile.
        int lexer_state;                                                //State: 0 = off, 1 = running
        std::list<token> tokenlist;                             //List of tokens
};