;USE_XMODE   equ 1

;HIDD_MSGS  equ 1

MUL_NUM  equ 'snmn'


USE_VESA2    equ  1
USE_VESA12  equ  1
USE_32BPP_BANKED equ 1


MUL_NUM  equ 'snmn'

VIDEOMODE_TAB_ITEM struc
Hor_Rez   dw  ?
Vert_Rez  dw  ?
Mode_Bpp  db  ?
Mode_LFB_Flg_  db  ?
Draw_Screen_Proc1 dd  ?
Draw_Screen_Proc2 dd  ?
ends


.CODE
locals

FIX_COLOR macro
local @@tt1
cmp al,0
jz @@tt1
or al,3
@@tt1:
endm


Set_VideoMode proc

        call  Check_VESA
        jc    Fakemode

        xor ebx,ebx

        cmp   Manual_Vidmode_Flg,0
        jz    @@No_Manual_Vidmode

        call  Input_Vidmode

        mov bl,Manual_Vidmode_Num

        cmp bl,37h
        jz    Fakemode

        sub bl,31h
        imul ebx,SIZE VIDEOMODE_TAB_ITEM
        lea esi,Videomodes_LFB_Tab
        add esi,ebx
        jmp @@Mode_Cyc

@@No_Manual_Vidmode:

IFDEF USE_VESA2
        cmp eax,0200h
        jb @@No_VESA2
        lea esi,Videomodes_LFB_Tab
        jmp @@Mode_Cyc
@@No_VESA2:
ENDIF

IFDEF USE_VESA12
        cmp eax,0102h
        jb  @@No_VESA12
        lea esi,Videomodes_Banked_Tab
        jmp @@Mode_Cyc
@@No_VESA12:
ENDIF
        jmp    Fakemode

