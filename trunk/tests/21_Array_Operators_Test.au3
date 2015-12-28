ConsoleWrite("Test :: Array Operations" & @CRLF & @CRLF)


$size = 500

;Initialize to an invalid amount
Global $testArray[$size]
;Re-initalise / copy newly sized array
$testArray = GetArray($size)

For $x = 0 to $size-1
	For $y = 0 to $size-1
		$testArray[$x][$y] = $x * $y
	Next
Next

For $a = 0 to $size-1 step 100
	For $b = 0 to $size-1 step 50
		ConsoleWrite($testArray[$a][$b] & @CRLF)
	Next
Next

ConsoleWrite("Test Complete." & @CRLF)


Func GetArray($size)
	local $testArray[$size][$size]
	return $testArray
EndFunc
