
//Forward decl
vr fSETERROR(vr err, vr ext, vr ret);


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









///////////////////////////////////////////////////////////////
//@ERROR and @EXTENDED MACROS AND SETERROR() BUILTIN
///////////////////////////////////////////////////////////////
vr rERROR;
vr rEXTENDED;

vr mERROR()
{
	return rERROR;
};

vr mEXTENDED()
{
	return rEXTENDED;
};

vr fSETERROR(vr* perr=NULL, vr* pext=NULL, vr* pret=NULL)
{
	vr err;
	vr ext;
	vr ret;
	
	if(perr)
		err = *perr;
	if(pext)
		ext = *pext;
	if(pret)
		ret = *pret;

	rERROR = err;
	rEXTENDED = ext;
	return ret;
};






///////////////////////////////////////////////////////////////////////////////
/// CORE BUILTINS
///////////////////////////////////////////////////////////////////////////////

//Builtin function :: <ret: int> ConsoleWrite(<text>)
vr fCONSOLEWRITE(vr* text)
{
	fSETERROR();

	std::cout << text->String();
	int siz = strlen(text->String());
	return vr(siz);
}

//Builtin function :: <ret: none> Sleep(<int>)
vr fSLEEP(vr* ptime)
{
	vr time = *ptime;

	fSETERROR();

	long long fintime = clock() + time.Short();
	while( fintime > clock())
	{
		Sleep(SLEEPFUNC_IDLETIME);
		//_Runtime_ChkMsgEvents();
	}
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
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_APPDATA, FALSE);
	return vr(temp);
}
//MACRO: @DESKTOPCOMMONDIR
vr mDESKTOPCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
	return vr(temp);
}
//MACRO: @DOCUMENTSCOMMONDIR
vr mDOCUMENTSCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_DOCUMENTS, FALSE);
	return vr(temp);
}
//MACRO: @FAVORITESCOMMONDIR
vr mFAVORITESCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_FAVORITES, FALSE);
	return vr(temp);
}
//MACRO: @PROGRAMSCOMMONDIR
vr mPROGRAMSCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_PROGRAMS, FALSE);
	return vr(temp);
}
//MACRO: @STARTMENUCOMMONDIR
vr mSTARTMENUCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_STARTMENU, FALSE);
	return vr(temp);
}
//MACRO: @STARTUPCOMMONDIR
vr mSTARTUPCOMMONDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_COMMON_STARTUP, FALSE);
	return vr(temp);
}
//MACRO: @APPDATADIR
vr mAPPDATADIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_APPDATA, FALSE);
	return vr(temp);
}
//MACRO: @DESKTOPDIR
vr mDESKTOPDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_DESKTOP, FALSE);
	return vr(temp);
}
//MACRO: @FAVORITESDIR
vr mFAVORITESDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_FAVORITES, FALSE);
	return vr(temp);
}
//MACRO: @PROGRAMSDIR
vr mPROGRAMSDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_PROGRAMS, FALSE);
	return vr(temp);
}
//MACRO: @STARTMENUDIR
vr mSTARTMENUDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_STARTMENU, FALSE);
	return vr(temp);
}
//MACRO: @STARTUPDIR
vr mSTARTUPDIR()
{
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(HWND_DESKTOP, temp, CSIDL_STARTUP, FALSE);
	return vr(temp);
}
