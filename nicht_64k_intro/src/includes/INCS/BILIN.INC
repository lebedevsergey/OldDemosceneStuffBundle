.CODE
Interp_Fix_P_Coef  equ   8

Bilin_Interp      macro ;proc   near
; ebp - Cur U
; ebx - Cur V
; esi - Tex ptr
                  pushad

                  mov   edx,ebx
                  shld  eax,ebp,TRI_FIX_P_COEF_UV
                  shr   edx,TRI_FIX_P_COEF_UV

                  mov  dh,al
                  and  ebx,(1 SHL TRI_FIX_P_COEF_UV)-1

                  mov  ecx,[esi+edx*4]
                  mov byte ptr _FPUDouble_C1 + 4,ch
                  inc  dl
                  and ecx,00ff00ffh
                  mov dword ptr _FPUDouble_C1,ecx
                  mov  eax,[esi+edx*4]
                  mov byte ptr _FPUDouble_C2 + 4,ah
                  dec  dl
                  inc  dh
                  and eax,00ff00ffh
                  mov dword ptr _FPUDouble_C2,eax
                  mov  ecx,[esi+edx*4]
                  shr  ebx,TRI_FIX_P_COEF_UV-Interp_Fix_P_Coef
                  mov byte ptr _FPUDouble_C3 + 4,ch
                  and  ebp,(1 SHL TRI_FIX_P_COEF_UV)-1  ; Выделили float part
                  and ecx,00ff00ffh
                  fld  Bilin_Neg_Tab[ebx*8]
                  mov dword ptr _FPUDouble_C3,ecx

                  inc  dl
                  fld  Bilin_Neg_Tab[ebx*8+4]
                  mov  eax,[esi+edx*4]
                  mov byte ptr _FPUDouble_C4 + 4,ah
                  fld  st(1)
                  and eax,00ff00ffh
                  mov dword ptr _FPUDouble_C4,eax

                  fld  st(1)
                  fmul _FPUDouble_C1
                  fxch st(1)
                  fmul _FPUDouble_C2
                  faddp st(1),st
                  fxch  st(1)
                  fmul _FPUDouble_C3
                  fxch  st(2)
                  fmul _FPUDouble_C4
                  faddp st(2),st

                  shr   ebp,TRI_FIX_P_COEF_UV-Interp_Fix_P_Coef

                  fstp qword ptr _FPUDouble_Ready_2
                  mov  eax,dword ptr _FPUDouble_Ready_2         ;распаковываем rgb
                  mov  ah,byte ptr _FPUDouble_Ready_2 + 4
                  fstp qword ptr _FPUDouble_Ready_2
                  mov  byte ptr _FPUDouble_C3 + 4,ah
                  and  eax,00ff00ffh
                  mov  dword ptr _FPUDouble_C3,eax


                  fld  Bilin_Neg_Tab[ebp*8]
                  fld  Bilin_Neg_Tab[ebp*8+4]

                  mov  eax,dword ptr _FPUDouble_Ready_2         ;распаковываем rgb
                  mov  ah,byte ptr _FPUDouble_Ready_2 + 4
                  mov  byte ptr _FPUDouble_C4 + 4,ah

                  fmul _FPUDouble_C3
                  and  eax,00ff00ffh
                  mov  dword ptr _FPUDouble_C4,eax
                  fxch st(1)
                  fmul _FPUDouble_C4


                  faddp st(1),st

                  popad

                  fstp qword ptr _FPUDouble_Ready_2
                  mov eax,dword ptr _FPUDouble_Ready_2         ;распаковываем rgb
                  mov ah,byte ptr _FPUDouble_Ready_2 + 4
                                                     ; Цвет готов
;                 retn
;Bilin_Interp      endp
endm

Init_Bilin_Interp proc   near
                  xor  ebx,ebx
@@tt1:
                  mov  eax,1 SHL Interp_Fix_P_Coef
                  sub  eax,ebx
                  mov  Tmp1,bx
                  fild Tmp1
                  fstp Bilin_Neg_Tab[ebx*8]
                  mov  Tmp1,ax
                  fild Tmp1
                  fstp Bilin_Neg_Tab[ebx*8+4]

                  inc  ebx
                  cmp  ebx,1 SHL Interp_Fix_P_Coef
                  jna  @@tt1


                  retn
Init_Bilin_Interp endp


_FPUDouble_C1          dq      1.0
_FPUDouble_C2          dq      1.0
_FPUDouble_C3          dq      1.0
_FPUDouble_C4          dq      1.0
_FPUDouble_Ready_2       dq      ?

.DATA?
Tex_C1   dd   ?
Tex_C2   dd   ?
Tex_C3   dd   ?
Tex_C4   dd   ?

Bilin_Neg_Tab         dd  2*((1 SHL Interp_Fix_P_Coef)) dup (?)