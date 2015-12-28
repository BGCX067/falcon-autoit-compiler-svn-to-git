/* Lexer Code */

// LEXER.CPP : Reads in a sourcefile and produces tokens.
// Liscenced under MIT.

#include "../stdafx.h"
#include "../tokens.h"
#include "lexer.h"

//FIXME: EXPONENTIAL LEXING + CALCULATING FAILS
//TODO:
//		TAKE NOTE OF PREPROCESSOR DIRECTIVES

//Constructor
Lexer::Lexer(char * filename){
	//Save the filename
	strcpy( this->scriptbase_filename, filename);
	//Setup the statemachine
	this->lexer_state = 1;
	//Insert an SOF token to the list.
	Lexer::Insert_Token( SOF_TOK, filename);
}

std::list<token>* Lexer::Export_Tokens(void){
	return &tokenlist;
}

void Lexer::Import_Tokens(std::list<token> toks){
	this->tokenlist = toks;
}

//Inserts a token into the end of the list
void Lexer::Insert_Token(int type, char* data){
	//Create the new token
	token* tok = new token;
	unsigned int count = strlen(data);
	char* dataptr = new char[count+1];
	strcpy(dataptr, data);
	dataptr[count] = '\0';
	tok->data = dataptr;
	tok->type = type;

	//Insert into the list
	this->tokenlist.insert(this->tokenlist.end(), *tok);
}

