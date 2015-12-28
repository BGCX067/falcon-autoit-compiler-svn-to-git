#RequireAdmin
#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Outfile_x64=compiler.exe
#AutoIt3Wrapper_Res_requestedExecutionLevel=asInvoker
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#include <file.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <EditConstants.au3>
#include <GDIPlus.au3>
#include <WinAPI.au3>
#include <GuiListView.au3>
#include "_Zip.au3"


;Program specific information
FileInstall("C:\falcon\trunk\tools\FALCON label.png", @AppDataDir & "/falconimg.png")
$version = "0.5.0"
$releasetype = "Alpha"
$download1url = "http://falcon-autoit-compiler.googlecode.com/files/MinGW-base-0.1.0.zip"
$download2url = "http://falcon-autoit-compiler.googlecode.com/files/falconcore-0.5.zip"
$liscense = "Copyright (C) 2012 Twitchyliquid64" & @CRLF
$liscense &= "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ""Software""), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:" & @CRLF & @CRLF
$liscense &= "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software." & @CRLF & @CRLF
$liscense &= "THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."

;Program vars
$state = 0

Opt("GUIOnEventMode", 1)
$win = GUICreate("Falcon Installer", 800, 600, Default, Default, Default, 0x00000018)
GUISetOnEvent($GUI_EVENT_CLOSE, "CLOSEClicked")

_GDIPlus_Startup()
$hImage = _GDIPlus_ImageLoadFromFile(@AppDataDir & "/falconimg.png")
$hGraphic = _GDIPlus_GraphicsCreateFromHWND($win)
$topborder = GUICtrlCreateLabel("", 0, 192, 800, 1)
GUICtrlSetBkColor($topborder, 0x000000)
$steptitle = GUICtrlCreateLabel("Introduction", 120, 200, 400, 30)
GUICtrlSetFont( $steptitle, 20, 10)


$infolabel = GUICtrlCreateLabel("", 150, 250,  600, 260)
GUICtrlSetFont( $infolabel, 10, 10)

$infoedit = GUICtrlCreateEdit($liscense, 150, 270, 600, 220, $ES_AUTOVSCROLL)
GUICtrlSetState($infoedit, $GUI_HIDE)

$progress = GUICtrlCreateProgress(150, 235, 600, 25)
GUICtrlSetState($progress, $GUI_HIDE)

$bottomborder = GUICtrlCreateLabel("", 0, 510, 800, 1)
GUICtrlSetBkColor($bottomborder, 0x000000)
$nextbutton = GUICtrlCreateButton("Next", 600, 530, 175, 50)
GUICtrlSetOnEvent($nextbutton, "Advance")
$backbutton = GUICtrlCreateButton("Previous", 400, 530, 175, 50)
GUICtrlSetOnEvent($backbutton , "GoBack")
$copyright = GUICtrlCreateLabel("(c) Twitchyliquid64 2012", 50, 550)

GUIRegisterMsg($WM_PAINT, "MY_WM_PAINT")
GUISetState()
UpdateWindow()


Func Advance()
	$state += 1
	UpdateWindow()
EndFunc

Func GoBack()
	$state -= 1
	if $state < 0 then $state = 0
	UpdateWindow()
EndFunc


