#include <file.au3>
#include <GUIConstantsEx.au3>
#include <GuiListView.au3>
Opt("GUIOnEventMode", 1)


;states for flashing
$Is_Compiling = False
$flash_on = False

AdlibRegister("Flash", 2500)


$win = GUICreate("Compile Script", 600, 300, Default, Default, Default, 0x00000018)
GUISetOnEvent($GUI_EVENT_CLOSE, "CLOSEClicked")
GUISetOnEvent($GUI_EVENT_DROPPED, "IsDropped")
GUICtrlCreateLabel("Input: ", 5, 210)
$in = GUICtrlCreateInput("", 55, 208, 336)
GUICtrlSetState(-1, $GUI_DROPACCEPTED)
GUICtrlCreateLabel("Output: ", 5, 250)
$out = GUICtrlCreateInput("", 55, 248, 336)
$comp_button = GUICtrlCreateButton("Compile", 417, 208, 167, 60)
GUICtrlSetOnEvent($comp_button, "Compile")

$progress_bar = GUICtrlCreateProgress(30, 040, 550, 20)
$statuslab = GUICtrlCreateLabel("Status: Idle", 30, 018, 500)

GUICtrlCreateGroup("Optimization", 417, 065, 167, 130)
$no_opt = GUICtrlCreateRadio("None", 435, 92)
$st_opt = GUICtrlCreateRadio("Standard", 435, 122)
GUICtrlSetState($st_opt, $GUI_CHECKED)
$ma_opt = GUICtrlCreateRadio("Max", 435, 152)
GUICtrlCreateGroup("Options", 222, 065, 167, 130)
$skip_semantic_chk = GUICtrlCreateCheckbox("Skip Semantic Checking", 242, 92)
$compress_exe = GUICtrlCreateCheckbox("Compress EXE (UPX)", 242, 122)
GUICtrlSetState($compress_exe, $GUI_CHECKED)
$gen_AST = GUICtrlCreateCheckbox("Dump AST to file", 242, 152)
GUICtrlCreateGroup("Icon", 30, 065, 167, 130)
GUICtrlCreateLabel("No Preview available", 57, 124)
GUISetState()



While 1
	Sleep(100)
WEnd


