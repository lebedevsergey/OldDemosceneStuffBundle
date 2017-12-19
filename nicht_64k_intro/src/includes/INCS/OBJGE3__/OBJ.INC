
; Это дело патчено для загрузки объектов уменьшенного
; размера, искать по Patch1

TOR_COM         equ  0
CYL_COM         equ  1
SIN_ENV_COM     equ  2
LINE_ROUTE_COM  equ  3
CIRC_ROUTE_COM  equ  4
SPIR_ROUTE_COM  equ  5
MOVE_OBJ_COM    equ  6
ROT_OBJ_COM     equ  7
FLIP_NORM_COM   equ  8
CALC_NORM_COM   equ  9
SPHERE_MAPP_COM equ  10
SPHERE_MAPP2_COM equ 11
OBJ_PARAMS_COM  equ  12
END_COM         equ  0FFH

ENV_X           equ  0
ENV_Y           equ  1
ENV_Z           equ  2


PHONG_OBJ         equ  0
TEX_OBJ           equ  1
WIRE_OBJ          equ  2
ALPHA_OBJ         equ  3

.DATA


Gen_Object_Com_Tab:
                dd   Do_Torus
                dd   Do_Cyl
                dd   Sin_Envelope
                dd   Do_Line_Route
                dd   Do_Circle_Route
                dd   Do_Spiral_Route
                dd   Move_Obj
                dd   Rotate_Object
                dd   Flip_Normals
                dd   Calc_Object_Norm
                dd   Calc_Obj_Sphere_Mapp
                dd   Calc_Obj_Sphere_Mapp2
                dd   Set_Obj_Params

Obj_Shading_Exec_Proc:
                         dd   Exec_Object_Phong
                         dd   Exec_Object_Tex
                         dd   Exec_Object_Wire
                         dd   Exec_Alpha_Obj

Obj_Shading_Draw_Proc:
                         dd   Draw_Poly_Phong
                         dd   Draw_Poly_Tex
                         dd   Draw_Poly_Tex
                         dd   Draw_Alpha_Obj2

MAX_ROUTE_LENGTH  equ   50               ; Максимальная длина пути объекта
MAX_TUBE_ANGLE_STEP equ 50


.CODE
locals

Do_Object    proc near
; esi: ptr to object script
; edi: ptr to object
                pushad

;               lea  eax,Object1
                mov  Cur_Gen_Object,edi

                mov  eax,[esi]
                cmp  eax,NULL
                jnz  @@Not_Loaded_Obj                 ; Объект надо загружать, а не генерить
                call Load_Object
                jmp  @@rr1
@@Not_Loaded_Obj:
                call Alloc_Object_Mem
@@rr1:
                lea edi,Envelope_X
                call Do_No_Envelope
                lea edi,Envelope_Y
                call Do_No_Envelope
                lea edi,Envelope_Z
                call Do_No_Envelope
@@com_cycle:
                xor  eax,eax
                lodsb           ; Берем команду
                cmp  al,END_COM
                jz   @@exit
                call dword ptr [Gen_Object_Com_Tab+eax*4]

                jmp  @@com_cycle

@@exit:


                popad
                retn

endp

Alloc_Object_Mem  proc
                pushad

                lodsd
                mov  ROUTE_LENGTH_,eax
                lodsd
                mov  TUBE_ANGLE_STEP_,eax

                                            ; Берем память для вершин объекта
                mov  eax,ROUTE_LENGTH_
                add  eax,2                  ; На всякий случай ;)
                imul eax,TUBE_ANGLE_STEP_
                imul eax,SIZE TVERTEX
                call Alloc_Mem
                mov  Cur_Gen_Object_Dots,eax

                                            ; Берем память для полигонов объекта
                mov  eax,ROUTE_LENGTH_
                imul eax,TUBE_ANGLE_STEP_
                imul eax,SIZE TFACE*2
                call Alloc_Mem
                mov  Cur_Gen_Object_Faces,eax


        mov eax,TUBE_ANGLE_STEP_
        imul eax,SIZE TVERTEX                ; Понадобится потом для генерации
        mov Value1,eax                       ; объекта
        mov eax,ROUTE_LENGTH_
        dec eax
        mov Value2,eax
        mov eax,TUBE_ANGLE_STEP_
        dec eax
        mov Value3,eax


                popad

                add esi,8
                retn
