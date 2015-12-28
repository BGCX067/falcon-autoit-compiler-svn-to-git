$prev = 20
$step = 1

For $s = 0 to $prev step $step
	For $a = 0 to 900 step 1
		ConsoleWrite("")
	Next

	ConsoleWrite("I AM OUTPUTTING THIS VALUE: " & $s & @CRLF)
Next

ConsoleWrite("POST EXECUTION: " & $s & @CRLF)