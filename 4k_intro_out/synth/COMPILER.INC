; synth script compiler

.code
locals

NOTE_PITCH_SYM         equ  'p'
NOTE_SUSTAIN_SYM       equ  's'
NOTE_PITCH_SUSTAIN_SYM equ  'q'
EMPTY_NOTE_SYM         equ  'ENON'

SEPARATOR   equ  ' '
SEPARATOR2  equ  ','
CR 	    equ  0dh
LF 	    equ  0ah

REM_SYM     equ  ';'
DECIMAL_DOT equ  '.'
MINUS_SYM   equ  '-' 

TOKEN_MAX_LEN   equ  300

CHK_FILE_ERR_ equ 1

Percent2	equ	255

Chk_File_Err macro msg
local Err_Msg
IFDEF CHK_FILE_ERR_
.DATA
Err_Msg  db  0Dh,0Ah,,msg,0Dh,0Ah,'$'
.CODE
lea edx,Err_Msg
jc sound_Err
ELSE
Err_Msg:
ENDIF
endm

upcase2  macro par
	local @@No_Upcase
	cmp par,'a'
	jb  @@No_Upcase
	cmp par,'z'
	ja  @@No_Upcase
	and par,0DFh	
@@No_Upcase:		
endm


TOKEN_STRUC   struc
Token_Str_Ptr dd  ?
Token_Proc_Ptr dd  ?
Token_Com  db ?
ends



.data?
Script_End  dd ?
Cur_Script_Ptr  dd  ?
Cur_Inner_Script_Ptr  dd  ?
Script_String_Counter dd  ?
Token_Buf   db TOKEN_MAX_LEN dup (?)

.code
Compile		proc 
; ecx - text script len
; esi - ptr to text script
; edi - ptr to buf for inner script
      mov Cur_Inner_Script_Ptr,edi

      mov Script_String_Counter,0	

      mov eax,ecx
      add eax,esi
      mov Script_End,eax

@@Comp_Cyc:

;        call Surp_Separator
 ;       jc @@Script_End	

  ;      call Surp_Rem
   ;     jc @@Script_End	

        lea edi,Token_Buf	
        call Get_One_Token
        jc @@Script_End	
	mov byte ptr [edi],0

        lea edi,Token_Buf	
	lea ebp,Token_Tab
        	
call Surp_Rem
	mov Cur_Script_Ptr,esi
        call Translate_Token
call Surp_Rem
	mov esi,Cur_Script_Ptr

        cmp esi,Script_End
        jbe @@Comp_Cyc 

@@Script_End:

        retn
endp



Get_One_Token      proc    

;		
; esi: Script
; edi: token buf
		mov dword ptr [edi],0

        	call Surp_Separator
        	jc @@Script_End	

        	call Surp_Rem
        	jc @@Script_End	

		mov ecx,TOKEN_MAX_LEN
@@tt1:
		lodsb
		cmp al,SEPARATOR
		jz  @@tt2
		cmp al,SEPARATOR2
		jz  @@tt2
		cmp al,CR
		jz  @@tt4
		cmp al,LF
		jnz  @@tt3
		cmp al,REM_SYM
		jnz  @@tt3
@@tt4:
		inc Script_String_Counter
		jmp  @@tt2
@@tt3:
		cmp esi,Script_End
                ja  @@Script_End

		stosb

		loop @@tt1

@@tt2:
		clc

@@Script_End:
                retn
		endp


Surp_Separator proc 

		jmp @@Param_End
@@tt5:
		inc esi
@@Param_End:
		cmp esi,Script_End
                ja  @@Script_End

		cmp byte ptr[esi],CR
                jz  @@tt6
		cmp byte ptr[esi],LF
                jnz  @@tt7
@@tt6:
		inc Script_String_Counter
		jmp  @@tt5
@@tt7:
		cmp byte ptr[esi],SEPARATOR
		jz  @@tt5
		cmp byte ptr[esi],SEPARATOR2
		jz  @@tt5

		clc

@@tt3:
		retn
@@Script_End:
		stc
		jmp @@tt3
		endp

Surp_Rem 	proc 

@@Next_Rem:
		cmp byte ptr [esi],REM_SYM
		jz @@tt5
		clc
		jmp @@ex
@@tt5:
		lodsb
		cmp al,CR
                jz  @@ex2
		cmp al,LF
                jz  @@ex2
		jmp @@tt5
@@ex2:
		call Surp_Separator
		jmp @@Next_Rem
@@ex:
		retn
endp


