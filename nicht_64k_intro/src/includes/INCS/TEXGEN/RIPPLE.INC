locals

.CODE

Draw_Ripple           Proc   Near
                pushad



                lea   edi,Wave_Map


                mov  bp,0
@@Loop_Y2:
                
                mov  cx,0
@@Loop_X2:
                mov  Tmp1,cx
                fild Tmp1
                fmul  Ripple_X_Coef               
		fmul  st,st

                mov  Tmp1,bp
                fild Tmp1
                fmul  Ripple_Y_Coef               
		fmul  st,st

		faddp st(1),st

		fsqrt

                fmul  Radian_Coef
                fsin

                fimul Ripple_Amp 

                fistp  Tmp1
                mov   ax,Tmp1
                stosb


                inc cx
                cmp  cx,TEX_SIZE
                jnz @@Loop_X2

                inc bp
                cmp  bp,TEX_SIZE
                jnz @@Loop_Y2


                popad

                retn
endp


Apply_Filter2           Proc   Near
		pushad
		push  edi


                lea   esi,Wave_Map
		lea   ebp,Tmp_Texture
		xor  edx,edx

		xor  ecx,ecx
@@Loop_X2:

		xor  eax,eax

		mov al,[esi+TEX_SIZE]
		sub al,[esi-TEX_SIZE]
		mov ah,al
		add ah,ch

		mov al,[esi+1]
		sub al,[esi-1]
		add al,cl

		mov  eax,[edi+eax*4]
		mov  dword ptr Tmp_Texture[ecx*4],eax

		inc  esi
		inc  ecx
		cmp  ecx,TEX_SIZE*TEX_SIZE
                jnz @@Loop_X2


		lea   esi,Tmp_Texture
		pop   edi
		mov   ecx,0FFFFh
		mov  ecx,TEX_SIZE*TEX_SIZE
		rep   movsd

		popad
                retn
endp

Ripplex256           Proc   Near
		pushad

		xor   eax,eax
		lodsb
		mov   Ripple_Amp,ax


                call Draw_Ripple
		call Apply_Filter2

		popad

		inc  esi

                retn
endp

.DATA
Ripple_X_Coef dd  17.5
Ripple_Y_Coef dd  17.5

.DATA
Ripple_Amp dw  ?

