.CODE
locals
RotoZoom        macro

pushad
                fild [ebp].Rotoz_Angle

                fmul Radian_Coef

                fsincos

                fimul Sin_Coef
                fimul [ebp].Rotoz_Distant

                fistp [ebp].Tmp3
                mov  eax,[ebp].Tmp3


		sar eax,6          ; Это чтобы дистанция была дробной

                fimul Sin_Coef

                mov  edx,eax
                mov  esi,eax
		shl  esi,16
                mov  si,word ptr [ebp].d2X

                fimul [ebp].Rotoz_Distant
                fistp [ebp].Tmp3
                mov  eax,[ebp].Tmp3

		sar eax,6

                mov  ecx,eax
                mov  ebx,eax

                neg  eax
                mov  [ebp].d2X,eax
                add  eax,edx
		shl  eax,7
		neg  eax

                add  ebx,edx
		shl  ebx,7
		neg  ebx
		shl  ebx,16
		mov  bx,ax

		shl  ecx,16
                mov  cx,dx
xchg	ebp,ecx

                mov  ecx,VERT_RAZR
@@Image_Loop1:


push ebx

                mov  edx,HOR_RAZR
@@Image_Loop2:
                mov  eax,ebx
		shr  eax,16	
                mov  al,bh
add ax,dx
		mov eax,dword ptr Texture[eax*4]

		stosd

                add  ebx,ebp

                dec  edx
                jnz  @@Image_Loop2

pop ebx
                add  ebx,esi

                loop @@Image_Loop1
popad
;                retn

endm