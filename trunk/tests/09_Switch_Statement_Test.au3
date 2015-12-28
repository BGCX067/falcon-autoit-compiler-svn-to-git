$val = 2

While $val < 18
	Switch $val
		Case 1
			ConsoleWrite("Should Never Fire.")
		Case 2
			ConsoleWrite("1")
		Case 3
			ConsoleWrite("2")
		Case 4
			ConsoleWrite("")
		Case 5
			ConsoleWrite("3")
		Case 6 to 12
			ConsoleWrite("6-12!")
		Case Else
			ConsoleWrite(">12 ")
	Endswitch
	$val += 1
Wend