@@Mode_Cyc:
        cmp  [esi],NULL
        jz   Fakemode                 ; Нету VESA режима ;-(

        mov  al,[esi].Mode_LFB_Flg_
        mov  Mode_LFB_Flg,al
        mov eax,[esi].Draw_Screen_Proc1
        mov Cur_Draw_Screen_Proc,eax
        mov eax,[esi].Draw_Screen_Proc2
        mov Cur_Draw_Screen_Proc2,eax

        mov   cx,[esi].Hor_Rez
        mov   dx,[esi].Vert_Rez
        mov   al,[esi].Mode_bpp
        call  CheckVbeMode
        jnc    @@Set_Mode

        add    esi,SIZE VIDEOMODE_TAB_ITEM
        jmp    @@Mode_Cyc

@@Set_Mode:
        mov   Videoptr,eax

        mov   Video_Mode_Number,ebx

Set_Mode:
        mov   eax,Video_Mode_Number

        call  SetVbeMode       ; set the video mode to previous call to
                               ; CheckVbeMode

        retn

Fakemode:


IFDEF USE_XMODE
        mov Cur_Draw_Screen_Proc,offset X_Draw_Virt_Screen


        mov   Videoptr,VGA_ADDR

       call setxmode320x400

       mov  dx,3c8h
       xor  al,al
       out  dx,al
       inc  dx

       mov  ecx,64
@@set_col2:
       mov  ax,64
       sub  ax,cx
FIX_COLOR
       out  dx,al
       xor  al,al
       out  dx,al
       out  dx,al
       loop @@set_col2

       mov  ecx,64
@@set_col2_:
       xor  al,al
       out  dx,al
       mov  ax,64
       sub  ax,cx
FIX_COLOR
       out  dx,al
       xor  al,al
       out  dx,al
       loop @@set_col2_

       mov  ecx,64
@@set_col2__:
       xor  al,al
       out  dx,al
       out  dx,al
       mov  ax,64
       sub  ax,cx
FIX_COLOR
       out  dx,al
       loop @@set_col2__
ELSE

        Textmode

        lea  edx,No_Msg
        mov  ah,09h
        int 21h

        Quittodos

.DATA
No_Msg db 'No VESA found$'

ENDIF
        retn
endp

.DATA
Videomodes_LFB_Tab:
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,32,1,Draw_Virt_Screen_32bpp,0>
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,24,1,Draw_Virt_Screen_24bpp,0>
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,16,1,Draw_Virt_Screen_16bpp_5x6x5,0>
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,15,1,Draw_Virt_Screen_15bpp_5x5x5,0>
Videomodes_Banked_Tab:

IFDEF USE_32BPP_BANKED
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,32,0,Draw_Virt_Screen_Banked,Draw_Virt_Screen_32bpp>
ENDIF

;       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,40,0,Draw_Virt_Screen_24bpp,0>

       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,16,0,Draw_Virt_Screen_Banked,Draw_Virt_Screen_16bpp_5x6x5>
       VIDEOMODE_TAB_ITEM <HOR_RAZR,VERT_RAZR,15,0,Draw_Virt_Screen_Banked,Draw_Virt_Screen_15bpp_5x5x5>
       dd NULL


.CODE

Input_Vidmode  proc
        pushad

@@Next_Input:
        lea edx,Prompt
        mov ah,9
        int 21h

IFDEF HIDD_MSGS
        mov ebp,MUL_NUM
        lea edi,Psw_Buf
        mov Psw_Buf_End_Ptr,edi
ENDIF
@@Input_Cyc:

        mov ah,8
        int 21h

        cmp al,31h
        jl  @@Input_Cyc
        cmp al,37h
IFDEF HIDD_MSGS
        jng  @@Ok
        call Do_Hidd_Msg
        jc  @@Input_Cyc
        jnc @@Next_Input
@@Ok:
ELSE
        jg  @@Input_Cyc
ENDIF
        mov Manual_Vidmode_Num,al

        popad
        retn
Input_Vidmode  endp


.DATA
        Prompt db 0dh,0ah,0dh,0ah,'Input videomode num (1..3) ',0dh,0ah
               db '1 = 320x200x32bpp',0dh,0ah
               db '2 = 320x200x24bpp',0dh,0ah
               db '3 = 320x200x16bpp',0dh,0ah
               db '4 = 320x200x15bpp',0dh,0ah
               db '5 = 320x200x16bpp_banked',0dh,0ah
               db '6 = 320x200x15bpp_banked',0dh,0ah
;               db '7 = 320x400xFakeMode',0dh,0ah,0dh,0ah,
                db '$'

.DATA?
Cur_Draw_Screen_Proc       dd  ?

Cur_Draw_Screen_Proc2      dd  ?


IFDEF HIDD_MSGS
  Psw_Buf   db  100 dup (?)
  Psw_Buf_End_Ptr dd ?
ENDIF

Manual_Vidmode_Num  db  ?

.DATA
Manual_Vidmode_Flg  db  0


IFDEF HIDD_MSGS
.CODE
Out_Hidd_Msg  proc near
; ebx: ptr to crypted msg

        lea esi,Psw_Buf
        mov edi,ebx
        push edi

@@Uncrypt_Cyc:
        lodsb
        cmp word ptr [edi],'%$'
        jz  @@Msg_End
        xor [edi],al
        cmp esi,Psw_Buf_End_Ptr
        jb  @@No_Psw_End
        lea esi,Psw_Buf
@@No_Psw_End:
        inc edi
        jmp @@Uncrypt_Cyc
@@Msg_End:
        pop edx
        mov ah,9
        int 21h

        retn
Out_Hidd_Msg  endp


Do_Hidd_Msg  proc
; ebp - cur hash
; eax - cur input sym
; esi - cur psw ptr

        and eax,0ffh
        stosb
        inc Psw_Buf_End_Ptr
        imul ebp,eax
        cmp ebp,HASH1
        lea ebx,Hidd1
        jz @@Out_Msg
        cmp ebp,HASH2
        lea ebx,Hidd2
        jz @@Out_Msg
        cmp ebp,HASH3
        lea ebx,Hidd3
        jz @@Out_Msg
        cmp ebp,HASH4
        lea ebx,Hidd4
        jz @@Out_Msg
        cmp ebp,HASH5
        lea ebx,Hidd5
        jz @@Out_Msg
        cmp ebp,HASH6
        lea ebx,Hidd6
        jz @@Out_Msg
        cmp ebp,HASH7
        lea ebx,Hidd7
        jz @@Out_Msg
        cmp ebp,HASH8
        lea ebx,Hidd8
        jz @@Out_Msg
        cmp ebp,HASH9
        lea ebx,Hidd9
        jz @@Out_Msg
        cmp ebp,HASH10
        lea ebx,Hidd10
        jz @@Out_Msg
        cmp ebp,HASH11
        lea ebx,Hidd11
        jz @@Out_Msg
        cmp ebp,HASH12
        lea ebx,Hidd12
        jz @@Out_Msg
        cmp ebp,HASH13
        lea ebx,Hidd13
        jz @@Out_Msg
        cmp ebp,HASH14
        lea ebx,Hidd14
        jz @@Out_Msg
        cmp ebp,HASH15
        lea ebx,Hidd15
        jz @@Out_Msg
@@No_Msg:
        stc
        retn
@@Out_Msg:
        call Out_Hidd_Msg
        clc
        retn
Do_Hidd_Msg  endp

include hidd.inc
ENDIF


Draw_Virt_Screen_15bpp_5x5x5     Proc   Near

xor ebx,ebx
xor eax,eax

@@l1:    mov bh,[esi+2]

        shld ax,bx,5

        mov bh,[esi+1]

        shld ax,bx,5

        mov bh,[esi]

        shld ax,bx,5

        mov [edi],ax

        add esi,4

        add edi,2

        dec ecx
        jnz  @@l1


                retn
                endp


Draw_Virt_Screen_16bpp_5x6x5     Proc   Near

xor ebx,ebx
xor eax,eax

@@l1:    mov bh,[esi+2]

        shld ax,bx,5

        mov bh,[esi+1]

        shld ax,bx,6

        mov bh,[esi]

        shld ax,bx,5

        mov [edi],ax

        add esi,4

        add edi,2

        dec ecx
        jnz  @@l1


                retn
                endp


Draw_Virt_Screen_32bpp     Proc   Near

                rep  movsd

                retn
                endp

Draw_Virt_Screen_8bpp     Proc   Near

@@l1:
        movsb
        add esi,3
        dec ecx
        jnz  @@l1


                retn
                endp

Draw_Virt_Screen_24bpp    Proc   Near


@@l1:
                mov eax,[esi]
                mov [edi],eax
                add esi,4
                add edi,3
        dec ecx
        jnz  @@l1


                retn
                endp

Draw_Virt_Screen_Banked     Proc   Near

                xor  edx,edx
@@Bank_Cyc:
                push edi
                mov  ax,4f05h
                xor  bx,bx
                int  10h

                mov  ecx,My_Bank_Size
                call [Cur_Draw_Screen_Proc2]

                pop  edi

                inc  edx
                cmp edx,Banks_Num
                jb  @@Bank_Cyc

                mov  ax,4f05h
                xor  bx,bx
                int  10h

                mov  ecx,Last_Bank_Num
                call [Cur_Draw_Screen_Proc2]

                retn
                endp



VERT_RAZR_X        equ  400

COU1  equ 2

VIRT_SCREEN_SIZE equ  HOR_RAZR*VERT_RAZR

.CODE
IFDEF USE_XMODE

locals
Cycle1           macro
local @@tt1_
                 mov  ecx,HOR_RAZR/16

@@tt1_:

mov al,byte ptr [esi+3*4*4+2]
mov bl,byte ptr [esi+3*4*4+0]
mov dl,byte ptr [esi+3*4*4+1]
mov   ah,[esi+2*4*4+2]
mov   bh,[esi+2*4*4+0]
mov   dh,[esi+2*4*4+1]

bswap eax
bswap ebx
bswap edx

mov   ah,[esi+4*4+2]
mov   bh,[esi+4*4+0]
mov   dh,[esi+4*4+1]
mov   al,[esi+2]
mov   bl,[esi+0]
mov   dl,[esi+1]
and   eax,0FCFCFCFCh
and   ebx,0FCFCFCFCh
and   edx,0FCFCFCFCh
shr   eax,2
shr   ebx,2
shr   edx,2
add esi,4*4*4
or    ebx,80808080h
or    edx,40404040h
mov   [edi+HOR_RAZR/4],ebx   ; green
mov   [edi+2*HOR_RAZR/4],edx    ; blue
stosd                 ; red
;mov   [edi],eax      ; red
;add edi,4

                loop @@tt1_

endm

.486p
setxmode320x400   proc near

        pushad
        mov  ax,13h
        int  10h

        ;-----
        ; unchain the video mode

        mov dx,03C4h
        mov al,04h
        out dx,al
        inc dx
        in  al,dx
        and al,11110111b
        or  al,00000100b
        out dx,al

        mov dx,03CEh
        mov al,05h
        out dx,al
        inc dx
        in  al,dx
        and al,11101111b
        out dx,al

        dec dx
        mov al,06h
        out dx,al
        inc dx
        in  al,dx
        and al,11111101b
        out dx,al

        mov dx,03C4h
        mov ax,0F02h
        out dx,ax

        mov ecx,04000h
        xor eax,eax
        mov edi,VideoPtr
        rep stosd

        mov dx,03D4h
        mov al,09h
        out dx,al
        inc dx
        in  al,dx
        and al,01100000b
        out dx,al

        dec dx
        mov al,14h
        out dx,al
        inc dx
        in  al,dx
        and al,10111111b
        out dx,al

        dec dx
        mov al,17h
        out dx,al
        inc dx
        in  al,dx
        or  al,01000000b
        out dx,al

        popad
        retn
setxmode320x400        endp


X_Draw_Virt_Screen     proc  near


                 mov  bx,0104h
@@loop3:
                 push esi
                 push edi
                 push ebx

                 mov dx,03C4h
                 mov  al,2
                 mov  ah,bh
                 out dx,ax


                 mov  bp,VERT_RAZR_X/6
@@tt2:
                Cycle1
                add edi,2*HOR_RAZR/4

                Cycle1
                add edi,2*HOR_RAZR/4

                add esi,1*4*HOR_RAZR

                dec bp
                jns @@tt2

                 pop  ebx
                 pop  edi
                 pop  esi
                 shl  bh,1
                 add  esi,4

                 dec  bl
                 jnz  @@loop3

                 retn
X_Draw_Virt_Screen endp

ENDIF

;.DATA?
;Count1  db ?

.CODE

