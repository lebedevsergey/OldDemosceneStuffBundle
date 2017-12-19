SPRITE1_X_SIZE  equ  640
SPRITE1_Y_SIZE  equ  112;118
SPRITE1_X_POS  equ  0
SPRITE1_Y_POS  equ  40 ;6

PART_OBJ_MAX_X  equ  190
PART_OBJ_MAX_Y  equ  130
PART_OBJ_Z      equ  30
PART_OBJ_NUM    equ  5000

Z_Buff_Value  equ  0FF000000h

Load_Fonts  proc

mov Invert_Rle_Flg,0

        mov  eax,SPRITE1_X_SIZE
        mov  ebx,SPRITE1_Y_SIZE
        lea  esi,Sprite1_Data
        lea  edi,Head_Sprite
        call Unp_Rle_Spr

mov Invert_Rle_Flg,0

        mov eax,FONT_X_SIZE
        mov ebx,FONT_Y_SIZE
        lea  esi,Font
        lea  edi,Font1_Sprite
        call Unp_Rle_Spr

        call Unp_Raw

        retn
Load_Fonts  endp

Update_Font_Particles  proc near
                pushad
                mov  eax,Time2
                sub  eax,Base_Time1
                mov  Tmp2,eax
                fild Tmp2
                fidiv Particle_Update_Coef1
@@tt1:
                fld  st
                fadd [esi].Part_Bright_Subber_Time
                fsin
fmul Part_Bright_Coef3
;fadd st,st
                fabs
                fstp  [esi].Part_Bright_Subber

                add  esi,SIZE FONT_PARTICLE
                dec  ecx
                jnz  @@tt1

                fstp  st
                popad
                retn
Update_Font_Particles  endp

Init_Font_Particles  proc near
                pushad
@@tt1:
                mov  eax,0FFFFh
                call random
                call random
                mov  Tmp2,eax
                fild Tmp2
                mov  Tmp2,0FFFFh
                fidiv Tmp2
                fldpi
                fadd  st,st
                fmulp st(1),st
                fstp  [esi].Part_Bright_Subber_Time

                add  esi,SIZE FONT_PARTICLE
                dec  ecx
                jnz  @@tt1
                popad
                retn
Init_Font_Particles  endp

.DATA
Particle_Update_Coef1    dd  5000000;1000000;0

.DATA?
Font_Particles    db  MY_FONT_X_SIZE*MY_FONT_Y_SIZE*SIZE FONT_PARTICLE*20 dup(?)


.CODE
Do_Textures proc

IFDEF      GEN_TEX
            lea esi,Text_1_1
            lea edi,Texture
            call Do_Texture

            lea esi,Text_1_1
            lea edi,Texture1_1
            call Do_Texture

            lea esi,Text_1_2
            lea edi,Texture1_2
            call Do_Texture

            lea esi,Text4_1
            lea edi,Texture4_1
            call Do_Texture

            lea esi,Text4_1_2
            lea edi,Texture4_1_2
            call Do_Texture

            lea esi,Text4_2
            lea edi,Texture4_2
            call Do_Texture
ENDIF
            lea esi,Lightmap1
            lea edi,Lightmap1_
            call Do_Texture

mov MIN_PHONG1,120;140
fld Phong1_Coef_5
fstp Phong1_Coef
lea esi,Lightmap3
lea edi,Texture5_1
call Do_Texture

lea esi,Lightmap4
lea edi,Texture7_1
call Do_Texture

            retn
Do_Textures endp

Do_Tables   proc
            lea esi,Table_Form2
            lea edi,Lookup_Table
            call Init_Table2

            retn
Do_Tables   endp


Do_Objects  proc

;           lea esi,Big_Torus2__
 ;          lea edi,Object4_4
  ;         call Do_Object


        retn
        endp


.DATA?
 Object1 TMODEL <?>
 Object2 TMODEL <?>
 Object3 TMODEL <?>



.CODE
Draw_Alpha_Sprite_With_Z_Buf   proc
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
                lea edi,[eax*4+edi]

;                shl SPRITE_X1,2

                mov eax,SPRITE_X1

                shl eax,2

                mov ebp,HOR_RAZR*4

                mov edx,SPRITE_Y1
                sub ebp,eax

                mov  Light_Drawed_Pixels_Count,0
@@D_A_S_Loop1:
                mov ecx,SPRITE_X1
@@D_A_S_Loop2:

                cmp  dword ptr [edi],Z_Buff_Value
                jnz  @@Ok
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

                add al,[edi]
                jnc @@Not_Carry3
                mov al,0FFh
@@Not_Carry3:
                mov [edi],al
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
Draw_Alpha_Sprite_With_Z_Buf endp

.DATA
Part_Bright_Coef3  dd 2.0

.DATA?
Light_Drawed_Pixels_Count    dd    ?

Head_Sprite   dd  ?,?,SPRITE1_X_SIZE*SPRITE1_Y_SIZE*2 dup (?)

.CODE

Gen_Part_Obj1  proc near
; ebx - ptr to particle object
                pushad

                mov  ebp,[ebx].FIRST_VERTEX
                mov  edi,[ebx].LAST_VERTEX
@@Part_Cyc:
                mov  eax,PART_OBJ_MAX_X*2
                call Random
                sub  eax,PART_OBJ_MAX_X
                mov  Tmp2,eax
                fild Tmp2
                fstp [ebp].VX
                mov  eax,PART_OBJ_MAX_Y*2
                call Random
                sub  eax,PART_OBJ_MAX_Y
                mov  Tmp2,eax
                fild Tmp2
                fstp [ebp].VY
                mov  eax,PART_OBJ_Z
                mov  Tmp2,eax
                fild Tmp2
                fstp [ebp].VZ

                add  ebp,SIZE TVERTEX
                cmp  ebp,edi
                jbe  @@Part_Cyc

                sub  ebp,SIZE TVERTEX
                mov  [ebx].LAST_VERTEX,ebp

                popad
                retn
Gen_Part_Obj1  endp
