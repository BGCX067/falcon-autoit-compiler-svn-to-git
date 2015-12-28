Global $lit = "THIS IS A TEST STRING."

;TODO: Add support for " in strings. AS IN:
;$lit &= "This "" is a test append"""

$lit &= "This 'is a test append'"
$maeo = "LOL"
$r = ConsoleWrite($lit & (" " &$lit & $maeo) & @crlf)
$r = $r + 20
ConsoleWrite($r & "::" & 2 & 0xff & @crlf)
