/* Main entry Point. */

#include "stdafx.h"
#include "tokens.h"
#include "lexer/lexer.h"
#include "scriptgraph/nodes.h"
#include "compiler.h"
#include "generators/provider.h"

char* ExecutableDirectory(void){

  HMODULE hModule = GetModuleHandleA(NULL);
  char* path = new char[MAX_PATH];
  GetModuleFileNameA(hModule, path, MAX_PATH);
  
  PathRemoveFileSpecA(path); //Strip filename to get directory.

  return path;
}


int main(int argc, char* argv[])
{
        Compiler compiler;
        
        if(argc < 2){		//No arguments; start test suite
			std::cout << "-==== Developer test suite activated ====-\nConponents Directory: " << ExecutableDirectory() << "\n\n";
			char* cd = new char[255];
			strcpy(cd, "C:\\");
			compiler.Set_CWD(cd);
			compiler.Set_Verbose();
			compiler.Add_Sourcefile("C:\\testsource.au3");
			compiler.Compile();
			compiler.PrintAST();
			std::string temp;
			std::cin >> temp;
		}
		
		
		
		
		else{				//Normal operation - Compile based on cmdline instructions.
			
		
			compiler.UnSet_Verbose();	//Default is slient except for errors.
			
			char* cwdpath = new char[MAX_PATH];
			bool knownCWD = false;
						
			int cmdpos = 1;
			for(;cmdpos<argc;cmdpos++){
				
				//std::cout << "TRIG: " << argv[cmdpos] << '\n'; //Debug
				
				switch(*argv[cmdpos]){
					
				case '-':
					if((*(argv[cmdpos]+1) == 'v')||(*(argv[cmdpos]+1) == 'V'))	//Set verbose mode switch
						compiler.Set_Verbose();
						
					else if((*(argv[cmdpos]+1) == 'p')||(*(argv[cmdpos]+1) == 'P'))
						compiler.GenerateCode();
					
					else if((*(argv[cmdpos]+1) == 'w')||(*(argv[cmdpos]+1) == 'W')) //Compile source to file
					{
						char filename[255];
						strcpy( filename, argv[cmdpos+1]);
						compiler.GenerateCode_ToFile(filename);
						cmdpos++;
					}
					
					else if((*(argv[cmdpos]+1) == 't')||(*(argv[cmdpos]+1) == 'T')) //Dump Tokens to a file
					{
						char filename[255];
						strcpy( filename, argv[cmdpos+1]);
						compiler.DumpTokens_ToFile(filename);
						cmdpos++;
					}
						
					else if((*(argv[cmdpos]+1) == 'd')||(*(argv[cmdpos]+1) == 'D')) //Set CWD switch
					{
						strcpy( cwdpath, argv[cmdpos+1]);
						PathRemoveFileSpecA( cwdpath);
						compiler.Set_CWD(cwdpath);
						knownCWD = true;
						cmdpos++;
					}
					break;
					
				default:
					if (!knownCWD){
						strcpy( cwdpath, argv[cmdpos]);
						PathRemoveFileSpecA( cwdpath);
						compiler.Set_CWD(cwdpath); 	//Obviously, the last passed sourcefile is the CWD.
					}
					compiler.Add_Sourcefile(argv[cmdpos]);
				}

			}
			
			
			
		}
}
