.386
.MODEL FLAT

FLASH  equ  1

include matem.inc

.DATA
Virt_Screen1  dd offset  Virt_Screen_1

Global Virt_Screen1

include rez.inc

Extrn  _TIME :near
Extrn  _INITTIME :near
Extrn  _PARTNUM :near


.DATA?


Texture  db   256*256*4 dup (?)
Texture2  db   256*256*4 dup (?)

db 400h dup (?)

Virt_Screen_1 db   HOR_RAZR*VERT_RAZR*4 dup (?)
Virt_Screen_1_End:

	include 1.inc

.CODE

Global _main
_main:
Intro proc

	    pushad

include intro4k.inc

Abort:

	popad

	retn



.code

endp

end _main
