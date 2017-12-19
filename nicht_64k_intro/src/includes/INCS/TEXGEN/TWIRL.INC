;NOISE_VALUE  equ 40
locals

.CODE

Draw_Twirl           Proc   Near
                pushad



                lea   edi,Wave_Map


                mov  bp,-TEX_SIZE/2
@@Loop_Y2:
                mov  cx,-TEX_SIZE/2
@@Loop_X2:
                mov  Tmp1,cx
                fild Tmp1
                fmul    st,st
  
                mov  Tmp1,bp
                fild Tmp1
                fmul    st,st

                faddp   st(1),st    

                fsqrt   

                fistp   Tmp1
                mov     ax, Tmp1
                stosb


                mov  Tmp1,bp
                fild Tmp1
                mov  Tmp1,cx
                fild Tmp1

                fpatan  
                fmul  Radian_Coef2

                fistp  Tmp1
                mov   ax,Tmp1
                stosb



                inc cx
                cmp  cx,TEX_SIZE/2
                jnz @@Loop_X2

                inc bp
                cmp  bp,TEX_SIZE/2
                jnz @@Loop_Y2


                popad

                retn
endp

Twirlx256           Proc   Near
		pushad

;                xor   eax,eax
 ;               lodsb
  ;              mov   Wobble_X_Amp,ax

   ;             lodsb
    ;            mov   Wobble_Y_Amp,ax

                call Draw_Twirl
                call Apply_Filter3

                popad

;                inc  esi
 ;               inc  esi

                retn
endp

