
Global $iterate[20]


For $x = 0 to 19
	$iterate[$x] = "Value: " & $x & @CRLF
Next


$input = $iterate

For $gdfgdf in $input
		ConsoleWrite($gdfgdf)
		For $gdfgdf in $input
			ContinueLoop
			ConsoleWrite($gdfgdf)
		Next
		Sleep(10)
Next
