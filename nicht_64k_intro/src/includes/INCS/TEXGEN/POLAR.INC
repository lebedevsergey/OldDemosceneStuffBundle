;NOISE_VALUE  equ 40
locals

.CODE

Draw_Polar           Proc   Near
                pushad



                lea   edi,Wave_Map


                mov  bp,0
@@Loop_Y2:
                
                mov  cx,0
@@Loop_X2:
                mov  Tmp1,cx
                fild Tmp1
;                fmul Polar_Y_Coef               
                fmul  Radian_Coef
                fsin
                fimul Polar_Y_Amp
                fistp  Tmp1
                mov   ax,Tmp1

                stosb

                mov  Tmp1,bp
                fild Tmp1
;                fmul Polar_X_Coef                
                fmul  Radian_Coef
                fcos
                fimul Polar_X_Amp 
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

Apply_Filter3           Proc   Near
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
		lodsb

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


Polarx256           Proc   Near
		pushad

                call Draw_Polar
                call Apply_Filter3

                popad

                retn
endp

.DATA
Polar_X_Coef dd  2.5
Polar_Y_Coef dd  2.5

;.DATA?
Polar_X_Amp dw  256;255;?
Polar_Y_Amp dw  256;255;?
