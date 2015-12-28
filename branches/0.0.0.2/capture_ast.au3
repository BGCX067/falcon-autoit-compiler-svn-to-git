$process = Run("falcone.exe", "", @SW_MAXIMIZE, 0x2)
$data = ""
while True
   $data &= StdoutRead($process)
   if @error then ExitLoop
WEnd
MsgBox(4096, @error, $data, 2)

FileWrite("output.txt", $data)