Func Compile()
	$Is_Compiling = True
	$inscript = GUICtrlRead($in)
	GUICtrlSetData($progress_bar, 10)
	Build_Script($inscript, @ScriptDir & "\conglomeration.dump")

	If GUICtrlRead($skip_semantic_chk) = $GUI_CHECKED Then
	Else
		GUICtrlSetData($statuslab, "Status: Semantic Checking")
		GUICtrlSetData($progress_bar, 0)
		$pid = Run("Au3Check.exe " & """" & $inscript & """", @ScriptDir, @SW_HIDE, 0x2)
		;$pid = Run("Au3Check.exe " & """" & @ScriptDir & "\conglomeration.dump" & """", @ScriptDir, @SW_HIDE, 0x2)
		Local $errorresults = ""
		While Not @error
			$errorresults &= StdoutRead($pid)
			If @error Then ExitLoop
			Sleep(20)
		WEnd
		If Not StringInStr($errorresults, "- 0 error(s)") Then
			GUICtrlSetData($statuslab, "Abort: Fatal compilation error")
			GUICtrlSetData($progress_bar, 100)
			Return DisplayErrorResults($errorresults)
		EndIf
	EndIf

	GUICtrlSetData($statuslab, "Status: Lexing + Parsing conglomeration")
	GUICtrlSetData($progress_bar, 0)
	$pid = Run("falcone.exe" & " -v """ & @ScriptDir & "\conglomeration.dump" & """ -w output.cpp", @ScriptDir, @SW_HIDE, 0x8)
	;$pid = Run("../falcone.exe" & " -v """ & @ScriptDir & "\conglomeration.dump" & """ -w output.cpp", @ScriptDir, @SW_HIDE, 0x8)
	Local $errorresults = ""
	While Not @error
		$errorresults &= StdoutRead($pid)
		If @error Then ExitLoop
		Sleep(20)
	WEnd
	If StringInStr($errorresults, "error:") Then
		GUICtrlSetData($statuslab, "Abort: Fatal internal error (frontend)")
		GUICtrlSetData($progress_bar, 100)
		Return DisplayErrorResults($errorresults, "Internal Error: (frontend)")
	EndIf

	GUICtrlSetData($statuslab, "Status: Final Stage Compilation")
	GUICtrlSetData($progress_bar, 0)
	$fhnd = FileOpen("compile.bat", 2)
	;FileWrite($fhnd, "PATH=C:\falcon\mingw\bin"&@CRLF)
	FileWrite($fhnd, "C:\FAC\mingw\bin\"&@CRLF)
	FileWrite($fhnd, "g++ -D BUILD_WINDOWS -fpermissive -o output -s")
	if GUICtrlRead($st_opt) = $GUI_CHECKED then FileWrite($fhnd, " -O2")
	if GUICtrlRead($ma_opt) = $GUI_CHECKED then FileWrite($fhnd, " -O3")
	FileWrite($fhnd, " -static-libgcc -static-libstdc++ output.cpp -lshlwapi")
	FileClose($fhnd)
	$pid = Run("compile.bat", @ScriptDir, @SW_HIDE, 0x8)
	Local $errorresults = ""
	While Not @error
		$errorresults &= StdoutRead($pid)
		If @error Then ExitLoop
		Sleep(20)
	WEnd
	If StringInStr($errorresults, "error:") Then
		GUICtrlSetData($statuslab, "Abort: Fatal internal error (MinGW backend)")
		GUICtrlSetData($progress_bar, 100)
		Return DisplayErrorResults($errorresults, "Internal Error: (MinGW Backend)")
	EndIf


	If GUICtrlRead($compress_exe) = $GUI_CHECKED Then
		GUICtrlSetData($statuslab, "Status: Compressing EXE...")
		GUICtrlSetData($progress_bar, 00)
		$pid = Run("upx.exe -v --best --compress-icons=0 """ & @ScriptDir & "\output.exe""", @ScriptDir, @SW_HIDE, 0x8)
		Local $errorresults = ""
		While Not @error
			$errorresults &= StdoutRead($pid)
			If @error Then ExitLoop
			Sleep(20)
		WEnd
	EndIf

	FileCopy(@ScriptDir & "\output.exe", GUICtrlRead($out))
	GUICtrlSetData($statuslab, "Status: Compilation finished successfully.")
	GUICtrlSetData($progress_bar, 100)
EndFunc   ;==>Compile


Func DisplayErrorResults($output, $title="Build Errors (Thanks to Tylo)")
	;get rid of ugly banner
	$spl = StringSplit($output, @CRLF, 1)
	$outpute = ""
	For $x = 2 To $spl[0]
		$outpute &= $spl[$x] & @CRLF
	Next
	global $win2 = GUICreate($title, 500, 250)
	GUISetOnEvent($GUI_EVENT_CLOSE, "CloseBuildWin")
	Local $outed = GUICtrlCreateEdit($outpute, 2, 2, 496, 246)
	GUICtrlSetBkColor(-1, 0xFFEEEE)
	WinSetOnTop($title, "", 1)
	GUICtrlCreateButton("", 600, 200)
	GUICtrlSetState(-1, $GUI_FOCUS)
	GUISetState()
EndFunc   ;==>DisplayErrorResults


Func CloseBuildWin()
	GUIDelete($win2)
EndFunc


Func IsDropped()
	$Is_Compiling = True
	GUICtrlSetData($statuslab, "Status: Idle")
	If GUICtrlRead($out) == "" Then
		local $path = GUICtrlRead($in)
		Local $path_slash_location = StringInStr($path, "\", 0, -1)
		GUICtrlSetData($out, StringTrimRight($path, StringLen($path) - $path_slash_location) & "output.exe")
	endif
EndFunc   ;==>IsDropped

Func Flash()
	If Not $Is_Compiling Then
		If $flash_on Then
			GUICtrlSetData($statuslab, "Status: Idle")
			$flash_on = False
		Else
			GUICtrlSetData($statuslab, "            Please drag and drop the script to compile into the input field.")
			$flash_on = True
		EndIf
	EndIf
EndFunc   ;==>Flash


Func Build_Script($path, $dump)
	If FileExists($dump) Then FileDelete($dump)
	Global $includeArray[1]
	Include_Includes_Dump($path, $dump, $includeArray)
	$includeArray = 0
EndFunc   ;==>Build_Script



;============================================================================================================
;						Dont interact with these functions plz. PRIVATE
;============================================================================================================


Func Include_Includes_Dump($path, $dump, ByRef $incarray)
	;first lets see if we have already included this file, to prevent infinite recursion loops and an eventual crash.
	For $a = 0 To UBound($incarray) - 1 Step 1
		If $incarray[$a] = StringReplace($path, "/", "\") Then
			Return ;note that the message that it is 'building through the include' will still appear even though we dont include it.
		EndIf

	Next



	;File has not been included, so lets add it to the list of included files.
	ReDim $incarray[UBound($incarray) + 1]
	$incarray[UBound($incarray) - 1] = StringReplace($path, "/", "\")

	Local $dumphnd = $dump
	Local $File_Array = Robust_File_Read_To_Array($path)
	For $e = 1 To $File_Array[0] Step 1
		$Include = StringInStr($File_Array[$e], "#include")
		GUICtrlSetData($progress_bar, ($e / $File_Array[0]) * 100)

		If $Include = 1 Then
			Local $sTemp = StringTrimLeft(StringTrimLeft($File_Array[$e], $Include - 1), 8)
			If StringLeft($sTemp, 1) = " " Then $sTemp = StringTrimLeft($sTemp, 1)
			Local $filename = StringReplace(StringReplace(StringReplace(StringReplace($sTemp, '"', ""), "'", ""), '<', ""), '>', "")
			If StringUpper($filename) = "-ONCE" Then;For the #include-once
				ContinueLoop
			ElseIf StringRight($sTemp, 1) <> "'" And StringRight($sTemp, 1) <> '"' And StringRight($sTemp, 1) <> ">" Then
				ContinueLoop
			EndIf
			Local $s_Au3Path = RegRead64('HKLM\Software\AutoIt v3\AutoIt', 'InstallDir')
			$filename = StringReplace($filename, "/", "\")
			$path = StringReplace($path, "/", "\")
			Local $slash_location = StringInStr($filename, "\", 0, -1)
			Local $path_slash_location = StringInStr($path, "\", 0, -1)
			;Find the include that they are after...
			If FileExists(StringTrimRight($path, StringLen($path) - $path_slash_location) & $filename) Then
				Include_Includes_Dump(StringTrimRight($path, StringLen($path) - $path_slash_location) & $filename, $dumphnd, $incarray)
				GUICtrlSetData($statuslab, "Status: Building (" & $filename & ")")
			ElseIf FileExists($s_Au3Path & "/Include/" & $filename) Then ;Standard Include file.
				Include_Includes_Dump($s_Au3Path & "/Include/" & $filename, $dumphnd, $incarray)
				GUICtrlSetData($statuslab, "Status: Building (" & $filename & ")")
			ElseIf FileExists(@WorkingDir & "/" & $filename) Then ;Local include to the mainfile itself.
				Include_Includes_Dump(@WorkingDir & "/" & $filename, $dumphnd, $incarray)
				GUICtrlSetData($statuslab, "Status: Building (" & $filename & ")")
			Else
				MsgBox(4096, "ERROR", @CRLF & "!> ERROR! " & $filename & "(Line: " & $e & ") Unable to find Include File." & @CRLF)
				Exit
			EndIf
		Else
			;Lets check for comment sections.
			If StringLeft($File_Array[$e], 3) = "#cs" Or StringLeft($File_Array[$e], 15) = "#comments-start" Then
				;If a comment section LOOP till its over.
				For $n = ($e + 1) To $File_Array[0] Step 1

					If StringLeft($File_Array[$n], 3) = "#ce" Or StringLeft($File_Array[$n], 13) = "#comments-end" Then

						$e = $n
						ExitLoop
					EndIf

				Next
			Else
				FileWrite($dumphnd, $File_Array[$e] & @CRLF)
			EndIf

		EndIf
	Next
EndFunc   ;==>Include_Includes_Dump



;==================================================================================================
;Wrapper Functions.
Func Robust_File_Read_To_Array($filepath)
	Local $count = _FileCountLines($filepath)
	Local $filehnd = FileOpen($filepath)
	If $filehnd = -1 Then
		MsgBox(4096, "ERROR", "-> ERROR! Failed to open file " & $filepath & @CRLF)
		Exit
	EndIf

	Local $array[$count + 1]
	$array[0] = 0
	While True
		$line = FileReadLine($filehnd)
		If @error <> -1 Then
			;ConsoleWrite("LINE: " & $line & @CRLF)
			If $line = "" Then
				ContinueLoop
			Else
				$array[0] += 1
			EndIf

			$array[$array[0]] = $line
		Else
			FileClose($filehnd)
			Return $array
		EndIf
	WEnd
	FileClose($filehnd)
	Return $array
EndFunc   ;==>Robust_File_Read_To_Array

;Stole it from Autoit Updater. If I knew who made it I would give credit. Lemme know if you know and I will update.
Func RegRead64($sKeyname, $sValue)
	Local $res = RegRead($sKeyname, $sValue)
	If @error And @AutoItX64 Then
		$sKeyname = StringReplace($sKeyname, "HKEY_LOCAL_MACHINE", "HKLM")
		$sKeyname = StringReplace($sKeyname, "HKLM\SOFTWARE\", "HKLM\SOFTWARE\Wow6432Node\")
		$res = RegRead($sKeyname, $sValue)
		If @error Then
			SetError(1)
			Return ""
		EndIf
	EndIf
	SetError(0)
	Return $res
EndFunc   ;==>RegRead64

Func CLOSEClicked()
	if FileExists(@ScriptDir & "/conglomeration.dump") then FileDelete(@ScriptDir & "/conglomeration.dump")
	if FileExists(@ScriptDir & "/output.cpp") then FileDelete(@ScriptDir & "/output.cpp")
	if FileExists(@ScriptDir & "/compile.bat") then FileDelete(@ScriptDir & "/compile.bat")
	Exit
EndFunc   ;==>CLOSEClicked