;-----------------------------------------------------------
; StrLength Подсчитать в строке количество ненулевых символов
;-----------------------------------------------------------
; Вход:
;       di = адрес строки (s)
; Выход:
;       cx = количество ненулевых символов в s
; Регистры:
;       cx
;-----------------------------------------------------------
Str_Len proc
        push    eax
        push    edi

        xor     al, al
        mov     ecx, 0FFFFFFFFh
        cld
        repnz   scasb
        not     ecx
        dec     ecx

        pop     edi
        pop     eax
        ret
ENDP


Str_Cmp      proc    	
; esi - str1
; edi - str2
; ecx - str2 len
		pushad
		
		rep cmpsb		
;		or  ecx,ecx

		popad
		retn
	        endp	

Str_Upcase      proc    	
; edi - str
		pushad
@@tt1:		
		mov al,[edi]
		or al,al
		jz @@tt2
		upcase2 al
		stosb
		jmp @@tt1

@@tt2:
		popad
		retn
	        endp	



Translate_Token      proc    		
; edi: token buf
; ebp: cur token tab
		pushad
		xor ebx,ebx
@@tt1:
                cmp [ebx+ebp].Token_Str_Ptr,NULL
		jz @@Unknown_Token
		mov esi,[ebx+ebp].Token_Str_Ptr

		call Str_Upcase
		call Str_Len
		call Str_Cmp
		jz @@Known_Token

		add ebx,SIZE TOKEN_STRUC
		jmp @@tt1

@@Known_Token:
		mov eax,[ebx+ebp].Token_Proc_Ptr	
		call eax

		popad
                retn
@@Unknown_Token:
		.CODE
		lea edx,Err_Msg6
		jmp Compiler_Err
		.DATA
Err_Msg6  db  'Unknown token',0Dh,0Ah,'$'
		endp

.code
Get_Number      proc    		
; esi: script buf
; ebp: cur token tab
		pushad
		fld1
		fistp Sign_Muller

		xor ebx,ebx

	        mov esi,Cur_Script_Ptr

                lea edi,Token_Buf	
                call Get_One_Token
	        jc @@Not_A_Num

	        mov Cur_Script_Ptr,esi

                lea esi,Token_Buf	

		fldz
@@tt1:
		xor eax,eax
		lodsb
		cmp al,MINUS_SYM
		jz @@Change_Sign

		cmp al,DECIMAL_DOT
		jz @@Float_Num
		cmp al,30h
		jb @@Not_A_Num
		cmp al,39h
		ja @@Not_A_Num
		sub al,30h
		mov Tmp1,ax
		fimul Decimal_Muller
		fiadd Tmp1
		cmp esi,edi
		jb  @@tt1

	        mov esi,Cur_Script_Ptr
		call Surp_Separator
	        mov Cur_Script_Ptr,esi

		fimul Sign_Muller

		popad
                retn

@@Float_Num:
		fld  Decimal_Dot_Muller
		fldz
@@tt1_:
		xor eax,eax
		lodsb
		cmp al,30h
		jb @@Not_A_Num
		cmp al,39h
		ja @@Not_A_Num
		sub al,30h
		mov Tmp1,ax
		fild Tmp1
		fmul st,st(2)
		faddp st(1),st

		fxch st(1)
		fmul Decimal_Dot_Muller
		fxch st(1)

		cmp esi,edi
		jb  @@tt1_
		fxch st(1)
		fstp st

		faddp st(1),st

		fimul Sign_Muller

		popad
                retn

@@Change_Sign:   
		neg Sign_Muller
		jmp @@tt1
@@Not_A_Num:
		.CODE
		lea edx,Err_Msg9
		jmp Compiler_Err

		.DATA
Sign_Muller       dw  1
Decimal_Muller    dw  10
Decimal_Dot_Muller    dd  0.100000000000000000000
Err_Msg9  db  'Wrong number',0Dh,0Ah,'$'
Err_Msg_Space  db 0Dh,0Ah,'$'
		endp

.data
Num_Line_Msg db 'At line :    '
Num_Line_Msg_End:
			db 0Dh,0Ah,'$'

.code
Compiler_Err:
	push edx
	lea edx,Err_Msg_Space
	mov ah,09h
	int 21h

	pop edx
	mov ah,09h
	int 21h

	mov eax,Script_String_Counter
	shr eax,1
	inc eax
	mov ebx,10
	lea esi,Num_Line_Msg_End
@@rr:
	cdq
	div ebx
	add dl,30h
	mov [esi],dl	
	dec esi	
	or eax,eax	
	jnz @@rr

	lea edx,Num_Line_Msg
	mov ah,09h
	int 21h

	mov ah,4ch
	int 21h

include compile2.inc

.DATA
Inner_Script_File_Name  db '           1.sc',0

Tones		db	9,11,0,2,4,5,7

.DATA?
Tmp1		dw	?
Tmp2		dd	?
Cur_Note_Num_2	dd	?