/***************************************
* FALCON runtime core version 0.1.51
* exc. Variants
*
* inc. Builtin/macro exports
* inc. Resource system
*
* todo: memory pool + allocator
* Written by Hyperzap, 2012
***************************************/


#undef UNICODE

//Hackz to make certian things work correctly.
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#define _WIN32_IE    0x0500
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <Shlwapi.h>
#include <new>
#include <shlobj.h>

/**************************************************
 *    Falcon Variant System - Variable Storage
 *    General purpose variable class
 *    meant for intermediate language
 *    to C++.
 *    Written by Shaggi, 2012. Modified by hyperzap.
 *************************************************/

//Possible variable types.
enum{
    VAR_INTTYPE,
    VAR_DOUBLETYPE,
    VAR_STRTYPE,
    VAR_PTRTYPE,
    VAR_REFTYPE,
    VAR_ARRAYTYPE
};



class Variant
{
public:
    //Constructors
    Variant(void) : type(VAR_INTTYPE), int64value(0), databuffsize(0), databuff((char*)0) {};
    Variant(int);
    Variant(long long);
    Variant(double);
    Variant(const char* in);
    Variant(char* in);
    Variant(const Variant & in);//Copy Constructor

    ~Variant();//Destructor



    //Operators
    inline Variant& operator= (Variant);
    inline Variant& operator&= (Variant);

    inline Variant operator+ (Variant);//Addition
    inline Variant operator- (Variant);//Subtraction
    inline Variant operator- ();       //Unary Subtraction
    inline Variant operator* (Variant);//Multiplication
    inline Variant operator/ (Variant);//Division
    inline Variant operator& (Variant);//Concatenation
    void concat_op(char*, char*, int);//concatenation workhorse
    
    //Inline type conversions
    inline operator bool();

    //Explicit type conversions
    char* String(void);
    int Short(void);
    long long Int(void);
    double Double(void);

    int type;

    //Different Datatypes
    union
    {
        long long int64value;
        void* ptr;
        double dvalue;
        Variant* varref;
    };

    char* databuff;
    int databuffsize;
private:
    void StorageReset(void);
    void Cache_StrVal(void);
};






//Constructors
inline Variant::Variant(int in)
{
    type = VAR_INTTYPE;
    int64value = in;
    databuff = (char*)0;
}
inline Variant::Variant(long long in)
{
    type = VAR_INTTYPE;
    int64value = in;
    databuff = (char*)0;
}
Variant::Variant(double in)
{
    type = VAR_DOUBLETYPE;
    dvalue = in;
    databuff = (char*)0;
}
inline Variant::Variant(const char* in)
{
    type = VAR_STRTYPE;
    databuff = (char*)in;
    databuffsize = 0;
}
Variant::Variant(char* in)
{
    type = VAR_STRTYPE;
    int len = strlen(in);
    databuff = new char[len+1];
    strcpy(databuff, in);
    databuffsize = len+1;
}
//Copy Constructor - DESTRUCTOR ALREADY CALLED THEREFORE NO NEED TO FREE
Variant::Variant(const Variant & in)
{
    int len;
    databuff = (char*)0;
    switch(in.type)
    {
    case VAR_INTTYPE:        //Int
        type = VAR_INTTYPE;
        int64value = in.int64value;
        break;
    case VAR_DOUBLETYPE:    //decimal
        type = VAR_DOUBLETYPE;
        dvalue = in.dvalue;
        break;
    case VAR_STRTYPE:        //String
        type = VAR_STRTYPE;
        len = strlen(in.databuff);
        databuff = new char[len+1];
        strcpy(databuff, in.databuff);
        databuffsize = len+1;
        break;
    default:                //Other - Cannot copy.
        type = VAR_INTTYPE;
        int64value = 0;
    }

}







