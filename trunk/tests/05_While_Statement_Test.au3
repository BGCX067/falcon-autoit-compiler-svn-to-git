;While Statement Test

$counter = 0

While $counter < 106
	ConsoleWrite(".")
	$counter += 1
WEnd
ConsoleWrite($counter & @CRLF)

$counter = 0
;Large test
While $counter < 200000
	$counter += 1
WEnd
ConsoleWrite(($counter/2) & @CRLF)

;String comp test.
$counter = "0"

While $counter <> "50"
	$counter = (($counter+1) & "")
WEnd
ConsoleWrite($counter & @CRLF)

;Exitloop and continueloop test
$counter = 0
While $counter < 99999
	if $counter > 500 then ExitLoop
	$counter += 1
	if $counter > 40 Then
		ContinueLoop
		ConsoleWrite("#")
	EndIf

	ConsoleWrite(".")
WEnd


ConsoleWrite($counter & @CRLF)