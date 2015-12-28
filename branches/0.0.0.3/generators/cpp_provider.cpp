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
		  file += "\n";
		}
		myfile.close();
	  }else
		  std::cout << "Error: Code Generation - Error opening " << directory << '\n';
	  

	return file;
}



std::string emit_core_code(void)
{
	
	std::string code = _provider_readfile("runtime.cppprovider");
	code += _provider_readfile("variant.cppprovider");
	code += _provider_readfile("core.cppprovider");
	return code;
}


std::string emit_entry_point(void)
{
	std::string code = "int main(int argc, char* argv[]){\n\ttry{\n";
	return code;
}


std::string emit_entry_point_closeure(void)
{
	std::string code = "\n\n\t}catch(std::bad_alloc){\n\tstd::cout << \"ERROR\";\n\t};\n}\n\n";
	return code;
}

std::string emit_global_definitions(Symtable* sym)
{
	std::string code = "//Global Variable Symbols (constructor initialising to INT64 of value 0)\nnamespace GlobalVars{\n";
	int i=0;
	for(;i < sym->entries.size(); i++)
	{
		if(sym->entries[i]->domain == "")//Global Definition
		{
			code += "  Variant Gv";
			code += sym->entries[i]->varname;
			code += ";\n";
		}
	}
	code += "};\nusing namespace GlobalVars;\n\n";
	return code;
}

std::string emit_symbol_dereference_global(std::string name)
{
	std::string sym = "Gv";
	sym += name;
	return sym;
}
