

#include "../stdafx.h"
#include "../tokens.h"
#include "../scriptgraph/nodes.h"
#include "parser.h"


std::vector<std::string> DefaultSymbols(void)
{
	std::vector<std::string> ret;
	ret.push_back("CONSOLEWRITE");
	ret.push_back("SLEEP");
	return ret;
}

bool _IsIn(std::vector<std::string>* set, std::string in)
{
	int poscount = 0;
	for(;poscount<set->size();poscount++)
	{
		if((*set)[poscount] == in)return true;
	}
	return false;
} 

int Tokens_Disambiguate_Pass(std::vector<token*>* tokenvect)
{
	std::vector<std::string> declared_symbols = DefaultSymbols();
	
	int poscount = 0;
	for(;poscount<tokenvect->size();poscount++)
	{
		if((*tokenvect)[poscount]->type == FUNC_DEC_TOK)
			declared_symbols.push_back((*tokenvect)[poscount]->data);
	}
	
	
	poscount = 0;
	for(;poscount<tokenvect->size();poscount++)
	{
		if(((*tokenvect)[poscount]->type == UNKNOWN_TOK) &&
			(!_IsIn(&declared_symbols, (*tokenvect)[poscount]->data)))
		{
			if(!((*tokenvect)[poscount+1]->type == LFT_ARRAY_PARANTH_TOK))
				(*tokenvect)[poscount]->type = VARIANT_VARIABLE_TOK;
			else
				(*tokenvect)[poscount]->type = ARRAY_VARIABLE_TOK;
		}			
		
	}
	
	
	return 0;
}
