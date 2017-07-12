SAMPLE_RATE_           equ   22050    ; Частота сэмплирования


MYFONT   equ  1

		.386
		.model flat, stdcall

	IFDEF masm
		option casemap :none
	ENDIF

include c:\masm32\include\windows.inc
include win003.def

include ddraw.inc
include time.inc

		.code

		_warp999mrsulu:
include play.inc
		invoke timeGetTime
		mov INITTIME,eax

		 xor eax,eax
		 invoke CreateWindowEx,WS_EX_TOPMOST,ADDR MyClassName,ADDR MyClassName,\
	WS_POPUP or WS_VISIBLE,eax,eax,eax,eax,eax,eax,eax,eax
		mov Windowhandle,eax

		 invoke ShowWindow,eax,SW_SHOWDEFAULT

;do startup init crap
include startup.inc

		invoke ShowCursor,FALSE

;The most important place in a windows program: The Message Pump / Message Loop. 

MessagePumpStart:

		invoke timeGetTime
		sub eax,INITTIME
		mov TIME,eax

		call MainDrawingProcedure

		cmp eax,PART5_TIME
		ja  WM_QUIT_received

		cmp eax,PART1_TIME
		jb  @@_tt1
		mov  PARTNUM,2
@@_tt1:
		cmp eax,PART2_TIME
		jb  @@_tt0
		mov  PARTNUM,3
@@_tt0:

COMMENT*
		xor  eax,eax

		push eax
		push eax
		push eax
		push eax
		push offset MessageBuffer
		call PeekMessageA
		or eax,eax
		jz MessagePumpStart
*
		xor  eax,eax
		push eax
		push eax
		push eax
		push offset MessageBuffer
		call GetMessageA
		or eax,eax
		jz WM_QUIT_received
		invoke	GetAsyncKeyState,VK_ESCAPE
		or	eax,eax
		jnz	WM_QUIT_received

		jmp MessagePumpStart
		
WM_QUIT_received:
		include	release.inc
Exit:
		xor  eax,eax
		invoke ExitProcess,eax

		include blitproc.inc

		end _warp999mrsulu
