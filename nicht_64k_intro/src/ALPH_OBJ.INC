.CODE
Transform_Alpha_Obj  proc
; ebx: object ptr
; esi: vertex matrix ptr
                 pushad
                                                ; Повернем вершины
                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX
@@B_S_Loop1:
                 lea  ebx,[ebp].X
                 lea  edi,[ebp].New_X
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1

                 popad
                 retn
Transform_Alpha_Obj  endp

Draw_Alpha_Obj   proc    near
; ebx - obj ptr
; esi - sprite ptr
                pushad

                 pushad
                                          ; Спроецируем вершины
                 mov eax,CAMERA_DIST[ebx]
                 mov Cam_Dist,eax

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX
@@Project:
                 fld   Cam_Dist
                 fld   [ebp].New_Z
                 fadd  Cam_Dist
                 fdivp st(1),st

                 fld   [ebp].New_Y
                 fld   [ebp].New_X
                 fmul  st,st(2)
                 fadd  X_SIZE_2
                 fmul  TRI_FIX_P_COEF_XY_
                 fistp [ebp].SX

                 fmulp st(1),st
                 fadd  Y_SIZE_2
                 fmul  TRI_FIX_P_COEF_XY_
                 fistp [ebp].SY

                 add  ebp,SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@Project

                 popad

                mov  @@Cur_Particle,offset Font_Particles

                mov  ebp,[ebx].FIRST_VERTEX
                mov  edx,[ebx].LAST_VERTEX
@@B_S_Loop1:

                fld  [ebp].New_Z
                fcomp Min_Alpha_Z
                fnstsw ax
                shr ah,1
                jc  @@Not_Draw

                mov  ebx,[ebp].SX
                mov  ecx,[ebp].SY

                fld  Sprite_Scale_Coef1
                fld  [ebp].New_Z
                fmul Sprite_Scale_Coef2
                fdivp st(1),st

                mov  eax,@@Cur_Particle
                fmul [eax].Part_Bright_Subber

                fld   st
                fcomp Max_Sprite_Scale
                fnstsw ax
                shr  ah,1
                jc   @@Ok
                fstp st
                fld  Max_Sprite_Scale
@@Ok:
                fmul Sprite_Scale1
                mov  esi,Cur_Alpha_Sprite

                fst  Sprite_Scale_X
                fstp Sprite_Scale_Y

                call Draw_Scaled_Alpha_Sprite2
;                call Draw_Alpha_Sprite2
@@Not_Draw:

                add  @@Cur_Particle,SIZE FONT_PARTICLE
                add  ebp,SIZE TVERTEX
                cmp  ebp,edx
                jbe  @@B_S_Loop1

                popad
                retn
.DATA?
@@Cur_Particle          dd  ?
.CODE
Draw_Alpha_Obj  endp


Draw_Alpha_Obj2 proc    near
; esi - poly ptr
                pushad
                mov  ebp,esi
                mov  ebp,[ebp]

                mov  edi,Virt_Screen1

                mov    Light_Drawed_Pixels_Count,0

                mov  edx,[ebp].PA
                mov  eax,[ebp].Poly_Texture_Ptr
                mov  @@Cur_Alpha_Sprite,eax

                fld  [edx].New_Z
                fcomp Min_Alpha_Z
                fnstsw ax
                shr ah,1
                jc  @@Not_Draw

                mov  ebx,[edx].SX
                mov  ecx,[edx].SY

                fld  Sprite_Scale_Coef1
                fadd [edx].New_Z
                fdiv [edx].New_Z

                mov  eax,Cur_Particle
                fmul [eax].Part_Bright_Subber

                fld   st
                fcomp Max_Sprite_Scale
                fnstsw ax
                shr  ah,1
                jc   @@Ok
                fstp st
                fld  Max_Sprite_Scale
@@Ok:

                fst  Sprite_Scale_X
                fstp Sprite_Scale_Y

                mov  esi,@@Cur_Alpha_Sprite
                call Draw_Scaled_Alpha_Sprite2_With_Z_Buf

@@Not_Draw:
                add  Cur_Particle,SIZE PARTICLE2_REC

                popad
                retn
.DATA?
Cur_Particle          dd  ?
@@Cur_Alpha_Sprite    dd  ?
.CODE
Draw_Alpha_Obj2 endp