endp

.DATA?
ROUTE_LENGTH_     dd ?
TUBE_ANGLE_STEP_  dd ?

Value1  dd  ?
Value2  dd  ?
Value3  dd  ?


.DATA
Cur_Gen_Object  dd   ?
Cur_Gen_Object_Faces  dd  ?
Cur_Gen_Object_Dots   dd  ?

.CODE
Load_Vertexes   proc
; esi : Packed object ptr
; edi : Object vertices ptr
; ecx : Object vertices num
; ebx : Object ptr

        mov  [ebx].FIRST_VERTEX,edi
@@tt1:
        lodsd
        mov [edi].X,eax
        lodsd
        mov [edi].Y,eax
        lodsd
        mov [edi].Z,eax
        xor eax,eax
        mov [edi].K1,eax
        add edi,SIZE TVERTEX
        loop @@tt1

        sub  edi,SIZE TVERTEX
        mov  [ebx].LAST_VERTEX,edi
        retn
Load_Vertexes   endp

Load_Faces   proc
; esi : Packed object ptr
; edi : Object faces ptr
; ecx : Object faces num
; ebx : Object ptr

        mov  [ebx].FIRST_FACE,edi
@@tt1:
        xor eax,eax
        lodsb ;w             ; !!!!!!!! Patch1
        imul eax,SIZE TVERTEX
        add eax,Cur_Gen_Object_Dots
        mov [edi].PA,eax

        xor eax,eax
;        lodsw
        lodsb ;w             ; !!!!!!!! Patch1
        imul eax,SIZE TVERTEX
        add eax,Cur_Gen_Object_Dots
        mov [edi].PB,eax

        xor eax,eax
;        lodsw
        lodsb ;w             ; !!!!!!!! Patch1
        imul eax,SIZE TVERTEX
        add eax,Cur_Gen_Object_Dots
        mov [edi].PC,eax

        add edi,SIZE TFACE
        loop @@tt1

        sub  edi,SIZE TFACE
        mov  [ebx].LAST_FACE,edi
        retn
Load_Faces   endp

Load_Object     proc
                add  esi,4
                lodsd
                push esi
                mov  esi,eax

                xor  eax,eax
                lodsw
                mov  Cur_Gen_Object_Dots_Num,eax
                                            ; Берем память для вершин объекта
                add  eax,2                  ; На всякий случай ;)
                imul eax,SIZE TVERTEX
                call Alloc_Mem
                mov  Cur_Gen_Object_Dots,eax

                xor  eax,eax
                lodsw
                mov  Cur_Gen_Object_Faces_Num,eax
                                            ; Берем память для полигонов объекта
                imul eax,SIZE TFACE
                call Alloc_Mem
                mov  Cur_Gen_Object_Faces,eax

                mov  ebx,Cur_Gen_Object
                mov  ecx,Cur_Gen_Object_Dots_Num
                mov  edi,Cur_Gen_Object_Dots
                call Load_Vertexes

                mov  ebx,Cur_Gen_Object
                mov  ecx,Cur_Gen_Object_Faces_Num
                mov  edi,Cur_Gen_Object_Faces
                call Load_Faces

                pop  esi
                retn
Load_Object     endp
.DATA?
Cur_Gen_Object_Faces_Num   dd   ?
Cur_Gen_Object_Dots_Num   dd   ?

.CODE

Make_Two_Poly  proc near
        pushad
; IN : EBX,EAX - ptr to dots

        mov [ebp].PA,ebx
        mov [ebp+SIZE TFACE].PA,ebx

        add ebx,SIZE TVERTEX
        mov [ebp+SIZE TFACE].PC,ebx

        mov [ebp].PB,eax
        add eax,SIZE TVERTEX
        mov [ebp].PC,eax
        mov [ebp+SIZE TFACE].PB,eax

        popad
        retn
        endp

