; Copyright 2002 Sergey Lebedev
; Licensed under the Apache License, Version 2.0

; DLL stub

.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib

extrn pascal GENERATE:proc
extrn pascal INIT:proc

.code
DllEntry proc hInstance:HINSTANCE, reason:DWORD, reserved1:DWORD
	.if reason==DLL_PROCESS_ATTACH
                call INIT
	.elseif reason==DLL_PROCESS_DETACH
	.elseif reason==DLL_THREAD_ATTACH
	.else        ; DLL_THREAD_DETACH
	.endif
	mov  eax,TRUE
	ret
DllEntry Endp

Generate proc Param1,Param2,Param3,Param4
	pushad

	mov  esi,Param1
	mov  esi,[esi]
	mov  ecx,Param2
	mov  edi,Param3

	call GENERATE

	mov  ecx,Param4
	mov  [ecx],eax

	popad
	ret	
Generate endp
End DllEntry