del synth.dll
del synth_.obj
del synth.obj
del main.obj
call b.bat
c:\masm32\bin\ML.EXE /Dmasm /c /Cp /coff /nologo  synth_.asm
c:\masm32\bin\LINK.EXE main.obj /DLL /DEF:synth_.def /NOLOGO synth_.obj /subsystem:windows
copy main.obj synth.obj
del *.exp
del *.lib

