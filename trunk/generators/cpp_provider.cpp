/* Provider interface for C++ code generation. */

#include "../stdafx.h"
#include "../scriptgraph/nodes.h"
#include "provider.h"
char* ExecutableDirectory(void); //from main.cpp


std::string _provider_readfile(char* filename)
{
	
	char directory[MAX_PATH];	//Actually represents the full path.
	strcpy( directory, ExecutableDirectory());
	strcat( directory, "\\core\\");
	strcat( directory, filename);
	
	std::string line;
	std::string file;
	std::ifstream myfile (directory);
	
	
	  if (myfile.is_open())
	  {
		while ( myfile.good() )
		{
		  getline (myfile,line);
		  file += line;
		  file += (char*)"\n";
		}
		myfile.close();
	  }else
		  std::cout << "Error: Code Generation - Error opening " << directory << '\n';
	  

	return file;
}



std::string emit_core_code(void)
{
	
	std::string code = _provider_readfile((char*)"base.cpp");
	//Considering eliminating the runtime engine for now; I should keep such things
	//as minimal as possible.
	//code += _provider_readfile((char*)"runtime_structures.cppprovider");
	//code += _provider_readfile((char*)"runtime_engine.cppprovider");
	code += _provider_readfile((char*)"variant.h");
	code += _provider_readfile((char*)"variant.cpp");
	code += _provider_readfile((char*)"core.cpp");
	//code += _provider_readfile((char*)"filecore.cppprovider");
	return code;
}


std::string emit_entry_point(void)
{
	return std::string(_provider_readfile((char*)"entrypoint.cpp"));
}


std::string emit_entry_point_closeure(void)
{
	std::string code = (char*)"\n\n\n}\n\n";
	return code;
}

std::string emit_global_definitions(Symtable* sym)
{
	std::string code = (char*)"//Global Variable Symbols (constructor initialising to INT64 of value 0)\nnamespace GlobalVars{\n";
	int i=0;
	for(;i < sym->entries.size(); i++)
	{
		if(sym->entries[i]->domain == "")//Global Definition
		{
			code += (char*)"  Variant Gv";
			code += sym->entries[i]->varname;
			code += (char*)";\n";
		}
	}
	code += (char*)"};\nusing namespace GlobalVars;\n\n";
	return code;
}

std::string emit_symbol_dereference_global(std::string name)
{
	std::string sym = (char*)"Gv";
	sym += name;
	return sym;
}
