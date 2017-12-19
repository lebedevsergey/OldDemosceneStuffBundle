locals

.CODE

Draw_Wobble    proc
                pushad



                lea   edi,Wave_Map


                mov  bp,0
@@Loop_Y2:
                
                mov  cx,0
@@Loop_X2:
                mov  Tmp1,cx
                fild Tmp1
                fmul Wobble_Y_Coef               
                fmul  Radian_Coef
                fsin 
                fimul Wobble_Y_Amp 
                fistp  Tmp1
                mov   ax,Tmp1
                stosb

                mov  Tmp1,bp
                fild Tmp1
                fmul Wobble_X_Coef                
                fmul  Radian_Coef
                fsin 
                fimul Wobble_X_Amp 
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

Apply_Filter     proc
                pushad
                push  edi


                lea   esi,Wave_Map
                lea   ebp,Tmp_Texture
                xor  edx,edx

                xor  ecx,ecx
@@Loop_X2:

                xor  eax,eax

		lodsb
                mov ah,al
                add ah,ch

		lodsb
                add al,cl

                mov  eax,[edi+eax*4]
                mov  dword ptr Tmp_Texture[ecx*4],eax


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


Wobblex256           Proc   Near
		pushad

                xor   eax,eax
                lodsb
                mov   Wobble_X_Amp,ax

                lodsb
                mov   Wobble_Y_Amp,ax

                call Draw_Wobble
                call Apply_Filter

                popad

                inc  esi
                inc  esi

                retn
endp

.DATA
Wobble_X_Coef dd  2.5
Wobble_Y_Coef dd  2.5

.DATA?
Wobble_X_Amp dw  ?
Wobble_Y_Amp dw  ?

Wave_Map db TEX_SIZE*TEX_SIZE*2*2 dup (?)
Tmp_Texture db TEX_SIZE*TEX_SIZE*4*2 dup (?)