Draw_Alpha_Obj3  proc    near
; ebx - obj ptr
; esi - sprite ptr
                pushad

                 pushad
                                          ; Спроецируем вершины
                 mov eax,CAMERA_DIST[ebx]
                 mov Cam_Dist,eax

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX
@@Project:
                 fld   Cam_Dist
                 fld   [ebp].New_Z
                 fadd  Cam_Dist
                 fdivp st(1),st

                 fld   [ebp].New_Y
                 fld   [ebp].New_X
                 fmul  st,st(2)
                 fadd  X_SIZE_2
                 fistp [ebp].SX

                 fmulp st(1),st
                 fadd  Y_SIZE_2
                 fistp [ebp].SY

                 add  ebp,SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@Project

                 popad

                mov  @@Cur_Particle,offset Font_Particles

                mov  ebp,[ebx].FIRST_VERTEX
                mov  edx,[ebx].LAST_VERTEX
@@B_S_Loop1:

                fld  [ebp].New_Z
                fcomp Min_Alpha_Z
                fnstsw ax
                shr ah,1
                jc  @@Not_Draw

                mov  ebx,[ebp].SX
                mov  ecx,[ebp].SY

                fld  Sprite_Scale_Coef1
                fld  [ebp].New_Z
                fmul Sprite_Scale_Coef2
                fdivp st(1),st

                fld   st
                fcomp Max_Sprite_Scale
                fnstsw ax
                shr  ah,1
                jc   @@Ok
                fstp st
                fld  Max_Sprite_Scale
@@Ok:
                fmul Sprite_Scale1
                mov  esi,Cur_Alpha_Sprite

                fst  Sprite_Scale_X
                fstp Sprite_Scale_Y

;                call Draw_Scaled_Alpha_Sprite2_Optim
call Draw_Alpha_Sprite2_Optim
;                call Draw_Alpha_Sprite2
@@Not_Draw:

                add  @@Cur_Particle,SIZE FONT_PARTICLE
                add  ebp,SIZE TVERTEX
                cmp  ebp,edx
                jbe  @@B_S_Loop1

                popad
                retn
.DATA?
@@Cur_Particle          dd  ?
.CODE
Draw_Alpha_Obj3  endp

.DATA
Sprite_Scale1         dd   0.2
Sprite_Scale_Coef1    dd   10.0

Sprite_Scale_Coef2    dd   0.2
Min_Alpha_Z           dd   1.0
Max_Sprite_Scale      dd   4.0

Cur_Alpha_Sprite      dd   Alpha_Sprite1

.DATA?
Alpha_Sprite1         dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)
Alpha_Sprite2         dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)
Alpha_Sprite3         dd  ?,?,MAX_SPRITE_X*MAX_SPRITE_Y dup (?)

.CODE
Draw_Scaled_Alpha_Sprite2_With_Z_Buf   proc
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y
                pushad

;shr ebx,SPRITE_DRAW_FIX_COEF
;shr ecx,SPRITE_DRAW_FIX_COEF

                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

                fild  SPRITE_X1
                fmul  Sprite_Scale_X
                fist  SCALED_SPRITE_X1
                fmul  One_Half_Coef
                fstp  Spr_Tmp3
                cmp   SCALED_SPRITE_X1,0
                jz    @@Not_Draw

                fild  SPRITE_Y1
                fmul  Sprite_Scale_Y
                fist  SCALED_SPRITE_Y1
                fmul  One_Half_Coef
                fstp  Spr_Tmp4
                cmp   SCALED_SPRITE_Y1,0
                jz    @@Not_Draw
                                   ; Чтобы были координаты центра спрайта
                mov   Tmp2,ebx
                fild  Tmp2
                fsub  Spr_Tmp3
                fistp Tmp2
                mov   ebx,Tmp2
                mov   Tmp2,ecx
                fild  Tmp2
                fsub  Spr_Tmp4
                fistp Tmp2
                mov   ecx,Tmp2

                mov  eax,ebx
                Ceil ebx
                mov  edx,ebx
                and  edx,0FFFF0000h
                sub  edx,eax
                mov  Sprite_Draw_Ex_X,edx
                sar  ebx,TRI_FIX_P_COEF_XY
                mov  eax,ecx
                Ceil ecx
                mov  edx,ecx
                and  edx,0FFFF0000h
                sub  edx,eax
                mov  Sprite_Draw_Ex_Y,edx
                sar  ecx,TRI_FIX_P_COEF_XY

                cmp    ebx,HOR_RAZR
                jge    @@Not_Draw
                cmp    ecx,VERT_RAZR
                jge    @@Not_Draw

                fld    SPRITE_DRAW_FIX_COEF_
                fdiv   Sprite_Scale_X
                fistp  Sprite_DU
                fld    SPRITE_DRAW_FIX_COEF_
                fdiv   Sprite_Scale_Y
                fistp  Sprite_DV

