FONT7_X  equ  4
FONT7_Y  equ  9

.CODE


FONT_RECORD   struc
  Font_Letter_X_Size      dd  ?
  Font_Letter_Y_Size      dd  ?
  Font_X_Size_            dd  ?
  Font_Ptr                dd  ?
  Font_X_Interval         dd  ?
  Font_Spacing_Ptr        dd  ?
ENDS


Draw_Letter   proc
; eax: Letter
; ebx: font record
                pushad

                cmp  eax,' '
                jz   @@Just_A_Space

                mov  ecx,[ebx].Font_Letter_X_Size
                shl  ecx,2
sub eax,'A'
                imul eax,ecx
add eax,8
                mov  esi,[ebx].Font_Ptr
                add  esi,eax

                mov ebp,HOR_RAZR*4
                sub ebp,ecx

                mov edx,[ebx].Font_Letter_Y_Size
@@D_S_Loop1:
                mov  ecx,[ebx].Font_Letter_X_Size
                push esi
                push edx
                mov  edx,Cur_Font_Color
@@D_S_Loop2:
                lodsd
                or  eax,eax
                jz  @@Not_Draw2
;                cmp eax,0FF000000h
 ;               jz  @@tt3
  ;              mov eax,edx
@@tt3:
                mov [edi],eax

@@Not_Draw2:
                add  edi,4
                dec ecx
                jnz @@D_S_Loop2

                pop edx
                pop esi

                add edi,ebp
                add esi,[ebx].Font_X_Size_
                dec edx
                jnz @@D_S_Loop1

@@Just_A_Space:
                popad
                retn

endp

Draw_String   proc
; esi: string
; edi: Screen ptr
; eax: color
; ebx: font rec
; ecx: X coord
; edx: Y Coord
                pushad

cmp ecx,HOR_RAZR
jae @@String_End
cmp edx,VERT_RAZR
jae @@String_End

                mov  Cur_Font_Color,eax


                cmp Write_String2_Flg,0
                jz @@rr3

                mov eax,[ebx].Font_Letter_Y_Size
                shr eax,1
                sub  edx,eax
                mov eax,[ebx].Font_Letter_X_Size
                shr eax,1
                sub  ecx,eax
@@rr3:

                imul edx,HOR_RAZR
                add  edx,ecx
                lea edi,[edx*4+edi]

                mov  edx,[ebx].Font_Letter_X_Size
                add  edx,[ebx].Font_X_Interval
                shl  edx,2

                xor  eax,eax
@@tt1:
                lodsb
                or  al,al
                jz  @@String_End

;cmp Invert_String_Flg,0
;jz @@rr1
;                call Draw_Invert_Letter
;jmp @@rr2

@@rr1:
                call Draw_Letter
@@rr2:

                add edi,edx
                jmp @@tt1

@@String_End:

                popad
                retn
endp

Draw_Proportional_String   proc
; esi: string
; edi: Screen ptr
; eax: color
; ebx: font rec
; ecx: X coord
; edx: Y Coord
                pushad

cmp ecx,HOR_RAZR
jae @@String_End
cmp edx,VERT_RAZR
jae @@String_End

                mov  Cur_Font_Color,eax


                cmp Write_String2_Flg,0
                jz @@rr3

                mov eax,[ebx].Font_Letter_Y_Size
                shr eax,1
                sub  edx,eax
                mov eax,[ebx].Font_Letter_X_Size
                shr eax,1
                sub  ecx,eax
@@rr3:

                imul edx,HOR_RAZR
                add  edx,ecx
                lea edi,[edx*4+edi]

;                mov  edx,[ebx].Font_Letter_X_Size
                mov  edx,[ebx].Font_X_Interval
                shl  edx,2

                mov  ebp,[ebx].Font_Spacing_Ptr
@@tt1:
                xor  eax,eax
                lodsb
                or  al,al
                jz  @@String_End

;cmp Invert_String_Flg,0
;jz @@rr1
;                call Draw_Invert_Letter
;jmp @@rr2

@@rr1:
                call Draw_Letter
@@rr2:
                cmp  eax,' '
                jz   @@It_Was_Space
                sub  eax,'A'
                movzx eax,byte ptr [ebp+eax]       ; Spacing in eax
                shl  eax,2
                add edi,eax
                add edi,edx
                jmp @@tt1
@@It_Was_Space:
                add edi,[ebx].Font_Letter_X_Size
                add edi,edx
                jmp @@tt1

@@String_End:

                popad
                retn
endp




.DATA
Invert_String_Flg db 0
Write_String2_Flg db 0

Font1_Spacing  db  15,15,15,16,15,11,16,16,9 ,9 ,14
                 ; A  B  C  D  E  F  G  H  I  J  K
               db  9 ,23,16,15,16,16,11,14,10,17,16
                 ; L  M  N  O  P  Q  R  S  T  U  V
               db  21,16,16,16
                 ; W  X  Y  Z

;Font1_Spacing  db  21,21,21,21,21,21,21,21,21,21,21
;                ; A  B  C  D  E  F  G  H  I  J  K
;              db  21,21,21,21,21,21,21,21,21,21,21
;                ; L  M  N  O  P  Q  R  S  T  U  V
;              db  21,21,21,21
;                ; W  X  Y  Z

.DATA?
Cur_Font_Color   dd ?

.DATA
Font1  FONT_RECORD <LETTER_X_SIZE,FONT_Y_SIZE,FONT_X_SIZE*4,Font1_Sprite,0,Font1_Spacing>
Font2  FONT_RECORD <LETTER_X_SIZE*3,FONT_Y_SIZE*3,FONT_X_SIZE*4*3,Font2_Sprite,0>