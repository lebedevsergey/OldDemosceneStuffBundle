MIN_PHONG1_  equ  70

.CODE
.486p
locals


Do_Phong1           Proc   Near
		pushad


                mov  bp,-TEX_SIZE/2
@@Loop_Y2:
               
                mov  cx,-TEX_SIZE/2
@@Loop_X2:
		mov  Tmp1,255
		fild Tmp1

		mov  Tmp1,cx
		fild Tmp1
fmul st,st
		mov  Tmp1,bp
		fild Tmp1
fmul st,st
		faddp st(1),st
		fsqrt		
		fsqrt		

		fexp

		fmul  Phong1_Coef

		fsubp st(1),st
		fistp Tmp1

mov ax,MIN_PHONG1
cmp Tmp1ax
jge @@tt2
mov Tmp1,ax
@@tt2:

		mov   ax,Tmp1
		mov   ah,al	
		bswap eax
		mov   ax,Tmp1
		mov   ah,al	

		stosd

                inc cx
                cmp  cx,TEX_SIZE/2
                jnz @@Loop_X2

                inc bp
                cmp bp,TEX_SIZE/2
                jnz @@Loop_Y2


		popad
                retn
endp


.DATA
Phong1_Coef  dd  0.7
MIN_PHONG1   dw  MIN_PHONG1_