.CODE
Morf_Trans_Alpha_Obj  proc
; ebx: object1 ptr
; edi: object2 ptr
; esi: vertex matrix ptr
; fpu - morf coef
                 pushad

                 fstp @@Morf_Coef
                                                ; Повернем вершины
                 mov  ebp,[edi].FIRST_VERTEX
                 mov  eax,[edi].LAST_VERTEX
                 mov  ecx,[ebx].FIRST_VERTEX
                 mov  edx,[ebx].LAST_VERTEX
@@B_S_Loop1:
                 fld  [ebp].X
                 fsub [ecx].X
                 fmul @@Morf_Coef
                 fadd [ecx].X
                 fstp [ecx].New_X
                 fld  [ebp].Y
                 fsub [ecx].Y
                 fmul @@Morf_Coef
                 fadd [ecx].Y
                 fstp [ecx].New_Y
;                 fld  [ebp].Z
 ;                fsub [ecx].Z
  ;               fmul @@Morf_Coef
   ;              fadd [ecx].Z

                 fld  [ecx].Z
                 fstp [ecx].New_Z

                 lea  ebx,[ecx].New_X
                 lea  edi,[ecx].New_X
                 call vecmat_mul

                 add  ebp, SIZE TVERTEX
                 add  ecx, SIZE TVERTEX
                 cmp  ecx,edx
                 cmp  ebp,eax
                 jbe  @@B_S_Loop1

                 popad
                 retn
.DATA?
@@Morf_Coef      dd    ?
.CODE
Morf_Trans_Alpha_Obj  endp

Draw_Alpha_Sprite_With_Z_Buf2   proc
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y
                pushad

mov Sprite_Clip_Adder,0


                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

                mov  Light_Drawed_Pixels_Count,0

pushad
                imul ecx,HOR_RAZR
                add  ebx,ecx
                mov  eax,Cur_Poly_Z
                cmp  ZBuffer[ebx*4],eax
popad
                jbe  @@Not_Draw

shr eax,1
sub ecx,eax                            ; Чтобы были координаты центра спрайта
mov eax,SPRITE_X1
shr eax,1
sub ebx,eax                            ; Чтобы были координаты центра спрайта


                mov eax,ebx
                cmp eax,0
                jge  @@tt6
                neg eax
                mov ebx,0
mov edx,eax
shl edx,2
add esi,edx
mov Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt6:
                mov eax,ecx
                cmp eax,0
                jge  @@tt5
                neg eax
mov edx,eax
imul edx,SPRITE_X1
shl edx,2
add esi,edx
                mov ecx,0
                sub SPRITE_Y1 ,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt5:

                mov eax,ecx
                add eax,SPRITE_Y1
                cmp eax,VERT_RAZR
                jle  @@tt1
                sub eax,VERT_RAZR
                sub SPRITE_Y1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt1:
                mov eax,ebx
                add eax,SPRITE_X1
                cmp eax,HOR_RAZR
                jle  @@tt2
                sub eax,HOR_RAZR
mov edx,eax
shl edx,2
mov Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt2:
                mov eax,ecx
                imul eax,HOR_RAZR
                add eax,ebx
                lea edi,[eax*4]

;                shl SPRITE_X1,2

                mov eax,SPRITE_X1

                shl eax,2

                mov ebp,HOR_RAZR*4

                mov edx,SPRITE_Y1
                sub ebp,eax


@@D_A_S_Loop1:
                mov ecx,SPRITE_X1
@@D_A_S_Loop2:
                mov  eax,Cur_Poly_Z
                cmp  ZBuffer[edi],eax
                jae  @@Ok

                add  esi,4
                add  edi,4
                jmp  @@Next_Cyc
@@Ok:
                inc  Light_Drawed_Pixels_Count
rept 4
local @@Not_Carry3,@@Not_Draw3

                lodsb
                sub al,Alpha_Sprite_Subber
                jc @@Not_Draw3
                jz @@Not_Draw3

                add al,Virt_Screen_1[edi]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov Virt_Screen_1[edi],al
@@Not_Draw3:
                inc edi
endm
@@Next_Cyc:
                dec ecx
                jnz @@D_A_S_Loop2
add esi,Sprite_Clip_Adder
                add edi,ebp
                dec edx
                jnz @@D_A_S_Loop1

@@Not_Draw:

                popad
                retn
Draw_Alpha_Sprite_With_Z_Buf2 endp


Alpha_One_Byte_Optim  macro Offset1
local @@Not_Carry3,@@Not_Draw3
; edx -  offset in texture
                mov   al,[esi+Offset1]
                add al,[edi+Offset1]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov [edi+Offset1],al
@@Not_Draw3:
                endm


Draw_Alpha_Sprite2_Optim   proc
; esi: sprite
; edi: screen ptr
; ebx, ecx: sprite x,y
                pushad

mov Sprite_Clip_Adder,0


                lodsd
                mov SPRITE_X1,eax
                lodsd
                mov SPRITE_Y1,eax

shr eax,1
sub ecx,eax                            ; Чтобы были координаты центра спрайта
mov eax,SPRITE_X1
shr eax,1
sub ebx,eax                            ; Чтобы были координаты центра спрайта

                mov eax,ebx
                cmp eax,0
                jge  @@tt6
                neg eax
                mov ebx,0
mov edx,eax
shl edx,2
add esi,edx
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt6:
                mov eax,ecx
                cmp eax,0
                jge  @@tt5
                neg eax
mov edx,eax
imul edx,SPRITE_X1
shl edx,2
add esi,edx
                mov ecx,0
                sub SPRITE_Y1 ,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt5:

                mov eax,ecx
                add eax,SPRITE_Y1
                cmp eax,VERT_RAZR
                jle  @@tt1
                sub eax,VERT_RAZR
                sub SPRITE_Y1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt1:
                mov eax,ebx
                add eax,SPRITE_X1
                cmp eax,HOR_RAZR
                jle  @@tt2
                sub eax,HOR_RAZR
mov edx,eax
shl edx,2
add Sprite_Clip_Adder,edx

                sub SPRITE_X1,eax
                js  @@Not_Draw
                jz  @@Not_Draw
@@tt2:

                mov eax,ecx
                imul eax,HOR_RAZR
                add eax,ebx
                lea edi,[eax*4+edi]

;                shl SPRITE_X1,2

                mov eax,SPRITE_X1

                shl eax,2

                mov ebp,HOR_RAZR*4

                mov edx,SPRITE_Y1
                sub ebp,eax
@@D_A_S_Loop1:
                mov ecx,SPRITE_X1
@@D_A_S_Loop2:
                Alpha_One_Byte_Optim 2
                
                add esi,4
                add edi,4

                dec ecx
                jnz @@D_A_S_Loop2
add esi,Sprite_Clip_Adder
                add edi,ebp
                dec edx
                jnz @@D_A_S_Loop1

@@Not_Draw:

                popad
                retn
endp
