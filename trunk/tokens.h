// TOKENTYPES.H : DEFINES TOKEN TYPES USED IN THE 'TOKEN' STRUCTURE, AND THE
// STRUCTURE ITSELF.
// Liscenced under MIT.

enum
{
        //General file/line managment tokens
        EOL_TOK,                                                        //End of line
        SOF_TOK,                                                        //Start of file
        EOF_TOK,                                                        //End of file
        UNKNOWN_TOK,                                            //Unknown

        //Variables
        VARIANT_VARIABLE_TOK,                           //Straight variable declaration.
        ARRAY_VARIABLE_TOK,                                     //Array Variable
        MACRO_TOK,                                                      //Read only
        LFT_ARRAY_PARANTH_TOK,                          //[
        RHT_ARRAY_PARANTH_TOK,                          //]

        //Operators
        COMP_OP_TOK,                                            //< > <= >= NOT OR AND
        MATH_OP_TOK,                                            //+ - / * ^
        EQUAL_TOK,                                                      //=
        DOUBLE_EQUAL_TOK,                                       //==
        REL_ASSIGN_TOK,                                         //+= -= /= *= ^=

        //Literals
        NUMHEX_TOK,                                                     //0x*
        INT32_TOK,                                                      //NUMBERS
        INT64_TOK,                                                      //NUMBERS
        DOUBLE_TOK,                                                     //NUMBERS
        STRINGLITERAL_TOK,                                      //String Literals

        //Control block statements
        IF_TOK,                                                         //IF THEN ENDIF
        WHILE_TOK,                                                      //WHILE WEND
        FOR_TOK,                                                        //FOR IN STEP NEXT
        SWITCH_TOK,                                                     //SWITCH ENDSWITCH
        DO_TOK,                                                         //DO UNTIL
        SELECT_TOK,                                                     //SELECT ENDSELECT
        CASE_TOK,                                                       //CASE

        //Functions
        FUNC_DEC_TOK,                                           //FUNC functionname()
        FUNC_END_TOK,                                           //ENDFUNC
        FUNC_BUILTIN_TOK,                                       //Any builtin function
        FUNC_UNKNOWN_TOK,                                       //Anything recognised as func call but not builtin
        LFT_BRACKET_TOK,                                        //(
        RHT_BRACKET_TOK,                                        //)
        PARAM_SEPARATOR_TOK,                            //,

        //Scope modifiers
        CONSTANT_KEYWORD_TOK,                           //Const
        GLOBAL_KEYWORD_TOK,                                     //Global
        LOCAL_KEYWORD_TOK,                                      //Local
        DIM_KEYWORD_TOK,                                        //Dim

        //Misc
        ENUM_KEYWORD_TOK,                                       //Enum
        OOP_DOT_TOK,                                            //.
        STATEMENT_MODIFIER_TOK,                         //Exitloop Continueloop
        FUNC_MOD_TOK,                                           //Return
        GENERAL_STATEMENT_TOK,                          //Exit

        //Tokens used internally - By the scoper
        SCOPER_DATA_REF_TOK,                            //Scoper reference to data
        SCOPER_CODE_REF_TOK,                            //Scoper reference to code
        SCOPER_NO_TOK,                                  //Token that should never, ever exist
        
        AST_REF_TOKEN									//Indicates the token holds an AST branch
};



//Token structure
struct token
{
        int type;
        std::string data;
		void* ASTnode;		
};

