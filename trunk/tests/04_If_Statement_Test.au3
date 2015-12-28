;If Statement test

If 1 Or 1 And 1 == 1 >= 0.3434 or (1 And Not 4) Then ConsoleWrite("True" & @CRLF)

$test = True

If $test Then ConsoleWrite("True" & @CRLF)
If Not $test Then ConsoleWrite("True" & @CRLF)
If $test and (1 Or 0) Then ConsoleWrite("True" & @CRLF)
If Not $test and (1 Or 0) Then ConsoleWrite("True" & @CRLF)

$test = 2
$test += 4 * 6
$test2 = 50

If $test < $test2 and (True Or False) Then
	ConsoleWrite("$test < $test2" & @CRLF)
EndIf

$temp = $test2
$test2 = $test
$test = $temp

If $test > $test2 And Not False Then
	ConsoleWrite("$test > $test2 now" & @CRLF)
	If True Then
		If $test Then
			ConsoleWrite("Nested!" & @CRLF)
		EndIf
	EndIf
Else
	ConsoleWrite("Shouldnt fire" & @CRLF)
EndIf

If False Then
	ConsoleWrite("FAIL" & @CRLF)
ElseIf 1 == 1 Then
	ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
	If False Then
		ConsoleWrite("FAIL" & @CRLF)
	ElseIf 1 == 2 Then
		ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
	Else
		ConsoleWrite("Fire da lazors!" & @CRLF)
		If False Then
			ConsoleWrite("FAIL" & @CRLF)
		ElseIf 1 == 2 Then
			ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
		Else
			ConsoleWrite("Fire da lazors!" & @CRLF)
			If False Then
				ConsoleWrite("FAIL" & @CRLF)
			ElseIf 1 == 2 Then
				ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
			Else
				ConsoleWrite("Fire da lazors!" & @CRLF)
				If False Then
					ConsoleWrite("FAIL" & @CRLF)
				ElseIf 1 == 2 Then
					ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
				Else
					ConsoleWrite("Fire da lazors!" & @CRLF)
					If False Then
						ConsoleWrite("FAIL" & @CRLF)
					ElseIf 1 == 2 Then
						ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
					Else
						ConsoleWrite("Fire da lazors!" & @CRLF)
					EndIf

				EndIf

			EndIf

		EndIf
	EndIf

Else
	ConsoleWrite("Fire da lazors!" & @CRLF)
EndIf

If False Then
	ConsoleWrite("FAIL" & @CRLF)
ElseIf 1 == 2 Then
	ConsoleWrite("GUESS WHAT! 1 = 1!!!" & @CRLF)
Else
	ConsoleWrite("Fire da lazors!" & @CRLF)
EndIf