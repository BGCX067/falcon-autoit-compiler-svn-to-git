/***************************************
* FALCON runtime base version 1
*
* encapsulates: headers and defs
* inc. Global misc functions
*
* Written by Twitchyliquid64, 2012
***************************************/


#undef UNICODE

#define SLEEPFUNC_IDLETIME 10



//Hackz to make certian things work correctly.
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#define _WIN32_IE    0x0500
#include <Windows.h>
#include "Winuser.h"

//C++ Datatypes
#include <vector>
#include <string>
#include <queue>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Shlwapi.h>
#include <new>
#include <shlobj.h>


void _DEBUG_MSG(char* msg)
{
#ifdef DEBUG
	std::cout << msg << '\n';
#endif
}



/***************************************
* END Runtime Base
***************************************/
