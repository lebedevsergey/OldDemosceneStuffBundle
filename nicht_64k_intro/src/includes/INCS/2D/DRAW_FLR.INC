
Draw_Flare_Byte  macro Adder
local @@Not_Carry3,@@Not_Draw3
                mov al,[esi+Adder]
                sub al,Alpha_Sprite_Subber
		jc @@Not_Draw3
		jz @@Not_Draw3

                add al,Virt_Screen_1[edi+Adder]
		jnc @@Not_Carry3
		mov al,0FFh
@@Not_Carry3:
		mov Virt_Screen_1[edi+Adder],al
@@Not_Draw3:

endm  

VEC_MULLER  equ 130
FLARES_NUM  equ 8

.CODE
locals

Project_Flare    macro
                pushad

		fld1
		fdiv Flare_Cam_Dist
		fstp Tmp2

		fld1
		fdiv Tmp_Vec3.VZ
		fadd Flare_Cam_Dist
		fmul Tmp2


		 fld  Tmp_Vec3.VX
fmul st,st(1)
		 fadd X_SIZE_2
		 fistp Flare_SX

		 fld  Tmp_Vec3.VY
fmul st,st(1)
		 fadd Y_SIZE_2
		 fistp Flare_SY
fstp st
                  popad
endm


Draw_Flares        proc    near
; esi - camera pos
; ebx - flare light pos
; edi - virt scr

finit
		 mov Flare_Virt_Screen,edi

		 lea  esi,Flare_Camera_Pos

                 pushad

		 xchg esi,ebx
		 lea  edi,Tmp_Vec2
		 call Vecvec_sub
		 lea  esi,Tmp_Vec2
		 call Vec_norm

		 lea  ebx,Flare_Camera_Forward
		 lea  esi,Tmp_Vec2
		 call Vecvec_mul
		 fabs
		 fcomp Light_Cam_Angle_Cos
                 fstsw ax
		 sahf
		 jc    @@Flares_Not_Visible

		 lea  esi,Tmp_Vec2
		 lea  ebx,Flare_Camera_Forward
		 call Vecvec_mul
		 fabs
fmul Light_Drawed_Pixels_Coef2
		 fstp  Light_Cam_Angle_Cos2
		
		 mov ebp,FLARES_NUM-1
@@Flares_Cyc:
push ebp

		 fild Flare_Ray_Lens[ebp*4]
		 lea  esi,Tmp_Vec2
		 lea  edi,Tmp_Vec3
		 call vec_const_mul
		 fstp st

		 lea  ebx,Flare_Camera_Pos
		 lea  esi,Tmp_Vec3
		 lea  edi,Tmp_Vec3
		 call Vecvec_add
		 
		 Project_Flare

		 mov edi,Flare_Virt_Screen
		 mov esi,Flare_Sprites[ebp*4]
		 mov ebx,Flare_SX
		 mov ecx,Flare_SY

		 fld Light_Cam_Angle_Cos2
		 fdiv Flare_Size_Coefs[ebp*4]
		 fst Sprite_Scale_X
		 fstp Sprite_Scale_Y

		 call Draw_Scaled_Alpha_Sprite2

;		 call Draw_Alpha_Sprite2

pop ebp		 
		 dec  ebp 
		 jns @@Flares_Cyc

@@Flares_Not_Visible:

                 popad
                 retn
Draw_Flares      endp


.DATA
FLARE1 TMODEL <Flare1_Vertex,Flare2_Vertex,NULL,NULL,200,NULL,NULL>
Flare1_Vertex         TVERTEX<100.0,100.0,10.0,1.0,0.0,0.0,0.0,0.0,0>
Flare2_Vertex         TVERTEX<100.0,100.0,10.0,1.0,0.0,0.0,0.0,0.0,0>

Flare_Camera_Pos      VECTOR4 <0.0,0.0,0.0,1.0>
Flare_Camera_Forward  VECTOR4 <0.0,0.0,1.0,0.0>
Light_Pos             TVERTEX <0.0,0.0,120.0,1.0>

Light_Cam_Angle_Cos   dd  0.3

Flare_Cam_Dist        dd  250.0

Flare_Ray_Lens        dd  40,50,60,70,-20,-30,-40,-60
Flare_Sprites         dd  Sprite,Sprite2,Sprite2,Sprite,Sprite2,Sprite2,Sprite2,Sprite2

;Flare_Sprites         dd  Light1_Sprite,Light1_Sprite,Light1_Sprite,Light1_Sprite,Light1_Sprite,Light1_Sprite,Light1_Sprite,Light1_Sprite
Flare_Size_Coefs      dd  1.2,0.8,0.8,2.0,1.0,1.0,1.0,1.0

.DATA?
Light_Drawed_Pixels_Coef2  dd  ?

Flare_Virt_Screen     dd   ?

Flare_SX  dd   ?
Flare_SY  dd   ?
;Tmp_Vec2 VECTOR4 <?>
Tmp_Vec3 VECTOR4 <?>
;Tmp3      dd   ?

Light_Cam_Angle_Cos2_  dd  ?
Light_Cam_Angle_Cos2  dd  ?