IFDEF SPRITE_SUBPIXEL
                                ; Делаем субпиксел
Do_Sprite_Subpixel Sprite_Draw_Ex_X,Sprite_DU,Sprite_Start_U
Do_Sprite_Subpixel Sprite_Draw_Ex_Y,Sprite_DV,Sprite_Start_V
ELSE
                mov    Sprite_Start_U,0
                mov    Sprite_Start_V,0
ENDIF

                cmp    ebx,HOR_RAZR
                jge    @@Not_Draw
                cmp    ecx,VERT_RAZR
                jge    @@Not_Draw

                fld1
                fdiv   Sprite_Scale_X
                fmul   SPRITE_DRAW_FIX_COEF_
                fistp  Sprite_DU

                fld1
                fdiv   Sprite_Scale_Y
                fmul   SPRITE_DRAW_FIX_COEF_
                fistp  Sprite_DV

IFDEF SPRITE_SUBPIXEL
                                ; Делаем субпиксел
Do_Sprite_Subpixel Sprite_Draw_Ex_X,Sprite_DU,Sprite_Start_U
Do_Sprite_Subpixel Sprite_Draw_Ex_Y,Sprite_DV,Sprite_Start_V
ELSE
                mov    Sprite_Start_U,0
                mov    Sprite_Start_V,0
ENDIF

                cmp    ebx,0
                jge    @@tt6
                mov    eax,ebx
                neg    eax
                sub    SCALED_SPRITE_X1,eax
                js     @@Not_Draw
                jz     @@Not_Draw
                imul   eax,Sprite_DU
                add    Sprite_Start_U,eax
                mov    ebx,0
@@tt6:
                cmp    ecx,0
                jge    @@tt7
                mov    eax,ecx
                neg    eax
                sub    SCALED_SPRITE_Y1,eax
                js     @@Not_Draw
                jz     @@Not_Draw
                imul   eax,Sprite_DV
                add    Sprite_Start_V,eax
                mov    ecx,0
@@tt7:

                mov    eax,ebx
                add    eax,SCALED_SPRITE_X1
                cmp    eax,HOR_RAZR
                jle    @@tt8
                sub    eax,HOR_RAZR
                jz     @@Not_Draw
                sub    SCALED_SPRITE_X1,eax
@@tt8:
                mov    eax,ecx
                add    eax,SCALED_SPRITE_Y1
                cmp    eax,VERT_RAZR
                jle    @@tt9
                sub    eax,VERT_RAZR
                jz     @@Not_Draw
                sub    SCALED_SPRITE_Y1,eax
@@tt9:
                mov    edi,ecx
                imul   edi,HOR_RAZR*4
                lea    edi,[edi+ebx*4]

                mov    eax,SCALED_SPRITE_X1
                shl    eax,2
                mov    Sprite_Adder,HOR_RAZR*4
                sub    Sprite_Adder,eax


                mov    ebp,Sprite_Start_V
                mov    edx,0
@@D_A_S_Loop1:
                push  edx

                mov    ebx,Sprite_Start_U
                mov    ecx,0
@@D_A_S_Loop2:
                mov  eax,Cur_Poly_Z
                cmp  eax,ZBuffer[edi]
                jge  @@Z_Clipped
;               mov  ZBuffer[edi],eax

                ror   ebp,SPRITE_DRAW_FIX_COEF
                ror   ebx,SPRITE_DRAW_FIX_COEF

                movzx edx,bp
                imul  edx,SPRITE_X1
                movzx eax,bx
                add   edx,eax

                rol   ebp,SPRITE_DRAW_FIX_COEF
                rol   ebx,SPRITE_DRAW_FIX_COEF

                Alpha_One_Byte_Z_Buf 0
                Alpha_One_Byte_Z_Buf 1
                Alpha_One_Byte_Z_Buf 2

                inc   Light_Drawed_Pixels_Count
@@Z_Clipped:
                add edi,4

                add ebx,Sprite_DU

                inc ecx
                cmp ecx,SCALED_SPRITE_X1
                jnz @@D_A_S_Loop2

                add ebp,Sprite_DV
                add edi,Sprite_Adder

                pop edx

                inc edx
                cmp edx,SCALED_SPRITE_Y1
                jnz @@D_A_S_Loop1
@@Not_Draw:

                popad
                retn
endp