Do_Closed_Tube proc near

; ebx - ptr to vertexes
; ebp - ptr to faces
; esi - ptr to model

        pushad
        push ebx

        mov  Tmp1,360
        fild Tmp1
        fidiv TUBE_ANGLE_STEP_
        fstp  Gen_Obj_Angle_Adder

        mov [esi].FIRST_VERTEX,ebx
        mov [esi].FIRST_FACE,ebp


        xor eax,eax

        fild Route_X
        fstp [ebx].X
        fild Route_Y
        fstp [ebx].Y
        fild Route_Z
        fstp [ebx].Z
        fld1
        fstp [ebx].K1
        add ebx,SIZE TVERTEX

        xor ecx,ecx
@@ll1:
        cmp ecx,ROUTE_LENGTH_
        jnz @@tt1__
        dec eax
        dec eax
        fild Route_X[eax]
fmul Envelope_X [eax*2]
        fstp [ebx].X
        fild Route_Y[eax]
fmul Envelope_Y [eax*2]
        fstp [ebx].Y
        fild Route_Z[eax]
fmul Envelope_Z [eax*2]
        fstp [ebx].Z
        fld1
        fstp [ebx].K1
        jmp @@ex

@@tt1__:

        fldz
        fstp Gen_Obj_Angle

        xor edx,edx
@@ll2:

        fld   Gen_Obj_Angle
        fmul Radian_Coef
        fsincos
        fld  st
        fimul Radius2_X
        fmul Derive_X[eax*2]
fmul Envelope_X [eax*2]
        fiadd Route_X[eax]
        fstp [ebx].X

        fimul Radius2_Y
        fmul Derive_Y[eax*2]
fmul Envelope_Y[eax*2]
        fiadd Route_Y[eax]
        fstp [ebx].Y

        fimul Radius2_Z
fmul Envelope_Z [eax*2]
        fiadd Route_Z[eax]
        fstp [ebx].Z

        fld1
        fstp [ebx].K1

        add ebx,SIZE TVERTEX

        fld   Gen_Obj_Angle
        fadd  Gen_Obj_Angle_Adder
        fstp  Gen_Obj_Angle

        inc edx
        cmp edx,TUBE_ANGLE_STEP_
        jb  @@ll2

        inc eax
        inc eax
@@ex:
        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe @@ll1


;       sub ebx,SIZE TVERTEX
        mov [esi].LAST_VERTEX,ebx

        pop ebx
        add ebx,SIZE TVERTEX

        xor ecx,ecx
@@ll3:

        push ebx

        xor edx,edx
@@ll4:

        or cx,cx
        jnz @@_tt1
        mov eax,[esi].FIRST_VERTEX
        mov [ebp].PA,eax
        mov [ebp].PB,ebx
        mov eax,ebx
        add eax,SIZE TVERTEX
        mov [ebp].PC,eax
        add ebp,SIZE TFACE
        jmp @@ex1_
@@_tt1:
        cmp ecx,Value2            ;ROUTE_LENGTH-1
        jb  @@ex1_
        mov eax,[esi].LAST_VERTEX
        mov [ebp].PB,eax
        mov [ebp].PA,ebx
        mov eax,ebx
        add eax,SIZE TVERTEX
        mov [ebp].PC,eax
        add ebp,SIZE TFACE
        jmp @@@ex1
@@ex1_:

        mov eax,ebx
        add eax,Value1            ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        call Make_Two_Poly

        add ebp,SIZE TFACE*2

@@@ex1:
        add ebx,SIZE TVERTEX

        inc edx
        cmp edx,Value3              ;TUBE_ANGLE_STEP-1
        jb  @@ll4

        pop edx

        or cx,cx
        jnz @@_tt1_
        mov eax,[esi].FIRST_VERTEX
        mov [ebp].PA,eax
        mov [ebp].PB,ebx
        mov [ebp].PC,edx
        add ebp,SIZE TFACE
        jmp @@_ex_
