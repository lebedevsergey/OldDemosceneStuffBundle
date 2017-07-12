MAX_DEPTH  equ  400

.486p
.CODE
locals


Calc_Table      macro
.CODE
;                pushad

                lea     edi,[ebp].Lookup_Table

                mov     ecx,-VERT_RAZR/2
@@I_T_LoopY:
                mov     edx,-HOR_RAZR/2
@@I_T_LoopX:

		fild     TABLE_SCALE                

		mov     [ebp].Cur_X1,ecx
                fild    [ebp].Cur_X1
                fmul    st,st  
		mov     [ebp].Cur_Y1,edx
                fild    [ebp].Cur_Y1
                fmul    st,st
                faddp   st(1),st    

                fsqrt   

		fdivp   st(1),st
                fistp   [ebp].Tmp3
                mov     eax, [ebp].Tmp3
		stosb
                fild    [ebp].Cur_Y1
                fild    [ebp].Cur_X1
                fpatan  
                fldpi   
                fdivp   st(1), st

                mov     [ebp].Tmp3,256;1024;128
                fimul   [ebp].Tmp3
                fistp   [ebp].Tmp3
                mov     eax,[ebp].Tmp3
                stosb

                inc     edx
                cmp     edx,HOR_RAZR/2
                jnz      @@I_T_LoopX

                inc     ecx
                cmp     ecx,VERT_RAZR/2
                jnz      @@I_T_LoopY

;		popad
;                retn
endm


Draw_Table     macro
local @@tt0,@@I_T_LoopY
		lea     esi,[ebp].Lookup_Table

		movzx	ebx,[ebp].Rotoz_Angle

		mov	eax,ebx
mov bh,bl
	cmp byte ptr _PARTNUM,3
	jnz @@tt0
	add ebx,ebx
	neg ebx
@@tt0:
                mov     ecx,VERT_RAZR*HOR_RAZR
@@I_T_LoopY:
		xor	eax,eax
		lodsb
		mov	ah,al
		lodsb

		sub     ax,bx
                mov     edx,dword ptr Texture2[eax*4]
                mov     eax,[edi]

		 and  eax,0FEFEFEFEh
		 and  edx,0FEFEFEFEh
		 add  eax,edx
		 shr  eax,1
                stosd

                loop     @@I_T_LoopY

;                retn    
endm
