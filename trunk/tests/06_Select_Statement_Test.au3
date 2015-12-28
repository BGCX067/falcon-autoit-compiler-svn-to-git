;TODO: Add test for continue case, and case else - DONE


$in = 222

;if $in = -1 then ConsoleWrite("LOLZ")

Select
	Case $in = 222
		Select
			Case $in = 222
				ConsoleWrite("YAY")
				ConsoleWrite("YAY")
			Case $in = 232
				ConsoleWrite("NAY")
			Case $in = 0
				ConsoleWrite("...")
			Case $in = 1
				ConsoleWrite("....")
		EndSelect
	Case $in = 232
		ConsoleWrite("NAY")
	Case $in = 0
		ConsoleWrite("...")
	Case $in = 1
		ConsoleWrite("....")
EndSelect

$in = 232



Select
	Case $in = 222
		ConsoleWrite("YAY")
		ContinueCase
	Case $in = 232
		ConsoleWrite("NAY")
		ContinueCase
	Case $in = 0
		ConsoleWrite("...")
		ContinueCase
	Case $in = 1
		ConsoleWrite("###")
EndSelect


$in = 0



Select
	Case $in = 222
		ConsoleWrite("YAY")
		ContinueCase
	Case $in = 232
		ConsoleWrite("NAY")
		ContinueCase
	Case $in = 0
		ConsoleWrite("...")
		ContinueCase
	Case $in = 1
		ConsoleWrite("###")
	Case Else
		ConsoleWrite("ELSE!")
EndSelect

$in = -88



Select
	Case $in = 222
		ConsoleWrite("YAY")
		ContinueCase
	Case $in = 232
		ConsoleWrite("NAY")
		ContinueCase
	Case $in = 0
		ConsoleWrite("...")
		ContinueCase
	Case $in = 1
		ConsoleWrite("###")
	Case Else
		ConsoleWrite("ELSE!")
EndSelect
ConsoleWrite("END")
ConsoleWrite("END2")
if $in=-88 then ConsoleWrite("END3")
