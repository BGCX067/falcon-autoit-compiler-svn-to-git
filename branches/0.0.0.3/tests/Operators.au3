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