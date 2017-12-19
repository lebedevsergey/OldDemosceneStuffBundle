
TABLE_WIDTH_2_  equ  256*2
TABLE_HEIGHT_2_ equ  256*2

.CODE
locals

Tab_Form  proc near
                push esi
                mov esi,Cur_Tab_Form
                lodsd
                mov Table_Coef3,eax
                fstp Table_Tmp
                fldz
@@tt1:
                cmp    dword ptr [esi],NULL
                jz     @@ex

                fld    Table_Tmp
                fadd   dword ptr[esi]
                fmul   dword ptr[esi+4]
                fsin
                fmul   dword ptr[esi+8]
                faddp  st(1),st
                add    esi,12

                jmp    @@tt1

@@ex:
                fmul   Table_Coef3

                pop esi

                retn
                endp

Init_Table2      Proc   Near
                pushad

                mov     ecx,Table_Height
                shr     ecx,1
                mov     edx,Table_Width
                shr     edx,1

                mov     bp,dx
                neg bp

                finit

                neg     cx
                mov     Cur_Y1,cx
                neg     cx
@@I_T_LoopY:
                mov     Cur_X1,bp
@@I_T_LoopX:
                fild    Cur_Y1
                fmul    Table2_Y_Coef
                fild    Cur_X1
                fmul    Table2_X_Coef
                fpatan
                fdiv    Table2_TWO_PI
                fmul    Table2_Coef1

                fild    Cur_X1
                fmul    Table2_X_Coef
                fmul    st,st
                fild    Cur_Y1
                fmul    Table2_Y_Coef
                fmul    st,st
                faddp   st(1),st
                fsqrt
                fdiv    Table2_Coef2
                fmul    Table2_Coef1

fld st
fmul Radian_Coef
fmul Table2_Coef3
fsin
fabs
fmul Table2_Coef5
fmulp st(1),st
;fstp st


                fistp   Tmp1
                mov     bx,Tmp1
                fistp   Tmp1
                mov     ax,Tmp1
                mov     ah,bl
xchg al,ah
                stosw

                inc     Cur_X1
                cmp     Cur_X1,dx
                jne      @@I_T_LoopX

                inc     Cur_Y1
                cmp     Cur_Y1,cx
                jne      @@I_T_LoopY

                mov     eax,Table_Width ; ширина таблицы
                sub     eax,HOR_RAZR
                shl     eax,1
                mov     Table_Width_Adder,eax

                popad
                retn
 endp


Draw_Table_Tex   proc near
;esi Table ptr
;edi screen ptr
; ecx - table center X
; edx - table center Y
                pushad

;               lea     edi,Virt_Screen
;               mov     esi,offset Lookup_Table

                imul    edx,Table_Width
                add     ecx,edx

                mov     eax,Table_Height
                imul    eax,Table_Width
                add     eax,Table_Width
                shr     eax,1
                sub     eax,ecx
                shl     eax,1
;                mov     Table_Show_Adder,eax

                mov     edx,esi

                add     esi,eax ;Table_Show_Adder

                mov     ecx,VERT_RAZR
@@loc_0_13B6:
                mov     ebx,HOR_RAZR
@@loc_0_13B7:
                xor     eax,eax

                lodsw
add ax,Table_Tex_Adder
                mov     eax,dword ptr [eax*4+ebp]
                mov     [edi],eax
                add     edi,4

                dec      ebx
                jnz      @@loc_0_13B7

                add      esi,Table_Width_Adder

                dec      ecx
                jnz      @@loc_0_13B6

                popad
                retn
Draw_Table_Tex  endp

.DATA
Table_Width_2  dd   TABLE_WIDTH_2_
Table_Height_2  dd  TABLE_HEIGHT_2_

Table2_TWO_PI   dd  6.283
Table2_Coef1    dd  256.0
Table2_Coef2    dd  320.0

Table2_Coef3    dd  1.0
Table2_Coef5    dd  1.0

Table2_X_Coef   dd  1.0
Table2_Y_Coef   dd  1.0

Table_Rec1      TABLE_REC <?>

                dd NULL

Table_Rec2      TABLE_REC <?>

.DATA?
Cur_Tab_Form  dd ?
Table_Coef3   dd ?
Table_Tmp     dd  ?
Table_Width_Adder  dd  ?
Cur_X1 dw ?
Cur_Y1 dw ?
Table_Show_Adder dd ?

.DATA
Table_Tex_Adder  dw   0

Table_Width   dd  TABLE_WIDTH_
Table_Height  dd  TABLE_HEIGHT_

Table_Form2 dd 32.0
            dd 3.0,35.0,1.5
            dd NULL