//Assignment operator
Variant& Variant::operator= (Variant in){//Remember to update copy constructor with near duplicate!
                                        //But without the StorageReset() as copy calls destructor.
    //Destroy our own data in prep for assignment
    this->StorageReset();

    int len;
    switch(in.type)
    {
    case VAR_INTTYPE:        //Int
        type = VAR_INTTYPE;
        int64value = in.int64value;
        break;
    case VAR_DOUBLETYPE:    //decimal
        type = VAR_DOUBLETYPE;
        dvalue = in.dvalue;
        break;
    case VAR_STRTYPE:        //String
        type = VAR_STRTYPE;
        len = strlen(in.databuff);
        databuff = new char[len+1];
        strcpy(databuff, in.databuff);
        databuffsize = len+1;
        break;
    default:                //Other - Cannot copy.
        type = VAR_INTTYPE;
        int64value = 0;
    }
    return *this;
}

Variant& Variant::operator&= (Variant in){
    int len = strlen(this->String()) + strlen(in.String()) + 1;
    this->concat_op(this->String(), in.String(), len);
    return *this;
}



//Operators
Variant Variant::operator+ (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value+in.int64value));
    else{
    long long outl = (long long)(this->Double() + in.Double());
    double outf = this->Double() + in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() + in.Double()));
    else
        return Variant(this->Double() + in.Double());
    }
}

Variant Variant::operator- (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value-in.int64value));
    else{
    long long outl = (long long)(this->Double() - in.Double());
    double outf = this->Double() - in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() - in.Double()));
    else
        return Variant(this->Double() - in.Double());
    }
}

Variant Variant::operator- (){
    if(type==VAR_INTTYPE)
        return Variant((long long)(-int64value));
    else return Variant(-this->Double());
}

Variant Variant::operator* (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value*in.int64value));
    else{
    long long outl = (long long)(this->Double() * in.Double());
    double outf = this->Double() * in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() * in.Double()));
    else
        return Variant(this->Double() * in.Double());
    }
}



Variant Variant::operator/ (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value/in.int64value));
    else{
    long long outl = (long long)(this->Double() / in.Double());
    double outf = this->Double() / in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() / in.Double()));
    else
        return Variant(this->Double() / in.Double());
    }
}

Variant Variant::operator& (Variant in){
    Variant ret;
    this->Cache_StrVal();
    char* str = in.String();
    ret.concat_op(this->databuff, str, strlen(this->String()) + strlen(str) + 1);
    return ret;
}



void Variant::concat_op(char* former, char* latter, int finallen)
{
    char* all = new char[finallen];
    strcpy(all, former);
    strcat(all, latter);
    this->StorageReset();
    databuff = all;
    databuffsize = finallen;
    type = VAR_STRTYPE;
}






//Destructor
Variant::~Variant()
{
    this->StorageReset();
}








//Internal functions
void inline Variant::StorageReset(void)
{
    if(databuff != 0)
    {
        if(databuffsize > 0)
        {
            delete [] databuff;
        }
    }
    type = VAR_INTTYPE;
    int64value = 0;
    databuff = (char*)0;
}

void Variant::Cache_StrVal(void)
{
    char buffer[32];
    
    if(databuff)//Already Cached.
        return;
    
    switch(type){
        case VAR_INTTYPE:
            sprintf(buffer, "%d", int64value);
            databuffsize = strlen(buffer)+1;
            databuff = new char[databuffsize];
            strcpy(databuff, buffer);
            break;
        case VAR_DOUBLETYPE:
            sprintf(buffer, "%f", dvalue);
            databuffsize = strlen(buffer)+1;
            databuff = new char[databuffsize];
            strcpy(databuff, buffer);
            break;
        default://All other cases: empty string.
            databuff = '\0';
            break;
    }
}

char* Variant::String(void){
    this->Cache_StrVal();
    return databuff;
}

int Variant::Short(void){
    switch(type){
        case VAR_INTTYPE:
            return (int)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (int)dvalue;
            break;
        case VAR_STRTYPE:
            return atoi(databuff);
            break;
        default:
            return 0;
            break;
    }
}