//Lexes a block of code. code = null terminated string of code.
int Lexer::Lex_Code( char* code){
	unsigned int i = 0;
	unsigned int iPosTemp;
	unsigned int Extend_Line = 0; //Set to 1 when '_' token on end of line.
	char ch;

	while(code[i] != '\0'){
		std::string temp;

		// DEMOLISH Whitespaceyness.
		while (code[i] == ' ' || code[i] == '\t')
			++i;
		// Another check for end of block
		if (code[i] == '\0')
			break;

		// Zero our pos.
		iPosTemp = 0;
		ch = code[i];						// Get the current character

		// Now we check if it is a number.
		if ( ((ch >= '0' && ch <= '9') || ch == '.'))
		{

			//First check if hex number.
			if ((ch=='0') && ((code[i+1]=='X') || code[i+1]=='x')){
				temp = "0x";
				i+=2;//Jump to the hex part of the hex literal.
				while((code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'f') || (code[i] >= 'A' && code[i] <= 'F')){
					temp += code[i];
					i++;
					if (code[i] == '\0')break;
				}
				this->Insert_Token( NUMHEX_TOK, (char*)temp.c_str());
				continue;
			}

			int toktype = INT32_TOK;

			//So it must be a normal number.
			while( ((code[i] >= '0' && code[i] <= '9') || code[i] == '.'))
			{
				if(code[i] == '.')//If its a float.
					toktype = DOUBLE_TOK;

				temp += code[i];
				i++;
				if (code[i] == '\0')break;//FAILURE

				//Peek ahead to see if special case - exponential.
				if((code[i]=='e') || (code[i] == 'E')){
					std::string temp2;
					i++;

					while(code[i] >= '0' && code[i] <= '9')
					{
						temp2 += code[i];
						i++;
					}
					double exp = (double)atoi(temp2.c_str());
					double base = (double)atoi(temp.c_str());
					base = pow(base, exp);
					char* cnum = new char[10];
					itoa( base, cnum, 10);
					temp = cnum;
					break;
				}
			}

			if(toktype == DOUBLE_TOK){;;;}
			else if(toktype == INT32_TOK){//Detect the type of the number.
				long int num = atoi( temp.c_str());
				if(num > 65535){toktype = INT64_TOK;}
			}
			this->Insert_Token( toktype, (char*)temp.c_str());
			continue;// Next loop
		}

		//EVERYTHING ELSE:
		char start = code[i];
		switch(code[i]){
		case '@':
			//Collect macro
			i++;
			while((code[i]=='_')||(code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')){
				temp += code[i];
				i++;
				if (code[i] == '\0')break;
			}
			
			{
				char upstr[880];
				strcpy( upstr, temp.c_str());
				for( int s = 0; upstr[s] != 0; s++)
					upstr[s] = toupper(upstr[s]);
				temp = upstr;
			}
			this->Insert_Token( MACRO_TOK, (char*)temp.c_str());
			continue;// Next loop
			break;

		case '$':
			//Collect variable
			i++;
			while((code[i]=='_')||(code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')){
				temp += code[i];
				i++;
				if (code[i] == '\0')break;
			}

			//Make uppercase. I know this is a really dirty way to do it.
			{
				char upstr[880];
				strcpy( upstr, temp.c_str());
				for( int s = 0; upstr[s] != 0; s++)
					upstr[s] = toupper(upstr[s]);
				temp = upstr;
			}

			if(code[i]=='['){this->Insert_Token( ARRAY_VARIABLE_TOK, (char*)temp.c_str());}
			else{this->Insert_Token( VARIANT_VARIABLE_TOK, (char*)temp.c_str());}
			continue;// Next loop
			break;

		case '+': case '-': case '/': case '*': case '&': case '^':
			temp += code[i];
			if(code[i+1] == '=')//Check relative assignments
			{temp += '=';this->Insert_Token( REL_ASSIGN_TOK, (char*)temp.c_str());}
			else{i--;this->Insert_Token( MATH_OP_TOK, (char*)temp.c_str());}
			i++;
			break;

		case '=':
			temp += '=';
			if(code[i+1] == '=')
			{
				temp += '=';
				this->Insert_Token( DOUBLE_EQUAL_TOK, (char*)temp.c_str());
			}else{i--;this->Insert_Token( EQUAL_TOK, (char*)temp.c_str());}
			i++;
			break;

		case '<': case '>':
		temp += code[i];
			if(code[i+1] == '=')
				temp += '=';
			else i--;
			this->Insert_Token( COMP_OP_TOK, (char*)temp.c_str());
			i++;
			break;
		case '(':
			this->Insert_Token( LFT_BRACKET_TOK, "(");break;
		case ')':
			this->Insert_Token( RHT_BRACKET_TOK, ")");break;
		case ',':
			this->Insert_Token( PARAM_SEPARATOR_TOK, ",");break;
		case '[':
			this->Insert_Token( LFT_ARRAY_PARANTH_TOK, "[");break;
		case ']':
			this->Insert_Token( RHT_ARRAY_PARANTH_TOK, "]");break;
		case '.':
			this->Insert_Token( OOP_DOT_TOK, ".");break;

		case '"': case '\'':
			//start defined at beginning of switch.
			i++;
			while((code[i]!=start) || (code[i+1]==start))
			{
				if(code[i]==start && code[i+1]==start){
					temp += code[i];
					i+=2;
				}else{
					temp += code[i];
					i++;
				}
			}
			this->Insert_Token( STRINGLITERAL_TOK, (char*)temp.c_str());
			break;

		case '\n':
			if(!Extend_Line){
				this->Insert_Token( EOL_TOK, "n");
			}else{
				Extend_Line = 0;
			}
			break;
		case '\r':
			break;

		case ';':
			while((code[i]!='\r')&&(code[i]!='\n')&&(code[i]!='\0'))
				i++;
			i--;
			break;

		default:

			//Collect unknown word.
			while((code[i]=='_')||(code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')){
				temp += code[i];
				i++;
				if (code[i]=='\0'){i--;break;};
			}
			i--;
			
			if(strlen(temp.c_str())<=1){i++;break;}

			CharUpper((LPSTR)temp.c_str());//Shutup. I know its hacky.
			//std::cout << "UNKNOWN: " << temp << '\n';
			if (strcmp(temp.c_str(), "FUNC") == 0){
				//Readin the function name
				i++;
				std::string temp2 = "";
				//Advance to func name.
				while(!((code[i]=='_')||(code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')))
					i++;

				//Collect function name.
				while((code[i]=='_')||(code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')){
					temp2 += code[i];
					i++;
				}
				i--;
				CharUpper((LPSTR)temp2.c_str());
				this->Insert_Token( FUNC_DEC_TOK, (char*)temp2.c_str());
			}else if(strcmp(temp.c_str(), "ENDFUNC") == 0){
				this->Insert_Token( FUNC_END_TOK, "ENDFUNC");
			}else if(strcmp(temp.c_str(), "IF") == 0){
				this->Insert_Token( IF_TOK, "IF");
			}else if(strcmp(temp.c_str(), "ELSEIF") == 0){
				this->Insert_Token( IF_TOK, "ELSEIF");
			}else if(strcmp(temp.c_str(), "ELSE") == 0){
				this->Insert_Token( IF_TOK, "ELSE");
			}else if(strcmp(temp.c_str(), "THEN") == 0){
				this->Insert_Token( IF_TOK, "THEN");
			}else if(strcmp(temp.c_str(), "ENDIF") == 0){
				this->Insert_Token( IF_TOK, "ENDIF");
			}else if(strcmp(temp.c_str(), "CONST") == 0){
				this->Insert_Token( CONSTANT_KEYWORD_TOK, "CONST");
			}else if(strcmp(temp.c_str(), "GLOBAL") == 0){
				this->Insert_Token( GLOBAL_KEYWORD_TOK, "GLOBAL");
			}else if(strcmp(temp.c_str(), "LOCAL") == 0){
				this->Insert_Token( LOCAL_KEYWORD_TOK, "LOCAL");
			}else if(strcmp(temp.c_str(), "DIM") == 0){
				this->Insert_Token( DIM_KEYWORD_TOK, "DIM");
			}else if(strcmp(temp.c_str(), "DIM") == 0){
				this->Insert_Token( DIM_KEYWORD_TOK, "DIM");
			}else if(strcmp(temp.c_str(), "WHILE") == 0){
				this->Insert_Token( WHILE_TOK, "WHILE");
			}else if(strcmp(temp.c_str(), "WEND") == 0){
				this->Insert_Token( WHILE_TOK, "WEND");
			}else if(strcmp(temp.c_str(), "FOR") == 0){
				this->Insert_Token( FOR_TOK, "FOR");
			}else if(strcmp(temp.c_str(), "TO") == 0){
				this->Insert_Token( FOR_TOK, "TO");
			}else if(strcmp(temp.c_str(), "STEP") == 0){
				this->Insert_Token( FOR_TOK, "STEP");
			}else if(strcmp(temp.c_str(), "NEXT") == 0){
				this->Insert_Token( FOR_TOK, "NEXT");
			}else if(strcmp(temp.c_str(), "IN") == 0){
				this->Insert_Token( FOR_TOK, "IN");
			}else if(strcmp(temp.c_str(), "SWITCH") == 0){
				this->Insert_Token( SWITCH_TOK, "SWITCH");
			}else if(strcmp(temp.c_str(), "ENDSWITCH") == 0){
				this->Insert_Token( SWITCH_TOK, "ENDSWITCH");
			}else if(strcmp(temp.c_str(), "DO") == 0){
				this->Insert_Token( DO_TOK, "DO");
			}else if(strcmp(temp.c_str(), "UNTIL") == 0){
				this->Insert_Token( DO_TOK, "UNTIL");
			}else if(strcmp(temp.c_str(), "CASE") == 0){
				this->Insert_Token( CASE_TOK, "CASE");
			}else if(strcmp(temp.c_str(), "CONTINUECASE") == 0){
				this->Insert_Token( CASE_TOK, "CONTINUECASE");
			}else if(strcmp(temp.c_str(), "SELECT") == 0){
				this->Insert_Token( SELECT_TOK, "SELECT");
			}else if(strcmp(temp.c_str(), "ENDSELECT") == 0){
				this->Insert_Token( SELECT_TOK, "ENDSELECT");
			}else if(strcmp(temp.c_str(), "NOT") == 0){
				this->Insert_Token( COMP_OP_TOK, "NOT");
			}else if(strcmp(temp.c_str(), "OR") == 0){
				this->Insert_Token( COMP_OP_TOK, "OR");
			}else if(strcmp(temp.c_str(), "AND") == 0){
				this->Insert_Token( COMP_OP_TOK, "AND");
			}else if(strcmp(temp.c_str(), "EXITLOOP") == 0){
				this->Insert_Token( STATEMENT_MODIFIER_TOK, "EXITLOOP");
			}else if(strcmp(temp.c_str(), "CONTINUELOOP") == 0){
				this->Insert_Token( STATEMENT_MODIFIER_TOK, "CONTINUELOOP");
			}else if(strcmp(temp.c_str(), "RETURN") == 0){
				this->Insert_Token( FUNC_MOD_TOK, "RETURN");
			}else if(strcmp(temp.c_str(), "ENUM") == 0){
				this->Insert_Token( ENUM_KEYWORD_TOK, "ENUM");
			}else if(strcmp(temp.c_str(), "EXIT") == 0){
				this->Insert_Token( GENERAL_STATEMENT_TOK, "EXIT");
			}else if(strcmp(temp.c_str(), "REDIM") == 0){
				this->Insert_Token( GENERAL_STATEMENT_TOK, "REDIM");
			}else if(strcmp(temp.c_str(), "BYREF") == 0){
				this->Insert_Token( GENERAL_STATEMENT_TOK, "BYREF");
			}else if(strcmp(temp.c_str(), "DEFAULT") == 0){
				this->Insert_Token( GENERAL_STATEMENT_TOK, "DEFAULT");
			}else if(strcmp(temp.c_str(), "TRUE") == 0){
				this->Insert_Token( INT32_TOK, "1");
			}else if(strcmp(temp.c_str(), "FALSE") == 0){
				this->Insert_Token( INT32_TOK, "0");
			}else if(strcmp(temp.c_str(), "_") == 0){
				Extend_Line = 1;
			}
			else{
				this->Insert_Token( UNKNOWN_TOK, (char*)temp.c_str());
			}
			
			break;
		}
		i++;
	}
	
	//an unknown memory corruption fucks shit up somewhere when parsing the last few tokens.
	this->Insert_Token( EOL_TOK, "n");
	this->Insert_Token( EOL_TOK, "n");
	this->Insert_Token( EOL_TOK, "n");
	this->Insert_Token( EOL_TOK, "n");
	this->Insert_Token( EOL_TOK, "n");
	this->Insert_Token( EOL_TOK, "n");
	//that should fix the problem ;) DAMMIT IT DOESNT
	
	return 1;
}

void Lexer::Dump_Tokens(char*filename){
	std::fstream file( filename, std::ios::out);
	std::list<token>::iterator i;
	int count = 0;
	for( i = this->tokenlist.begin(); i != this->tokenlist.end(); ++i)
	{
		count++;
		char temp[10];
		sprintf( (char*)&temp, "%07d", count);
		file << temp << "  ";
		sprintf( (char*)&temp, "%02d", i->type);
		file << temp;
		file << ": =>";
		file << i->data;
		file << '\n';
	}
	file.close();
}

void Lexer::Lex_File(char* filename){

	std::string line;
	std::string file;
	std::ifstream myfile (filename);
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);
	  file += line;
	  file += "\n";
    }
	this->Lex_Code( (char*)file.c_str());
    myfile.close();
  }

}

void Lexer::Print_Tokens(void){
	std::cout << "TOKEN OUTPUT:\n\n";

	std::list<token>::iterator i;
	for( i = this->tokenlist.begin(); i != this->tokenlist.end(); ++i)
	{
		std::cout << i->type;
		std::cout << " : '";
		std::cout << i->data.c_str();
		std::cout << "'\n";
	}
}

int Lexer::Tok_Count(void){
	int ret = 0;
	std::list<token>::iterator i;
	for( i = this->tokenlist.begin(); i != this->tokenlist.end(); ++i)
		ret++;
	return ret;
}
