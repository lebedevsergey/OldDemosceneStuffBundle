
.CODE
locals

Lissaju      proc
; OUT :
;      ECX - X
;      EDX - Y

       lea ecx,Time2

       fld  dword ptr [ecx]
fmul Radian_Coef
       fidiv [esi].Lissaju_Angle_Y_Coef
       fsin
       fimul [esi].Lissaju_Amp_Y_Coef
       fadd  VERT_RAZR_DIV_2

       fld  dword ptr [ecx]
fmul Radian_Coef
       fidiv  [esi].Lissaju_Angle_X_Coef
       fsin
       fimul [esi].Lissaju_Amp_X_Coef
       fadd  HOR_RAZR_DIV_2

       retn
       endp
.DATA
HOR_RAZR_DIV_2    dd  160.0 ;HOR_RAZR/2
VERT_RAZR_DIV_2    dd  100.0 ;VERT_RAZR/2