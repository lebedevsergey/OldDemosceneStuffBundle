del synth.dll
del synth_.obj
call _makesynth.bat
ML.EXE /Dmasm /c /Cp /coff /nologo  synth_.asm
LINK.EXE main.obj /DLL /DEF:synth_.def /NOLOGO synth_.obj /subsystem:windows
del *.exp
del *.lib