Func UpdateWindow()
	Switch $state
		Case 0
			GUICtrlSetData($steptitle, "Introduction")
			GUICtrlSetData($infolabel, "You are about to install the Falcon compiler suite. Click 'Next' to proceed." & @CRLF & @CRLF & @CRLF & @TAB & "Package Version: " & $version & @CRLF & @CRLF & @TAB & "Release Type: " & $releasetype)
			GUICtrlSetState($infoedit, $GUI_HIDE)
			GUICtrlSetState($infolabel, $GUI_SHOW)
			GUICtrlSetData($nextbutton, "Next")
			GUICtrlSetState($progress, $GUI_HIDE)
			GUICtrlSetState($nextbutton, $GUI_ENABLE)
			GUICtrlSetState($backbutton, $GUI_DISABLE)
		Case 1
			GUICtrlSetData($steptitle, "Licensing")
			GUICtrlSetData($nextbutton, "I Accept this license")
			GUICtrlSetData($infoedit, $liscense)
			GUICtrlSetState($infolabel, $GUI_HIDE)
			GUICtrlSetState($infoedit, $GUI_SHOW)
			GUICtrlSetState($progress, $GUI_HIDE)
			GUICtrlSetState($nextbutton, $GUI_ENABLE)
			GUICtrlSetState($backbutton, $GUI_ENABLE)
		Case 2
			GUICtrlSetData($steptitle, "Download")
			GUICtrlSetState($infoedit, $GUI_HIDE)
			GUICtrlSetState($infolabel, $GUI_SHOW)
			GUICtrlSetData($nextbutton, "Next")
			GUICtrlSetState($progress, $GUI_HIDE)
			GUICtrlSetState($nextbutton, $GUI_ENABLE)
			GUICtrlSetState($backbutton, $GUI_ENABLE)
			GUICtrlSetData($infolabel, "Clicking Next will download required files for FALCON from the hosting provider. This includes:" & @CRLF & @CRLF & @TAB & "MinGW backend (approx. 80mb)" & @CRLF & @TAB & "Falcon core files (approx. 4mb)")
		Case 3
			GUICtrlSetData($steptitle, "Downloading ... Please Wait.")
			GUICtrlSetState($infoedit, $GUI_SHOW)
			GUICtrlSetState($infolabel, $GUI_HIDE)
			GUICtrlSetData($nextbutton, "Next")
			GUICtrlSetState($progress, $GUI_SHOW)
			GUICtrlSetState($nextbutton, $GUI_DISABLE)
			GUICtrlSetState($backbutton, $GUI_DISABLE)
			GUICtrlSetData($infoedit, "Preparing to download...")
			$download = InetGet ( $download1url ,@AppDataDir & "/falcon_download1.zip", 1, 1)
			GUICtrlSetData($infoedit, @CRLF & "Downloading: " & $download1url, "append")
			While not InetGetInfo($download, 2)
				Sleep(300)
				GUICtrlSetData($progress, (InetGetInfo($download, 0)/InetGetInfo($download, 1))*100)
			WEnd
			$download = InetGet ( $download2url ,@AppDataDir & "/falcon_download2.zip", 1, 1)
			GUICtrlSetData($infoedit, @CRLF & "Downloading: " & $download2url, "append")
			While not InetGetInfo($download, 2)
				Sleep(300)
				GUICtrlSetData($progress, (InetGetInfo($download, 0)/InetGetInfo($download, 1))*100)
			WEnd
			GUICtrlSetData($steptitle, "Installing ... Please Wait.")
			GUICtrlSetData($infoedit, @CRLF & "Unpacking: " & @AppDataDir & "/falcon_download1.zip", "append")
			Sleep(1000)
			DirCreate(@HomeDrive & "/FAC")
			FileCopy(@AppDataDir & "/falcon_download1.zip", @HomeDrive & "/FAC/cppprovider.zip")
			FileCopy(@AppDataDir & "/falcon_download2.zip", @HomeDrive & "/FAC/compiler_core.zip")
			GUICtrlSetState($nextbutton, $GUI_ENABLE)
		Case 4
			GUICtrlSetData($steptitle, "Unpacking ... FAILED")
			GUICtrlSetState($infoedit, $GUI_HIDE)
			GUICtrlSetState($infolabel, $GUI_SHOW)
			GUICtrlSetData($nextbutton, "Finish")
			GUICtrlSetState($progress, $GUI_HIDE)
			GUICtrlSetState($nextbutton, $GUI_ENABLE)
			GUICtrlSetState($backbutton, $GUI_DISABLE)
			GUICtrlSetData($infolabel, "The installer failed to unpack the downloaded files. This is what you must do to finish the installation:" & @CRLF & "1. Go to (Homedrive):\FAC (normally C:\FAC). This is where everything is stored." & @CRLF & "2. Unpack the two archives into (homedrive):\FAC." & @CRLF & "3. Lastly, test the install by going into /tests and running test.exe. If any fail, try again. If they fail multiple times, bug report plz." _
			& @CRLF & @CRLF & "This concludes installation. Please see the forum to see how to build your scripts using FAC.")
		Case 5
			FileDelete(@AppDataDir & "/falcon_download2.zip")
			FileDelete(@AppDataDir & "/falcon_download1.zip")
			FileDelete(@AppDataDir & "/falconimg.png")
			_GDIPlus_GraphicsDispose($hGraphic)
			_GDIPlus_ImageDispose($hImage)
			_GDIPlus_Shutdown()
			Exit


	EndSwitch
EndFunc






While 1
	Sleep(100)
WEnd

; Draw PNG image
Func MY_WM_PAINT($hWnd, $msg, $wParam, $lParam)
    #forceref $hWnd, $Msg, $wParam, $lParam
    _WinAPI_RedrawWindow($win, 0, 0, $RDW_UPDATENOW)
    _GDIPlus_GraphicsDrawImageRect($hGraphic, $hImage, 16,10, 772/1, 200/1.15)
    _WinAPI_RedrawWindow($win, 0, 0, $RDW_VALIDATE)
    Return $GUI_RUNDEFMSG
EndFunc   ;==>MY_WM_PAINT

Func CLOSEClicked()
	; Clean up resources
	FileDelete(@AppDataDir & "/falcon_download2.zip")
	FileDelete(@AppDataDir & "/falcon_download1.zip")
	FileDelete(@AppDataDir & "/falconimg.png")
	_GDIPlus_GraphicsDispose($hGraphic)
	_GDIPlus_ImageDispose($hImage)
	_GDIPlus_Shutdown()
	Exit
EndFunc   ;==>CLOSEClicked