@echo off
rem ####################################################################
rem FALCON Build Script
rem MAKE SURE YOUR minGW DIRECTORY IS AT C:\minGW !!!!!!!!!!!!!!!!!!!!!!
PATH=C:\mingw\bin\


del falcon.exe
del output.cpp
del tokens.dump
del toks.txt
del falcone.exe

rem ############### BUILDING ##########################################

echo    
echo Compiling...
g++ -D BUILD_WINDOWS -o falcone -s -static-libgcc -static-libstdc++  *.cpp lexer/lexer.cpp generators/cpp_provider.cpp parser/*.cpp scriptgraph/*.cpp -lshlwapi




pause
