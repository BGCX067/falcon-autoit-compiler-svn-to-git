;Addition
$test_1 = 3
ConsoleWrite($test_1 & " ")
$test_1 = 0x3
ConsoleWrite($test_1 & " ")
$test_1 = "3"
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 + 0x3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 + 3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 + "3"
ConsoleWrite($test_1 & " ")
ConsoleWrite(@CRLF)
;Subtraction
$test_1 = $test_1 - 0x3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 - 3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 - "3"
ConsoleWrite($test_1 & " ")
ConsoleWrite(@CRLF)
;Multiplication
$test_1 = $test_1 * 0x3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 * 3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 * "3"
ConsoleWrite($test_1 & " ")
ConsoleWrite(@CRLF);
;Division
$test_1 = $test_1 / 0x3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 / 3
ConsoleWrite($test_1 & " ")
$test_1 = $test_1 / "3"
ConsoleWrite($test_1 & " ")
ConsoleWrite(@CRLF);
;
;Combination + Operator Precedence
$test_2 = 111245 + (52334 - 2423*2)/2
ConsoleWrite($test_2 & " ")
ConsoleWrite(@CRLF);
;
;Compound arthmetic operators
$test_3 = 10
ConsoleWrite($test_3 & " ")
$test_3 += 6
ConsoleWrite($test_3 & " ")
$test_3 /= 2
ConsoleWrite($test_3 & " ")
$test_3 *= 4
ConsoleWrite($test_3 & " ")
$test_3 -= 10
ConsoleWrite($test_3 & " ")
ConsoleWrite(@CRLF)

;Comparison operators
$test_4 = (not 2) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (1 and 4 and 3 or 2) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (1 and 0 or 1 or 2) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (1 = 1) + 1
ConsoleWrite($test_4 & " ")
$test_4 = ("a" = "a") + 1
ConsoleWrite($test_4 & " ")
$test_4 = ("a" = "A") + 1
ConsoleWrite($test_4 & " ")
$test_4 = ("a" == "a") + 1
ConsoleWrite($test_4 & " ")
$test_4 = ("a" == "A") + 1
ConsoleWrite($test_4 & " ")
$test_4 = ("c" > "a") + 1
ConsoleWrite($test_4 & " ")
$test_4 = (3 > 1) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (3 < 1) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (3 >= 1) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (3 >= 3) + 1
ConsoleWrite($test_4 & " ")
ConsoleWrite(@CRLF)
$test_4 = (3 <= 4) + 1
ConsoleWrite($test_4 & " ")
$test_4 = ( (($test_4 < $test_2) and True) and ($test_1 or ($test_3+1))) + 1
ConsoleWrite($test_4 & " ")
$test_4 = ( (($test_4 < $test_2) and False) and ($test_1 or ($test_3+1))) + 1
ConsoleWrite($test_4 & " ")
$test_4 = (($test_4 >= (5.5+$test_2))) + 1
ConsoleWrite($test_4 & " ")

$test_4 = ( 3 <> 54+1 ) + 1
ConsoleWrite($test_4 & " ")
$test_4 = ( 3 <> 3 ) + 1
ConsoleWrite($test_4 & " ")
$test_4 = ( True <> True ) + 1
ConsoleWrite($test_4 & " ")

;Bug #1 test for case: unary minus in compound expression
if Not($test_4 = -1) then ConsoleWrite("LOLZ")