@@_tt1_:
        cmp ecx,Value2   ;ROUTE_LENGTH-1
        jb @@_ex_
        mov eax,[esi].LAST_VERTEX
        mov [ebp].PB,eax
        mov [ebp].PA,ebx
        mov [ebp].PC,edx
        add ebp,SIZE TFACE
        jmp @@ex1__
@@_ex_:

        mov eax,ebx
        mov [ebp].PA,eax
        mov [ebp+SIZE TFACE].PA,eax
        add eax,Value1                     ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        mov [ebp].PB,eax

        mov [ebp+SIZE TFACE].PC,edx
        add edx,Value1                     ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        mov [ebp+SIZE TFACE].PB,edx
        mov [ebp].PC,edx

        add ebp,SIZE TFACE*2

@@ex1__:
        add ebx,SIZE TVERTEX

        inc ecx
        cmp ecx,ROUTE_LENGTH_                      ;-1  ;-1
        jb  @@ll3

        sub ebp,SIZE TFACE
        mov [esi].LAST_FACE,ebp


        popad

        retn
        endp

Do_Tube proc near

; ebx - ptr to vertexes
; ebp - ptr to faces
; esi - ptr to model

        pushad
        push ebx

        mov  Tmp1,360
        fild Tmp1
        fidiv TUBE_ANGLE_STEP_
        fstp  Gen_Obj_Angle_Adder


        mov [esi].FIRST_VERTEX,ebx
        mov [esi].FIRST_FACE,ebp

        xor eax,eax

        xor ecx,ecx
@@ll1:

        fldz
        fstp Gen_Obj_Angle

        xor edx,edx
@@ll2:

        fld   Gen_Obj_Angle
        fmul Radian_Coef
        fsincos
        fld  st
        fimul Radius2_X
        fmul Derive_X[eax*2]
fmul Envelope_X [eax*2]
        fiadd Route_X[eax]
        fstp [ebx].X

        fimul Radius2_Y
        fmul Derive_Y[eax*2]
fmul Envelope_Y[eax*2]
        fiadd Route_Y[eax]
        fstp [ebx].Y

        fimul Radius2_Z
fmul Envelope_Z [eax*2]
        fiadd Route_Z[eax]
        fstp [ebx].Z

        fld1
        fstp [ebx].K1

        add ebx,SIZE TVERTEX

        fld   Gen_Obj_Angle
        fadd  Gen_Obj_Angle_Adder
        fstp  Gen_Obj_Angle

        inc edx
        cmp edx,TUBE_ANGLE_STEP_
        jb  @@ll2

        inc eax
        inc eax

        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe @@ll1

        sub ebx,SIZE TVERTEX
        mov [esi].LAST_VERTEX,ebx

        pop ebx
        push ebx

        xor ecx,ecx
@@ll3:

        push ebx

        xor edx,edx
@@ll4:
        mov eax,ebx
        add eax,Value1           ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        call Make_Two_Poly

        add ebx,SIZE TVERTEX
        add ebp,SIZE TFACE*2

        inc edx
        cmp edx,Value3            ;TUBE_ANGLE_STEP-1
        jb  @@ll4

        pop edx

        mov eax,ebx
        mov [ebp].PA,eax
        mov [ebp+SIZE TFACE].PA,eax
        add eax,Value1                   ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        mov [ebp].PB,eax

        mov [ebp+SIZE TFACE].PC,edx
        add edx,Value1                  ;(SIZE TVERTEX)*(TUBE_ANGLE_STEP)
        mov [ebp+SIZE TFACE].PB,edx
        mov [ebp].PC,edx

        add ebx,SIZE TVERTEX
        add ebp,SIZE TFACE*2

        inc ecx
        cmp ecx,Value2                   ;ROUTE_LENGTH-1  ;-1
        jb  @@ll3


        pop  ecx

        cmp  Zamkn_Flag,1
        jnz  @@End_Make

        push ecx
        push ebx

        xor edx,edx
@@ll5:
        mov eax,ecx
        call Make_Two_Poly

        add ebx,SIZE TVERTEX
        add ecx,SIZE TVERTEX
        add ebp,SIZE TFACE*2

        inc edx
        cmp edx,Value3                       ;TUBE_ANGLE_STEP-1
        jb  @@ll5

        pop edx
        pop eax

        mov [ebp].PA,ebx
        mov [ebp+SIZE TFACE].PA,ebx
        mov [ebp].PB,ecx

        mov [ebp+SIZE TFACE].PC,edx
        mov [ebp+SIZE TFACE].PB,eax
        mov [ebp].PC,eax

        add ebp,SIZE TFACE*2

@@End_Make:

        sub ebp,SIZE TFACE
        mov [esi].LAST_FACE,ebp


        popad

        retn
        endp

.DATA?
RADIUS2_X      dw  ?
RADIUS2_Y      dw  ?
RADIUS2_Z      dw  ?

Angle1_Cos  dd  ?
Angle1_Sin  dd  ?


X1          dw  ?
Y1          dw  ?

Zamkn_Flag    db ?


.CODE
Do_Line_Route proc near

; ebx - ptr to vertexes
; ebp - ptr to faces
; esi - ptr to model

        pushad

        lodsw
        mov Line_Route_Len,ax


        fild Line_Route_Len
        fidiv ROUTE_LENGTH_
        fstp  Gen_Obj_Angle_Adder

        fldz
        fstp Gen_Obj_Angle


        xor eax,eax

        xor ecx,ecx
@@ll1:
        fld1
        fstp Derive_X[eax*2]

        fldz
        fistp Route_X[eax]

        fldz
        fstp Derive_Y[eax*2]

        fld   Gen_Obj_Angle
        fistp Route_Y[eax]

        fldz
        fistp Route_Z[eax]

        inc eax
        inc eax

        fld   Gen_Obj_Angle
        fadd  Gen_Obj_Angle_Adder
        fstp  Gen_Obj_Angle

        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe  @@ll1


        popad
inc esi
inc esi
        retn
        endp


Do_Spiral_Route:

; ebx - ptr to vertexes
; ebp - ptr to faces
; esi - ptr to model

        xor eax,eax
        lodsw
        mov Spiral_Len,ax
        lodsw
        mov Spiral_Times,ax
        pushad
        fild Spiral_Len
        fidiv ROUTE_LENGTH_
        fstp Spiral_Len_Coef
        mov Spiral_Flag,1
        jmp Circle2


Do_Circle_Route proc near

; ebx - ptr to vertexes
; ebp - ptr to faces
; esi - ptr to model

        pushad

        mov Spiral_Flag,0
        mov Spiral_Times,1

Circle2:

xor eax,eax
lodsw
mov Radius1_X,ax
lodsw
mov Radius1_Y,ax



        mov  Tmp1,360
        fild Tmp1
        fimul Spiral_Times
        fidiv ROUTE_LENGTH_
        fstp  Gen_Obj_Angle_Adder

        fldz
        fstp Gen_Obj_Angle

        xor eax,eax

        xor ecx,ecx
@@ll1:
        fld Gen_Obj_Angle
        fmul Radian_Coef
        fsincos

        fst Derive_X[eax*2]
        fimul Radius1_X
        fistp Route_X[eax]

        fst Derive_Y[eax*2]

        fimul Radius1_Y
        fistp Route_Y[eax]

        fldz
        cmp Spiral_Flag,1
        jnz @@tt1
        fstp st
        mov Tmp1,cx
        fild Tmp1
        fmul Spiral_Len_Coef
@@tt1:
        fistp Route_Z[eax]

        inc eax
        inc eax

        fld   Gen_Obj_Angle
        fadd  Gen_Obj_Angle_Adder
        fstp  Gen_Obj_Angle

        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe @@ll1


        popad
add esi,4
        retn
        endp


Do_Sin_Envelope proc near
; edi - ptr to env

        pushad

        mov  Tmp1,360
        fild Tmp1
        fmul Sin_Envelope_Freq
        fidiv ROUTE_LENGTH_
        fstp  Gen_Obj_Angle_Adder

        fldz
        fstp Gen_Obj_Angle

        xor eax,eax

        xor ecx,ecx
@@ll1:
        fld Gen_Obj_Angle
        fmul Radian_Coef
        fsin

        fmul Sin_Envelope_Amp
        fadd Sin_Envelope_Adder

        fstp dword ptr [edi+ecx*4]

        fld   Gen_Obj_Angle
        fadd  Gen_Obj_Angle_Adder
        fstp  Gen_Obj_Angle

        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe @@ll1


        popad

        retn
        endp

Do_No_Envelope proc near
; edi - ptr to env

        pushad

        xor eax,eax

        xor ecx,ecx
@@ll1:
        fld1
        fstp dword ptr [edi+ecx*4]

        inc ecx
        cmp ecx,ROUTE_LENGTH_
        jbe @@ll1


        popad

        retn
        endp


.DATA?

Sin_Envelope_Amp        dd   ?
Sin_Envelope_Adder      dd   ?
Sin_Envelope_Freq       dd   ?

Gen_Obj_Angle  dd   ?
Gen_Obj_Angle_Adder  dd  ?

Spiral_Flag  db  ?
Spiral_Len  dw  ?
Spiral_Len_Coef  dd  ?
Spiral_Times  dw  ?

RADIUS1_X     dw  ?
RADIUS1_Y     dw  ?

Route_X     dw  MAX_ROUTE_LENGTH+10 dup (?)
Route_Y     dw  MAX_ROUTE_LENGTH+10 dup (?)
Route_Z     dw  MAX_ROUTE_LENGTH+10 dup (?)

Derive_X     dd  MAX_ROUTE_LENGTH+2 dup (?)
Derive_Y     dd  MAX_ROUTE_LENGTH+2 dup (?)
Derive_Z     dd  MAX_ROUTE_LENGTH+2 dup (?)

Envelope_X     dd  MAX_ROUTE_LENGTH+2 dup (?)
Envelope_Y     dd  MAX_ROUTE_LENGTH+2 dup (?)
Envelope_Z     dd  MAX_ROUTE_LENGTH+2 dup (?)

.CODE
Do_Torus proc near

pushad
xor eax,eax
lodsw
mov Radius2_X,ax
lodsw
mov Radius2_Y,ax
lodsw
mov Radius2_Z,ax

mov Zamkn_Flag,1

mov ebx,Cur_Gen_Object_Dots
mov ebp,Cur_Gen_Object_Faces
mov esi,Cur_Gen_Object
call Do_Tube

popad
add esi,6
retn
endp

Do_Cyl proc near
pushad
xor eax,eax

lodsw
mov Radius2_X,ax
lodsw
mov Radius2_Y,ax
lodsw
mov Radius2_Z,ax

mov Zamkn_Flag,0

mov ebx,Cur_Gen_Object_Dots
mov ebp,Cur_Gen_Object_Faces
mov esi,Cur_Gen_Object
;call Do_Tube

call Do_Closed_Tube


popad
add esi,6
retn
endp

.DATA?
Line_Route_Len  dw  ?


.CODE

Sin_Envelope proc near
; db Env_Num,Amp*10,Add,Freq
pushad
      xor eax,eax
      lodsb
      mov edi,dword ptr Env_Tab[eax*4]

      lodsd
      mov Sin_Envelope_Amp,eax

      lodsd
      mov Sin_Envelope_Adder,eax

      lodsd
      mov Sin_Envelope_Freq,eax

      call Do_Sin_Envelope
popad
     add esi,13
retn
endp


.CODE
Move_Obj  proc near
        pushad

        mov ebx,Cur_Gen_Object

        xor eax,eax

        lodsw
        mov Obj_Move_X,ax
        lodsw
        mov Obj_Move_Y,ax
        lodsw
        mov Obj_Move_Z,ax

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX

@@B_S_Loop1:
                 fld  [ebp].X
                 fiadd Obj_Move_X
                 fstp  [ebp].X

                 fld  [ebp].Y
                 fiadd Obj_Move_Y
                 fstp  [ebp].Y

                 fld  [ebp].Z
                 fiadd Obj_Move_Z
                 fstp  [ebp].Z

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1

        popad
     add esi,6
        retn
        endp


Rotate_Object  proc    near
                 pushad

                lodsw
                mov  Tmp1,ax
push esi
                fild Tmp1
                fmul Radian_Coef
                lea edi,Obj_Tmp_Matr1
                call do_rot_X_mat

pop esi
                lodsw
                mov  Tmp1,ax
push esi
                fild Tmp1
                fmul Radian_Coef
                lea edi,Obj_Tmp_Matr2
                call do_rot_Y_mat
pop esi
                lodsw
                mov  Tmp1,ax
                fild Tmp1
                fmul Radian_Coef
                lea edi,Obj_Tmp_Matr3
                call do_rot_Z_mat

                lea ebx,Obj_Tmp_Matr1
                lea esi,Obj_Tmp_Matr2
                lea edi,Obj_Tmp_Matr1
                call matmat_mul

                lea esi,Obj_Tmp_Matr1
                lea ebx,Obj_Tmp_Matr3
                lea edi,Obj_Tmp_Matr1
                call matmat_mul

                lea esi,Obj_Tmp_Matr1
                mov ebx,Cur_Gen_Object

                                                ; Повернем вершины и их нормали
                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  eax,[ebx].LAST_VERTEX
@@B_S_Loop1:
                 lea  ebx,[ebp].X
                 lea  edi,[ebp].X
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1


                 popad
                 add  esi,6

                 retn
                 endp



Flip_Normals  proc near
        pushad
        mov ebx,Cur_Gen_Object

        mov  ebp,[ebx].FIRST_FACE
        mov  edx,[ebx].LAST_FACE
@@B_S_Loop1:
                 mov eax,[ebp].PA
                 xchg eax,[ebp].PB
                 mov [ebp].PA,eax

        add  ebp, SIZE TFACE
        cmp  ebp,edx
        jbe  @@B_S_Loop1

                popad
                retn
endp

.CODE
Calc_Object_Norm  proc    near
; in EBX->object
                 pushad
        mov ebx,Cur_Gen_Object

                 push ebx

                 mov  ebp,[ebx].FIRST_VERTEX   ; Обнуление нормалей вершин
                 mov  ecx,[ebx].LAST_VERTEX
@@B_S_Loop0:
                 mov  [ebp].NX,0
                 mov  [ebp].NY,0
                 mov  [ebp].NZ,0
                 mov  [ebp].NK,0
                 mov  [ebp].NPoly,0

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop0



                 mov  ebp,[ebx].FIRST_FACE
                 mov  eax,[ebx].LAST_FACE

@@B_S_Loop1:
                 call Calc_Poly_Normal

                 add_norm [ebp].PA
                 add_norm [ebp].PB
                 add_norm [ebp].PC

                 add  ebp, SIZE TFACE
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1


                 pop  ebx

                 mov  ebp,[ebx].FIRST_VERTEX   ; Вычисление нормалей вершин
                 mov  ecx,[ebx].LAST_VERTEX
@@B_S_Loop3:
                 fld  [ebp].NX
                 fidiv [ebp].NPoly
                 fld  [ebp].NY
                 fidiv [ebp].NPoly
                 fld  [ebp].NZ
                 fidiv [ebp].NPoly
                 fld  [ebp].NK
                 fidiv [ebp].NPoly
                 fstp  [ebp].NK
                 fstp  [ebp].NZ
                 fstp  [ebp].NY
                 fstp  [ebp].NX

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop3

                 popad
                 retn
                 endp

Set_Obj_Params proc
                 pushad
                 mov ebx,Cur_Gen_Object

