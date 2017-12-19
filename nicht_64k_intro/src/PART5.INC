Greets_Rec struc
Greets_X    dw   ?
Greets_Y    dw   ?
Greets_Str_Ptr    dd   ?
ends

Greets_Color     equ   0BFh*01010101h
Greets_Max_Tremor   equ  2
Greets_Y_Interval  equ  37

Greets_X1          equ  10
Greets_Y1          equ  5
Greets_Y2          equ  Greets_Y1+Greets_Y_Interval*1
Greets_Y3          equ  Greets_Y1+Greets_Y_Interval*2
Greets_Y4          equ Greets_Y1+Greets_Y_Interval*3
Greets_Y5          equ  Greets_Y1+Greets_Y_Interval*4
Greets_Y6          equ  Greets_Y1+Greets_Y_Interval*5

Greets_X7          equ  100
Greets_X8          equ  100
Greets_X9          equ  100
Greets_X10          equ  100
Greets_X11          equ  100
.CODE

Draw_Credit  proc
; esi - greet rec
;ecx,edx - X,Y
;eax - color
             pushad
             xor   ebp,ebp
@@Next:
             inc   ebp
             push  esi

             movzx ecx,[esi].Greets_X
             movzx edx,[esi].Greets_Y
             mov esi,[esi].Greets_Str_Ptr

             mov eax,Greets_Max_Tremor*2
             call Random
             sub eax,Greets_Max_Tremor
             add  ecx,eax
             mov eax,Greets_Max_Tremor*2
             call Random
             sub eax,Greets_Max_Tremor
             add  edx,eax

             lea  ebx,AA_Font1

             cmp  ebp,Highlight_Greet_Num
             jz   @@High
             call Draw_Proportional_String2
             jmp  @@Ok
@@High:
             call Draw_Proportional_String
@@Ok:

             pop esi

             add esi,SIZE Greets_Rec
             cmp byte ptr [esi],0FFh
             jnz @@Next

             popad
retn
endp

Init_Part5 proc near

        mov  eax,Time2
        mov  Base_Time1,eax

        mov edi,Virt_Screen1
        mov ecx,HOR_RAZR*VERT_RAZR
        xor eax,eax
        rep stosd

        mov Font1.Font_X_Interval,0

        retn
        endp

.CODE
Part5   proc near

        fild  Tab_Tex_Add1_Coef
        call  Line_Param
        fistp Table_Tex_Adder


        fild   Tab_X_Coef2_2
        fild  Tab_X_Coef2_1
        call  Sin_Param
        fistp Tmp2
        mov   ecx,Tmp2
        add ecx,HOR_RAZR

;        mov ecx,190
        mov edx,VERT_RAZR
        lea ebp,Texture1_1
        lea esi,Lookup_Table
        mov edi,Virt_Screen4
        call Draw_Table_Tex

        fild  Tab_Tex_Add2_Coef
        call  Line_Param
        fistp Table_Tex_Adder

        fild   Tab_X_Coef1_2
        fild  Tab_X_Coef1_1
        call  Sin_Param
        fistp Tmp2
        mov   ecx,Tmp2
        add ecx,HOR_RAZR/2
;mov ecx,160
        mov edx,0
        lea ebp,Texture1_2
        lea esi,Lookup_Table
        mov edi,Virt_Screen3
        call Draw_Table_Tex

cmp Greets1_Flg,1
jz @@Tab2
        lea  esi,Times_Tab5_1
        call Do_Time_Tab
        lea  esi,Greets_Rec1_
        jmp  @@yy1
@@Tab2:
        lea  esi,Times_Tab5_2
        call Do_Time_Tab
        lea  esi,Greets_Rec2_
@@yy1:

        mov edi,Virt_Screen4

        call Draw_Credit


        mov alpha1,40
        mov alpha2,40
        mov esi,Virt_Screen3
        mov edi,Virt_Screen4
        call Alpha_Virt_Screen

        mov esi,Virt_Screen4
        mov edi,Virt_Screen1
        call Motion_Blur


;        mov esi,Virt_Screen1
 ;       mov edi,Virt_Screen4
  ;      mov ecx,HOR_RAZR*VERT_RAZR
   ;     rep movsd


        lea  esi,Times_Tab5
        call Do_Time_Tab

retn
endp


.DATA
Greet_Str1             db  'HAUJOBB',0
Greet_Str2             db  'SANDS',0
Greet_Str3             db  'LOOKER HOUSE',0
Greet_Str4             db  'EXCESS TEAM',0
Greet_Str5             db  'PROXIUM',0
Greet_Str6             db  'T REX',0
Greet_Str7             db  'TRANSMISSION',0
Greet_Str8             db  'STORM',0
Greet_Str9             db  'EX NEONRAY',0
Greet_Str10             db  'ADELAIDA',0

Tab_X_Coef1_1     dd   600000
Tab_X_Coef1_2     dd   HOR_RAZR/4

Tab_X_Coef2_1     dd   700000
Tab_X_Coef2_2     dd   -HOR_RAZR/2

Greets_Rec1_:
Greets_Rec1 Greets_Rec<Greets_X1,Greets_Y1,Greet_Str1>
Greets_Rec2 Greets_Rec<Greets_X1,Greets_Y2,Greet_Str2>
Greets_Rec3 Greets_Rec<Greets_X1,Greets_Y3,Greet_Str3>
Greets_Rec4 Greets_Rec<Greets_X1,Greets_Y4,Greet_Str4>
Greets_Rec5 Greets_Rec<Greets_X1,Greets_Y5,Greet_Str5>
db 0FFh

Greets_Rec2_:
Greets_Rec7 Greets_Rec<Greets_X7,Greets_Y1,Greet_Str6>
Greets_Rec8 Greets_Rec<Greets_X8,Greets_Y2,Greet_Str7>
Greets_Rec9 Greets_Rec<Greets_X9,Greets_Y3,Greet_Str8>
Greets_Rec10 Greets_Rec<Greets_X10,Greets_Y4,Greet_Str9>
Greets_Rec11 Greets_Rec<Greets_X11,Greets_Y5,Greet_Str10>
db 0FFh

Highlight_Greet_Num  dd  1
Greets1_Flg          db  0