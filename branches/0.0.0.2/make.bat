@echo off
rem ####################################################################
rem FALCON Build Script
rem MAKE SURE YOUR minGW DIRECTORY IS AT C:\minGW !!!!!!!!!!!!!!!!!!!!!!
PATH=C:\mingw\bin\

g++ -D BUILD_WINDOWS -o testout -static-libgcc -static-libstdc++  output.cpp -lshlwapi

del falcon.exe
del output.cpp
del tokens.dump
del tokens.txt
del falcone.exe

rem ############### BUILDING ##########################################


g++ -D BUILD_WINDOWS -o falcone -static-libgcc -static-libstdc++  *.cpp lexer/lexer.cpp generators/cpp_provider.cpp parser/*.cpp scriptgraph/*.cpp -lshlwapi




pause
