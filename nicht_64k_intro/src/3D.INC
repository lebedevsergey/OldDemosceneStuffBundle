.CODE
locals

include 3d.def
include 3d_trans.inc
include 3d_scene.inc

Chk_Vid          proc
; in EBX->object
                 pushad

                 mov  ecx,Sorted_Faces_Cur_Ptr
                 mov  edi,Z_Sum_Ptr

                 mov  ebp,[ebx].FIRST_FACE
                 mov  edx,[ebx].LAST_FACE
push ebx

@@B_S_Loop1_:

IFDEF BACKFACE_CLIP
                 Calc_Screen_Normal
                 ftst
                 fstsw ax
                 fstp st
                 sahf
                 jnc @@Invisible_
ENDIF
                 mov [ecx],ebp

                 mov eax,[ebp].PA
                 fld [eax].New_Z
                 mov eax,[ebp].PB
                 fadd [eax].New_Z
                 mov eax,[ebp].PC
                 fadd [eax].New_Z
                 fistp dword ptr [edi]

                 add ecx,4
                 add edi,4
                 inc Sorted_Poly_Num
@@Invisible_:
                 add  ebp, SIZE TFACE
                 cmp  ebp,edx
                 jbe  @@B_S_Loop1_
pop ebx
                 mov  Sorted_Faces_Cur_Ptr,ecx
                 mov  Z_Sum_Ptr,edi

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  edx,[ebx].LAST_VERTEX
@@B_S_Loop3:
                 mov [ebp].VERTEX_VIS_FLG,1
                 add  ebp, SIZE TVERTEX
                 cmp  ebp,edx
                 jbe  @@B_S_Loop3

                 popad
                 retn
Chk_Vid          endp
.DATA?
MIN_Z2            dd   ?

