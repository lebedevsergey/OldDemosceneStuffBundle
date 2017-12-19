INTRO_IMS_PLAYER equ  1
;INTRO_RXM_PLAYER equ  1

NO_VIDEO_SEL   equ 1
;IS_MUSIC equ 1

.386
.MODEL FLAT

include file1.inc


Extrn GETVIDEOFLG_:near
Global pascal intro_ :near

.CODE

intro_ PROC

        pushad
        push ds
        push es

        call GETVIDEOFLG_
        or al,al
        jz @@ttt1
        mov Manual_Vidmode_Flg,1
@@ttt1:
        mov ax,ds
        mov es,ax
        mov gs,ax
        mov fs,ax

        call Player_Init

        call Set_Videomode

include myprog.inc

        call Player_Deinit

        Textmode

        lea  edx,Msg1
        mov  ah,09h
        int 21h


        pop es
        pop ds
        popad
        retn

        include 1.inc
endp
end
