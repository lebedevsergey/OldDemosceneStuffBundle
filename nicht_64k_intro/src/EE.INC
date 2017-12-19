.CODE
locals

Wobble_Object  proc    near
                 pushad

;finit

mov [ebx].OBJECT_New_Coord_Transf_Flg,1
                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  esi,[ebx].LAST_VERTEX
@@B_S_Loop1:
                 fld  [ebp].Y
                 fst  [ebp].New_Y
                 fmul Wobble_Freq_
                 fcos
                 fmul Wobble_Amp
                 fadd Wobble_Coef2
                 fld  st

                 fmul  [ebp].Z
                 fstp  [ebp].New_Z

                 fmul  [ebp].X
                 fstp  [ebp].New_X

                 mov  eax,[ebp].K1
                 mov  [ebp].New_K1,eax

                 add  ebp,SIZE TVERTEX
                 cmp  ebp,esi
                 jbe  @@B_S_Loop1

                 popad
                 retn
                 endp

.DATA
Wobble_Amp         dd   0.0
Wobble_Coef2       dd   1.0

Wobble_Freq   dd   0.7;4

Wobble_X_Amp2   dw  1
Wobble_Y_Amp2   dw   1
Wobble_Z_Amp2   dw   1

.DATA?
Wobble_Freq_   dd     ?
Wobble_Angel    dw   ?
