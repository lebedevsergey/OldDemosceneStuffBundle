
GEN_TEX     equ 1

.386
.MODEL FLAT

CUT_NEAR_POLY equ 1
POLY_CLIP     equ  1
NO_VIDEO_SEL  equ 1

include f:\demo_sb\player\incs\vesa.inc
include f:\dem\common\incs\macros.inc                ; include macros

.STACK 1000h

.DATA
msg1  db 0dh,0ah,'Code - SunmaN     Music - Manwe                                     Throb, 2001 ',0dh,0ah,'$'

Virt_Screen1  dd offset  Virt_Screen_1
Virt_Screen3  dd offset  Virt_Screen_2
Virt_Screen4  dd offset  Virt_Screen_3


.DATA?
Tmp1        dw  ?
Tmp2        dd  ?
Tmp3        dd ?
Tmp4        dd   ?

.DATA?
   db 400 dup (?)
   __psp   dw  ?
   mxmdata db 4000h dup (?)
   db 400 dup (?)


VideoPtr    dd  ?
Video_Mode_Number dd ?

db 300 dup (?)
Texture       db   256*256*4 dup (?)
Texture2      db   256*256*4 dup (?)

Texture1_1    db   256*256*4 dup (?)
Texture1_2    db   256*256*4 dup (?)

Texture4_1    db   256*256*4 dup (?)
Texture4_2    db   256*256*4 dup (?)
Texture4_1_2    db   256*256*4 dup (?)

Texture5_1    db   256*256*4 dup (?)
Lightmap1_    db   256*256*4 dup (?)

Texture7_1    db   256*256*4 dup (?)

Virt_Screen_1 db   HOR_RAZR*VERT_RAZR*4 dup (?)
Virt_Screen_2 db   HOR_RAZR*VERT_RAZR*4 dup (?)
Virt_Screen_3 db   HOR_RAZR*VERT_RAZR*4 dup (?)

dd 300 dup(?)

Lookup_Table    dw   HOR_RAZR*VERT_RAZR*4 dup (?)
