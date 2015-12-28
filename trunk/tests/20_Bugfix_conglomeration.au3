
;Bugfix #1
Func Test_SymBackPropergation()
	If 1 Then
		$e = 1
	ElseIf 4 Then
		$s = 1
	Else
		$g = 1
	EndIf

	Select
		Case 1
			$r = 3
		Case 2
			$o = 2
	EndSelect

	Switch 1
		Case 1
			$q = 4
		Case 4
			$u = 5
	EndSwitch


EndFunc

Test_SymBackPropergation()
ConsoleWrite("#1" & @CRLF)