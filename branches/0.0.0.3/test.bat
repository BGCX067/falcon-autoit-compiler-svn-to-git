falcone.exe "C:\testsource.au3" -d "C:\falcon\trunk\\" -v -t tokens.txt -w output.cpp -s symbols.txt
PATH=C:\mingw\bin\
del testout.exe
g++ -D BUILD_WINDOWS -o testout -static-libgcc -static-libstdc++  output.cpp -lshlwapi
testout.exe
pause