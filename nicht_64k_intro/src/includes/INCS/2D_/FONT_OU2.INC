.CODE

Gen_One_Letter   proc
; eax: Letter
; ebx: font record
; edi - cur obj ptr
                pushad

                cmp  eax,' '
                jz   @@Just_A_Space

                mov edi,Cur_Text_Obj_Ptr

                mov  ecx,[ebx].Font_Letter_X_Size
                shl  ecx,2
sub eax,'A'
                imul eax,ecx
add eax,8
                mov  esi,[ebx].Font_Ptr
                add  esi,eax

                mov  edx,[ebx].Font_Letter_Y_Size

                mov  eax,Start_Y
                mov  @@Cur_Y,eax
@@D_S_Loop1:
                mov  ecx,[ebx].Font_Letter_X_Size

                mov  eax,Start_X
                mov  @@Cur_X,eax
                push esi
@@D_S_Loop2:
                lodsd
                or eax,eax
                jz @@Not_Draw2

                cmp  edi,[ebp].LAST_VERTEX
                ja   @@Obj_End

                fild @@Cur_X
                fstp [edi].X
                fild @@Cur_Y
                fstp [edi].Y
                fild Text_Z
                fstp [edi].Z
                add  edi,SIZE TVERTEX
@@Not_Draw2:
                mov eax,X_Pixel_Interval
                add @@Cur_X,eax

                dec ecx
                jnz @@D_S_Loop2
@@Obj_End:
                pop esi
                add esi,[ebx].Font_X_Size_

                mov eax,Y_Pixel_Interval
                add @@Cur_Y,eax
                dec edx
                jnz @@D_S_Loop1

                mov Cur_Text_Obj_Ptr,edi
@@Just_A_Space:
                popad
                retn
.DATA?
@@Cur_X         dd    ?
@@Cur_Y         dd    ?
.CODE
Gen_One_Letter  endp

Gen_Text_Obj    proc
; esi: string
; ebx: font rec
; ecx: X coord
; edx: Y Coord
; eax: Z Coord
; ebp: ptr to object

 push [ebx].Font_X_Interval

                pushad

                mov  Text_Z,eax
                mov  edi,[ebp].FIRST_VERTEX
                mov  Cur_Text_Obj_Ptr,edi
                mov  Start_X,ecx
                mov  Start_Y,edx

                mov  edx,[ebx].Font_Letter_X_Size
                add  edx,[ebx].Font_X_Interval
                imul edx,X_Pixel_Interval

                xor  eax,eax
@@String_Cyc:
                lodsb
                or  al,al
                jz  @@String_End

                call Gen_One_Letter

                sub  Start_X,edx
                jmp @@String_Cyc

@@String_End:
                mov  edi,Cur_Text_Obj_Ptr
                sub  edi,SIZE TVERTEX
                mov  [ebp].LAST_VERTEX,edi

                popad
 pop [ebx].Font_X_Interval

                retn
Gen_Text_Obj    endp

.DATA
Max_Part_Bright_Subber   dd  Max_Part_Bright_Subber_
X_Pixel_Interval   dd  X_Pixel_Interval_
Y_Pixel_Interval   dd  Y_Pixel_Interval_

.DATA?
Start_X           dd  ?
Start_Y           dd  ?
Text_Z            dd  ?
Cur_Text_Obj_Ptr  dd  ?
