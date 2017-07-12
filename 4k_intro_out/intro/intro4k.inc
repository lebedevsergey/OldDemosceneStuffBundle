.CODE
include time.inc

	lea ebp,All_Data

	cmp Init_Flag,0
	jnz @@No_Init

	mov Init_Flag,1

	Calc_Table

	xor bh,bh
	lea edi,Texture
	call Do_Tlo
	inc bh
	lea edi,Texture2
	call Do_Tlo

@@No_Init:
	mov edi,Virt_Screen1
pushad

mov eax,dword ptr _TIME
shr eax,4
and ax,1ffh
mov [ebp].Rotoz_Angle,ax

shr ax,1
mov [ebp].Tmp4,ax
fild [ebp].Tmp4
fmul Radian_Coef
fcos

mov [ebp].Tmp4,120
fimul [ebp].Tmp4
mov [ebp].Tmp4,230
fiadd [ebp].Tmp4
fistp [ebp].Rotoz_Distant

mov ax,[ebp].Rotoz_Distant
mov [ebp].Alpha,al

	cmp byte ptr _PARTNUM,3
	jz @@tt0
	cmp byte ptr _PARTNUM,1
	jnz @@rotoz
@@tt0:
		cmp  dword ptr _TIME,PART4_TIME
		ja @@rotoz
@@tt_0:
	Draw_Table

	jmp  @@tt_ex

@@rotoz:

mov eax,dword ptr _TIME
shr eax,4
and ax,1ffh
mov [ebp].Tmp4,ax
fild [ebp].Tmp4
fmul Radian_Coef
fcos
fimul Sin_Coef
fistp [ebp].Rotoz_Distant
add [ebp].Rotoz_Distant,50

Rotozoom

		cmp  dword ptr _TIME,PART4_TIME
		ja @@tt_0

@@tt_ex:
popad
;	mov edi,Virt_Screen1

	mov eax,dword ptr _TIME

	cmp eax,INVERT_TIME_START
	jb  @@No_Invert
	Invert_Virt_Screen
@@No_Invert:
	cmp eax,FLASH_START_TIME1
	jb  @@No_Flash
	cmp eax,FLASH_END_TIME1
	jb  @@tt3

	cmp eax,FLASH_START_TIME2
	jb  @@No_Flash
	cmp eax,FLASH_END_TIME2
	ja  @@No_Flash
@@tt3:

IFDEF FLASH
Fade_White_Virt_Screen
ENDIF

@@No_Flash: