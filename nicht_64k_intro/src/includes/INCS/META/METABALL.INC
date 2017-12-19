.CODE
locals

Calc_One_Blob  macro
local @@tt1,@@tt2
; esi - blob ptr
;             fld    __1
              fld    [edi].Func_Val_X
              fsub   [esi].Blob_X
              fld    [edi].Func_Val_Y
              fsub   [esi].Blob_Y
              fld    [edi].Func_Val_Z
              fsub   [esi].Blob_Z

IF  METAB_TYPE EQ  1
fxch   st(2)            ;XYZ
fmul st,st
fxch   st(1)            ;YXZ
fmul st,st
fxch   st(2)            ;ZXY
fmul st,st
fxch   st(2)            ;YXZ

ELSE
fabs
fxch   st(1)            ;YXZ
fabs
fxch   st(2)            ;YXZ
fabs
ENDIF

              faddp  st(1),st
              faddp  st(1),st

fld st
fcomp Eff_Rad_Sqr
fnstsw ax
shr ah,1
jbe  @@tt1
fstp st
fldz
jmp @@tt2
@@tt1:

fmul Eff_Rad_Inv_Sqr
fld  st
fmul st,st
fld  st
fmul st,st(2)
fmul Coef_6_1
fxch st(1)
fmul Coef_4_1
fxch st(2)
fmul Coef_2_1
fxch st(1)
faddp st(1),st
faddp st(1),st
;fadd    __1

@@tt2:
;             fdivp  st(1),st
;             retn
;Calc_One_Blob  endp
              endm

Find_Hash    proc
; ebx - vertex 1
; esi - vertex 2
; OUT: edi - ptr to vertex, if found

             mov   eax,ebx
             add   eax,esi                           ;  HASH1
             mov   edx,ebx                           ;  HASH2
             imul  edx,esi                           ; Считаем хеш
@@Next_Try:
             and   eax,BLOB_VERTS_HASH_TAB_SIZE
             cmp   Blob_Verts_Hash_Tab1[eax*4],0
             jz    @@Not_Found
             cmp   edx,Blob_Verts_Hash_Tab2[eax*4]
             jz    @@Found
             add   eax,HASH_COLLISION_STEP
             jmp   @@Next_Try
@@Found:
             mov   edi,Blob_Verts_Hash_Tab1[eax*4]
             clc
             retn
@@Not_Found:
             stc
             retn
Find_Hash    endp

Add_Hash     proc
; ebx - vertex 1
; esi - vertex 2
; !!!!!!!!!!     Ф-ция может зациклиться при маленькой
                 ; хеш-таблице
             mov   eax,ebx
             add   eax,esi     ;  HASH1
@@Next_Try:
             and   eax,BLOB_VERTS_HASH_TAB_SIZE       ; Считаем хеш
             cmp   Blob_Verts_Hash_Tab1[eax*4],0
             jz    @@Add
             add   eax,HASH_COLLISION_STEP
             jmp   @@Next_Try
@@Add:
             mov   Blob_Verts_Hash_Tab1[eax*4],edi
             mov   edx,ebx     ;  HASH2
             imul  edx,esi
             mov   Blob_Verts_Hash_Tab2[eax*4],edx
             retn
Add_Hash     endp


Init_Blobs  proc
            fild  BOX_X_SIZE_
            fidiv BOX_X_DIM_
            fstp  BOX_X_STEP
            fild  BOX_Y_SIZE_
            fidiv BOX_Y_DIM_
            fstp  BOX_Y_STEP
            fild  BOX_Z_SIZE_
            fidiv BOX_Z_DIM_
            fstp  BOX_Z_STEP

            mov   Meta_Object.OBJECT_SHADING_TYPE,offset Exec_Object_Phong
            mov   Tmp1,BLOB_OBJ_CAM_DIST
            fild  Tmp1
            fstp  Meta_Object.CAMERA_DIST
            mov   Meta_Object.MIN_CUT_Z,NULL   ;Ничего близкого не режем ;)

              lea   edi,Iso_Func

              mov   eax,Blob_Start_Z      ; Считаем координаты узлов сетки
              mov   Blob_Cur_Z,eax
              mov   edx,BOX_Z_DIM
@@Cyc_Z:
              mov   eax,Blob_Start_Y
              mov   Blob_Cur_Y,eax
              mov   ecx,BOX_Y_DIM
@@Cyc_Y:
              mov   eax,Blob_Start_X
              mov   Blob_Cur_X,eax
              mov   ebx,BOX_X_DIM
@@Cyc_X:
              mov  eax,Blob_Cur_X
              mov  [edi].Func_Val_X,eax
              mov  eax,Blob_Cur_Y
              mov  [edi].Func_Val_Y,eax
              mov  eax,Blob_Cur_Z
              mov  [edi].Func_Val_Z,eax

              fld   Blob_Cur_X
              fadd  BOX_X_STEP
              fstp  Blob_Cur_X
              add   edi,SIZE ISO_FUNC_VALUE
              dec   ebx
              jnz   @@Cyc_X

              fld   Blob_Cur_Y
              fadd  BOX_Y_STEP
              fstp  Blob_Cur_Y
              dec   ecx
              jnz   @@Cyc_Y

              fld   Blob_Cur_Z
              fadd  BOX_Z_STEP
              fstp  Blob_Cur_Z
              dec   edx
              jnz   @@Cyc_Z

              fld   Meta_UV_Coef1
              fmul  TRI_FIX_P_COEF_UV_
              fstp  Meta_UV_Coef2

fld1
fld  Eff_Rad
fmul st,st
fst  Eff_Rad_Sqr
fdivp st(1),st
fstp Eff_Rad_Inv_Sqr

            lea  esi,Edge_Table_Pack
            lea  edi,Edge_Table
            mov  ecx,Edge_Table_Size_/2
            push ecx
            rep  movsw
            pop  ecx
            dec  esi
            dec  esi
@@Do:
            movsw
            sub  esi,4
            dec  ecx
            jnz @@Do


            retn
Init_Blobs  endp


Inters_Interp  proc
; ebx - Grid1
; esi - Grid1
; edi - Rez Grid

              fld1
              fst  [edi].K1
              fstp  [edi].NK

              mov  eax,[ebx].Func_Value    ; А вдруг ничег интерполить и не надо ?
              cmp  eax,[esi].Func_Value
              jz   @@Interp_Not_Need1

              fld  ISO_FUNC_CONST_
              fsub [ebx].Func_Value
              fld  [esi].Func_Value
              fsub [ebx].Func_Value
              fdivp st(1),st
                                    ; k
              fld  [esi].Func_Val_X
              fsub [ebx].Func_Val_X
              fld  [esi].Func_Val_Y
              fsub [ebx].Func_Val_Y
              fld  [esi].Func_Val_Z
              fsub [ebx].Func_Val_Z
              fmul st,st(3)
fxch st(2)                   ; XYZ
              fmul st,st(3)
fxch st(1)                   ; YXZ
              fmul st,st(3)

              fld  [esi].Func_Grad_X
              fsub [ebx].Func_Grad_X
              fld  [esi].Func_Grad_Y
              fsub [ebx].Func_Grad_Y
              fld  [esi].Func_Grad_Z
              fsub [ebx].Func_Grad_Z
              fmul st,st(6)
fxch st(2)                   ; XYZ
              fmul st,st(6)
fxch st(1)                   ; YXZ
              fmul st,st(6)
fxch st(2)                   ; ZXY
              fadd [ebx].Func_Grad_Z
fxch st(1)                   ; XZY
              fadd [ebx].Func_Grad_X
fxch st(2)                   ; YZX
              fadd [ebx].Func_Grad_Y
fxch st(1)                   ; ZYX
              fstp [edi].NZ
              fstp [edi].NY
              fstp [edi].NX

              fadd [ebx].Func_Val_Y
              fstp [edi].Y
              fadd [ebx].Func_Val_X
              fstp [edi].X
              fadd [ebx].Func_Val_Z
              fstp [edi].Z

              fstp st

              retn

@@Interp_Not_Need1:
              mov  eax,[ebx].Func_Val_X
              mov  [edi].X,eax
              mov  eax,[ebx].Func_Val_Y
              mov  [edi].Y,eax
              mov  eax,[ebx].Func_Val_Z
              mov  [edi].Z,eax
              mov  eax,[ebx].Func_Grad_X
              mov  [edi].NX,eax
              mov  eax,[ebx].Func_Grad_Y
              mov  [edi].NY,eax
              mov  eax,[ebx].Func_Grad_Z
              mov  [edi].NZ,eax
              retn
Inters_Interp  endp


Poligonize    macro
              pushad

              mov   Grid_Tri_Num,0
              mov   Cube_Index,0

              COMPARE_TMP_GRID 0
              COMPARE_TMP_GRID 1
              COMPARE_TMP_GRID 2
              COMPARE_TMP_GRID 3
              COMPARE_TMP_GRID 4
              COMPARE_TMP_GRID 5
              COMPARE_TMP_GRID 6
              COMPARE_TMP_GRID 7

              cmp  Cube_Index,0          ; Весь кубик сверху
              jz   @@No_Polys
              cmp  Cube_Index,0FFh       ; или снизу поверхности ?
              jz   @@No_Polys

              mov   ebx,Cube_Index
              mov   bp,Edge_Table[ebx*2]
              CALC_GRID_INTERSECT 0,1,0
              CALC_GRID_INTERSECT 1,2,1
              CALC_GRID_INTERSECT 2,3,2
              CALC_GRID_INTERSECT 3,0,3
              CALC_GRID_INTERSECT 4,5,4
              CALC_GRID_INTERSECT 5,6,5
              CALC_GRID_INTERSECT 6,7,6
              CALC_GRID_INTERSECT 7,4,7
              CALC_GRID_INTERSECT 8,4,0
              CALC_GRID_INTERSECT 9,5,1
              CALC_GRID_INTERSECT 10,6,2
              CALC_GRID_INTERSECT 11,7,3
                                      ; Делаем полигоны
              mov   esi,Cube_Index
              shl   esi,4             ; т.к. таблица у нас 16x256
              add   esi,offset Tri_Table
              mov   edi,Cur_Blob_Poly_Ptr
              xor   eax,eax
@@Do_Polys:
              lodsb
              cmp   al,-1
              jz    @@No_More_Polys
              mov   edx,Cur_Blob_Verts_Nums[eax*4]
              mov   [edi].PA,edx
              lodsb
              mov   edx,Cur_Blob_Verts_Nums[eax*4]
              mov   [edi].PB,edx
              lodsb
              mov   edx,Cur_Blob_Verts_Nums[eax*4]
              mov   [edi].PC,edx

              mov   [edi].Poly_Draw_Proc,offset Draw_Poly_Phong

              add   edi,SIZE TFACE
              inc   Grid_Tri_Num
              jmp   @@Do_Polys
@@No_More_Polys:
              mov   Cur_Blob_Poly_Ptr,edi
@@No_Polys:
              popad
;             retn
;Poligonize    endp
endm

Calc_Func_Values  macro
 local @@Cyc_XYZ
;              pushad

              lea   edi,Iso_Func

              mov   edx,BOX_Z_DIM*BOX_Y_DIM*BOX_X_DIM
@@Cyc_XYZ:
              fld   __0
              lea   esi,Blob1
              mov   ecx,BLOBS_NUM
@@Blobs:
              Calc_One_Blob
              add   esi,SIZE BLOB_STRUC
              dec   ecx
              faddp st(1),st
              jnz   @@Blobs
              fstp  [edi].Func_Value
              add   edi,SIZE ISO_FUNC_VALUE
              dec   edx
              jnz   @@Cyc_XYZ

                                     ; Считаем градиенты
              lea   ebp,Iso_Func
              mov   edx,(BOX_Z_DIM-1)*(BOX_Y_DIM-1)*(BOX_X_DIM-1)
@@Cyc_XYZ2:
              GET_NEIGHBOUR 1,0,0
              fsub  [ebp].Func_Value
              GET_NEIGHBOUR 0,1,0
              fsub  [ebp].Func_Value
              GET_NEIGHBOUR 0,0,1
              fsub  [ebp].Func_Value
fxch st(2)
              fstp  [ebp].Func_Grad_X
              fstp  [ebp].Func_Grad_Y
              fstp  [ebp].Func_Grad_Z

              add   ebp,SIZE ISO_FUNC_VALUE
              dec   edx
              jnz   @@Cyc_XYZ2

;             popad
;             retn
;Calc_Func_Values  endp
endm


Do_Surface    proc
              pushad

fstcw  Tmp1
and Tmp1,1111110011111111b      ; Уменьшаем точность деления
fldcw  Tmp1

              Calc_Func_Values

IFDEF USE_HASH_TAB
              lea   edi,Blob_Verts_Hash_Tab1
              mov   ecx,BLOB_VERTS_HASH_TAB_SIZE
              xor   eax,eax
              rep   stosd        ; Чистим хеш-таблицу
ENDIF

              mov   Cur_Blob_Vert_Ptr,offset Meta_Object_Verts
              mov   Cur_Blob_Poly_Ptr,offset Meta_Object_Polys

              lea   ebp,Iso_Func
              mov   edx,(BOX_X_DIM-1)*(BOX_Y_DIM-1)*(BOX_Z_DIM-1)
@@Cyc_XYZ:
              FILL_TMP_GRID 0,0,0,0
              FILL_TMP_GRID 1,0,0,1
              FILL_TMP_GRID 1,0,1,2
              FILL_TMP_GRID 0,0,1,3
              FILL_TMP_GRID 0,1,0,4
              FILL_TMP_GRID 1,1,0,5
              FILL_TMP_GRID 1,1,1,6
              FILL_TMP_GRID 0,1,1,7

              Poligonize

              add   ebp,SIZE ISO_FUNC_VALUE
              dec   edx
              jnz   @@Cyc_XYZ

              mov   eax,Cur_Blob_Vert_Ptr
              sub   eax,SIZE TVERTEX
              mov   Meta_Object.LAST_VERTEX,eax
              mov   eax,Cur_Blob_Poly_Ptr
              sub   eax,SIZE TFACE
              mov   Meta_Object.LAST_FACE,eax

              lea    ebx,Meta_Object
              Meta_Calc_UV          ; Считаем координаты текстуры

or Tmp1,NOT 1111110011111111b       ;Вернули точность деления
fldcw  Tmp1

              popad
              retn
Do_Surface    endp

.DATA
BOX_X_DIM_    dd    BOX_X_DIM
BOX_Y_DIM_    dd    BOX_Y_DIM
BOX_Z_DIM_    dd    BOX_Z_DIM

BOX_X_SIZE_    dd    BOX_X_SIZE
BOX_Y_SIZE_    dd    BOX_Y_SIZE
BOX_Z_SIZE_    dd    BOX_Z_SIZE

Blob_Start_X     dd    BOX_Start_X_
Blob_Start_Y     dd    BOX_Start_Y_
Blob_Start_Z     dd    BOX_Start_Z_

ISO_FUNC_CONST_  dd  ISO_FUNC_CONST

Meta_UV_Coef1   dd    128.0

Meta_Object    TMODEL   <Meta_Object_Verts,Meta_Object_Verts,\
      Meta_Object_Polys,Meta_Object_Polys,200.0,200.0,Draw_Poly_Phong>

__0            dd    0.0
Blob1           BLOB_STRUC <0.0,0.0,0.0>
rept BLOBS_NUM
         BLOB_STRUC <0.0,0.0,0.0>
endm

.DATA?
Blob_Cur_X     dd    ?
Blob_Cur_Y     dd    ?
Blob_Cur_Z     dd    ?

BOX_X_STEP     dd    ?
BOX_Y_STEP     dd    ?
BOX_Z_STEP     dd    ?

Cube_Index         dd    ?

Grid_Tri_Num       dd    ?

Meta_UV_Coef2      dd    ?

Cur_Blob_Vert_Ptr  dd    ?
Cur_Blob_Poly_Ptr  dd    ?

Iso_Func       db    BOX_X_DIM*BOX_Y_DIM*BOX_Z_DIM*SIZE ISO_FUNC_VALUE dup(?)
Cur_Grid_Ptrs  dd    2*2*2*4 dup(?)

Cur_Blob_Verts_Nums  dd   MAX_BLOB_VERT_NUM dup(?)

Blob_Verts_Hash_Tab1  dd   BLOB_VERTS_HASH_TAB_SIZE dup(?)
Blob_Verts_Hash_Tab2  dd   BLOB_VERTS_HASH_TAB_SIZE dup(?)

;Meta_Object_Verts  db   BOX_X_DIM*BOX_Y_DIM*BOX_Z_DIM*SIZE TVERTEX dup(?)
;Meta_Object_Polys  db   BOX_X_DIM*BOX_Y_DIM*BOX_Z_DIM*SIZE TFACE dup(?)

Meta_Object_Verts  db   (BOX_X_DIM+1)*(BOX_Y_DIM+1)*(BOX_Z_DIM+1)*SIZE TVERTEX dup(?)
Meta_Object_Polys  db   (BOX_X_DIM+1)*(BOX_Y_DIM+1)*(BOX_Z_DIM+1)*SIZE TFACE dup(?)

Eff_Rad_Inv_Sqr  dd ?
Eff_Rad_Sqr      dd ?
Meta_Tex         dd ?

Edge_Table   dw  Edge_Table_Size/2 dup (?)

.DATA
Eff_Rad  dd  40.0
Coef_6_1 dd  -2.444444444444
Coef_4_1 dd  1.888888888889
Coef_2_1 dd  -0.4444444444444
