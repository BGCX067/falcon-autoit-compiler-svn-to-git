#pragma once


#undef UNICODE

//Hackz to make certian things work correctly.
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

// TODO: reference additional headers your program requires here
#include <windows.h>
//C++ Datatypes
#include <list>
#include <vector>
#include <stack>
#include <string>
//Input/Output
#include <iostream>
#include <fstream>
#include <stdio.h>
//Standard library
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Extra
#include <Shlwapi.h>
