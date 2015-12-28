
$globalvar = 8

Func TestPrint($input)
	ConsoleWrite($input)
Endfunc

Func TestRetVal()
	return 69;LOL!
Endfunc

Func TestMultiArgs($fdff, $rsfjh, $fdgfdf)
	ConsoleWrite($fdff + $rsfjh + $fdgfdf + $globalvar)
Endfunc


TestPrint("Testing Print" & @CRLF)
COnsoleWrite("Correct Return Value: " & (1+(TestRetVal() == 69)) & @CRLF)

COnsoleWrite(@CRLF & TestMultiArgs(3,2,6))