long long Variant::Int(void){
    switch(type){
        case VAR_INTTYPE:
            return (long long)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (long long)dvalue;
            break;
        case VAR_STRTYPE:
#ifdef _MSC_VER
            return _atoi64(databuff);
#else
            return atoll(databuff);
#endif
            break;
        default:
            return 0;
            break;
    }
}

double Variant::Double(void){
    switch(type){
        case VAR_INTTYPE:
            return (double)int64value;
            break;
        case VAR_DOUBLETYPE:
            return dvalue;
            break;
        case VAR_STRTYPE:
            return atof(databuff);
            break;
        default:
            return 0;
            break;
    }
}














Variant::operator bool()
{
    switch(type){
        case VAR_INTTYPE:
            return (bool)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (bool)dvalue;
            break;
        case VAR_STRTYPE:
            if(databuff=='\0')return false;
            return true;
            break;
        default:
            return false;
            break;
    }
}
















typedef Variant vr;



///////////////////////////////////////////////////////////////////////////////
// Util_RegReadString()
//
// Read a string from a registry key
//
///////////////////////////////////////////////////////////////////////////////

void Util_RegReadString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwBufLen, char *szValue )
{
        HKEY    hRegKey;

        // Make sure the return value is blank just in case we error
        szValue[0] = '\0';

        if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_QUERY_VALUE, &hRegKey) != ERROR_SUCCESS)
                return;

        RegQueryValueEx(hRegKey, lpValueName, NULL, NULL, (LPBYTE)szValue, &dwBufLen);

        RegCloseKey(hRegKey);

} // Util_RegReadString()








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// BUILTINS
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Builtin function :: <ret: int> ConsoleWrite(<text>)
vr fCONSOLEWRITE(vr text)
{
	std::cout << text.String();
	int siz = strlen(text.String());
	return vr(siz);
}



//Builtin function :: <ret: none> Sleep(<int>)
vr fSLEEP(vr time)
{
	Sleep(time.Short());
	return vr();
}




















///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// MACROS
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//MACRO: @CRLF
vr mCRLF()
{
	return vr("\r\n");
}
//MACRO: @CR
vr mCR()
{
	return vr("\r");
}
//MACRO: @LF
vr mLF()
{
	return vr("\n");
}
//MACRO: @PROGRAMFILESDIR
vr mPROGRAMFILESDIR()
{
	char temp[MAX_PATH+1];
	Util_RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion", "ProgramFilesDir", MAX_PATH, temp);
	return vr(temp);
}
//MACRO: @COMMONFILESDIR
vr mCOMMONFILESDIR()
{
	char temp[MAX_PATH+1];
	Util_RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion", "CommonFilesDir", MAX_PATH, temp);
	return vr(temp);
}
//MACRO: @MYDOCUMENTSDIR
vr mMYDOCUMENTSDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_PERSONAL, FALSE);
	return vr(temp);
}
//MACRO: @APPDATACOMMONDIR
vr mAPPDATACOMMONDIR()
{
	char temp[MAX_PATH+1];
    Util_RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Common AppData", MAX_PATH, temp);
	return vr(temp);
}
//MACRO: @DESKTOPCOMMONDIR
vr mDESKTOPCOMMONDIR()
{
	char temp[MAX_PATH+1];
    Util_RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "Common Desktop", MAX_PATH, temp);
	return vr(temp);
}

//Global Variable Symbols (constructor initialising to INT64 of value 0)
namespace GlobalVars{
  Variant GvLIT;
  Variant GvMAEO;
  Variant GvR;
};
using namespace GlobalVars;



int main(int argc, char* argv[]){
	try{
    GvLIT = (vr("THIS IS A TEST STRING."));
    GvLIT &= (vr("This " is a test append""));
    GvMAEO = (vr("LOL"));
    GvR = (fCONSOLEWRITE(((GvLIT&((vr(" ")&GvLIT)&GvMAEO))&mCRLF())));
    GvR = ((GvR+vr(20)));
   fCONSOLEWRITE(((((GvR&vr("::"))&vr(2))&vr(255))&mCRLF()));


	}catch(std::bad_alloc){
	std::cout << "ERROR";
	};
}