mov [ebx].OBJECT_New_Coord_Transf_Flg,0
                 
                 lodsd
                 mov [ebx].CAMERA_DIST,eax

                 lodsd
                 mov [ebx].MIN_CUT_Z,eax

                 lodsd
                 mov edx,dword ptr Obj_Shading_Exec_Proc[eax*4]
                 mov [ebx].OBJECT_SHADING_TYPE,edx
                 mov  edx,dword ptr Obj_Shading_Draw_Proc[eax*4]

                 lodsd    ; Берем указатель  на текстуру
                 mov  edi,eax

                 mov  ebp,[ebx].FIRST_FACE
                 mov  ecx,[ebx].LAST_FACE
@@B_S_Loop3:
                mov [ebp].Poly_Draw_Proc,edx
                mov [ebp].Poly_Texture_Ptr,edi

                 add  ebp, SIZE TFACE
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop3

                 mov ecx,SIZE OBJECT_MOVE_VEC
                 lea edi,[ebx].OBJECT_MOVE_VEC
                 xor eax,eax
                 rep stosb

                 mov  [ebx].OBJECT_ANGLE_X,eax
                 mov  [ebx].OBJECT_ANGLE_Y,eax
                 mov  [ebx].OBJECT_ANGLE_Z,eax

                 popad
                 add esi,16
                 retn
                 endp


Calc_Obj_Sphere_Mapp   proc    near
                 pushad
        mov ebx,Cur_Gen_Object

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  ecx,[ebx].LAST_VERTEX
@@B_S_Loop3:
                                        ; Вычисление текстурных кооординат
                 Conv_X_U  [ebp].NZ,[ebp].NY,[ebp].VV
                 Conv_X_U  [ebp].NZ,[ebp].NX,[ebp].VU

;                Conv_X_U  [ebp].Z,[ebp].Y,[ebp].VV
;                Conv_X_U  [ebp].Z,[ebp].X,[ebp].VU

                 add  ebp, SIZE TVERTEX
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop3

                 popad
                 retn
                 endp


Calc_Obj_Sphere_Mapp2   proc    near

lodsd
mov @@Radius_,eax

                 pushad
        mov ebx,Cur_Gen_Object
finit

fldpi
fstp PI_

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  ecx,[ebx].LAST_VERTEX
@@B_S_Loop3:
                                        ; Вычисление текстурных кооординат

fld [ebp].Z
fdiv @@Radius_
call Arccos
fld st
fdiv PI_
mov Tmp1,255
fimul Tmp1
        fmul  TRI_FIX_P_COEF_UV_
fistp [ebp].VV

fsin
fmul @@Radius_
fld [ebp].X
fdiv st,st(1)
call Arccos
fdiv PI_

mov Tmp1,255
fimul Tmp1
        fmul  TRI_FIX_P_COEF_UV_
fistp [ebp].VU

fstp st

;                Conv_X_U  [ebp].NZ,[ebp].NY,[ebp].VV
;                Conv_X_U  [ebp].NZ,[ebp].NX,[ebp].VU

;                Conv_X_U  [ebp].Z,[ebp].Y,[ebp].VV
;                Conv_X_U  [ebp].Z,[ebp].X,[ebp].VU


                 add  ebp, SIZE TVERTEX
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop3


                 popad
                 retn
@@Radius_  dd   ?
                 endp
.DATA?

PI_      dd   ?


.DATA?
Obj_Tmp_Matr1  MATRIX4x4 <?>
Obj_Tmp_Matr2  MATRIX4x4 <?>
Obj_Tmp_Matr3  MATRIX4x4 <?>

;Obj_Tmp_Vec1  VECTOR4 <?>

Obj_Move_X  dw  ?
Obj_Move_Y  dw  ?
Obj_Move_Z  dw  ?


.DATA
Env_Fix_P_Coef  dw  10

Env_Tab:
dd  offset Envelope_X
dd  offset Envelope_Y
dd  offset Envelope_Z
