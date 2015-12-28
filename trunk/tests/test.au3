#include <GUIConstantsEx.au3>
#include <GuiListView.au3>
Opt("GUIOnEventMode", 1)

;Stores the tests.
Global $tests[1][2], $count = 0


$win = GUICreate("FALCON Regression Test", 480, 400)
GUISetOnEvent($GUI_EVENT_CLOSE, "CLOSEClicked")


$testview = GUICtrlCreateListView("# | Regression Name | Status", 30, 30, 420, 200)
_GUICtrlListView_SetColumnWidth($testview, 1, 310)
_GUICtrlListView_SetColumnWidth($testview, 2, $LVSCW_AUTOSIZE_USEHEADER)

$output_lab = GUICtrlCreateLabel("Compilation Output:", 32, 243)
$output_edit = GUICtrlCreateEdit("", 30, 260, 420, 120)

$firstchk = FileFindFirstFile("*.au3")
While 1
	$file = FileFindNextFile($firstchk)
	If @error Then ExitLoop
	If $file = "test.au3" Then ContinueLoop
	$tests[$count][0] = GUICtrlCreateListViewItem($count & "|" & StringTrimRight($file, 4) & "|Queued", $testview)
	$tests[$count][1] = $file
	$count += 1
	ReDim $tests[$count + 1][2]
WEnd
GUISetState()


;Test the output against each other to see if they match
Local $s_Au3Path = RegRead64('HKLM\Software\AutoIt v3\AutoIt', 'InstallDir')
For $x = 0 To UBound($tests) - 1 Step 1
	$output_interpreter = ""
	If $tests[$x][1] = "test.au3" Then ContinueLoop
	If $tests[$x][1] = "" Then ContinueLoop
	GUICtrlSetData($tests[$x][0], ($x) & "|" & StringTrimRight($tests[$x][1], 4) & "|Running")

	GUICtrlSetData($output_edit, "---> Running Regression test " & $tests[$x][1] & @CRLF, "append")
	$pid = Run($s_Au3Path & "/Autoit3.exe" & " """ & @ScriptDir & "/" & $tests[$x][1] & """", @WorkingDir, @SW_MAXIMIZE, 0x2)
	While Not @error
		$latest = StdoutRead($pid)
		If @error Then ExitLoop
		$output_interpreter &= $latest
		GUICtrlSetData($output_edit, $latest, "append")
	WEnd

	GUICtrlSetData($output_edit, @CRLF & "Invoking Compilation:" & @CRLF, "append")
	$output_compiler = ""
	$pid = Run("../falcone.exe" & " -v """ & @ScriptDir & "\" & $tests[$x][1] & """ -w output.cpp", @WorkingDir, @SW_HIDE, 0x2)
	While Not @error
		$latest = StdoutRead($pid)
		If @error Then ExitLoop
		$output_compiler &= $latest
		GUICtrlSetData($output_edit, $latest, "append")
	WEnd
	GUICtrlSetData($output_edit, @CRLF, "append")
	ShellExecuteWait("compile.bat", "", @ScriptDir, Default, @SW_HIDE)

	$output_falcon = ""
	$pid = Run("output.exe", @WorkingDir, @SW_HIDE, 0x2)
	While Not @error
		$latest = StdoutRead($pid)
		If @error Then ExitLoop
		$output_falcon &= $latest
		GUICtrlSetData($output_edit, $latest, "append")
	WEnd
	GUICtrlSetData($output_edit, @CRLF, "append")

	;Check if output the same.
	If StringReplace($output_interpreter, @CR, "") = StringReplace($output_falcon, @CR, "") Then
		GUICtrlSetData($tests[$x][0], ($x) & "|" & StringTrimRight($tests[$x][1], 4) & "|Passed")
		GUICtrlSetBkColor($tests[$x][0], 0x44FF44)
	Else
		GUICtrlSetData($tests[$x][0], ($x) & "|" & StringTrimRight($tests[$x][1], 4) & "|Failed")
		GUICtrlSetBkColor($tests[$x][0], 0xFF4444)
	EndIf

Next



While 1
	Sleep(100)
WEnd

Func CLOSEClicked()
	Exit
EndFunc   ;==>CLOSEClicked


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

