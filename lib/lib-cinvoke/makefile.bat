PATH=C:\mingw\bin\

del libcinvoke.a

gcc -I arch/ -I ../ -c cinvoke.c
gcc -I arch/ -I ../ -c structure.c
gcc -I arch/ -I ../ -c  hashtable.c
gcc -I../ -c  arch/gcc_x86_win.c

ar -cvq libcinvoke.a *.o
ar -t libcinvoke.a

rem nm libcinvoke.a

pause
