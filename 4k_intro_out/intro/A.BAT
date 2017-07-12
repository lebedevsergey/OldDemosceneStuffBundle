del win003.*
call b.bat
c:\masm32\bin\ml.EXE /Dmasm /c /Cx /coff /Ic:\masm32\include\ /nologo  win003.asm
c:\masm32\bin\LINK.EXE /RELEASE /MERGE:.data=.text /NOLOGO win003.obj main.obj synth.obj c:\masm32\lib\kernel32.lib c:\masm32\lib\user32.lib c:\masm32\lib\winmm.lib c:\masm32\lib\gdi32.lib  /subsystem:windows /SECTION:.text,ERW /OPT:ref /STUB:./bin/dos_stub.exe
rem win003

