.CODE
Draw_Poly_Tex proc
; esi - ptr to poly
              pushad

              mov  esi,[esi]
              call Prepare_Poly_Data

              call Chk_Need_Clip
              jnc  @@Clip_Not_Need

              call Clip_3D
              cmp  Poly_V_Num,3
              jb   @@Not_Visible
@@Clip_Not_Need:

              Calc_Poly_Data_Tex
              Calc_Scanlines_Tex
              Draw_Poly_Tex_
@@Not_Visible:
              popad
              retn
Draw_Poly_Tex endp

Draw_Poly_Phong proc
; esi - ptr to poly
              pushad

              mov  esi,[esi]
              call Prepare_Poly_Data
              Calc_Poly_Data_Phong

              call Chk_Need_Clip
              jnc  @@Clip_Not_Need

              call Clip_3D
              cmp  Poly_V_Num,3
              jb   @@Not_Visible
@@Clip_Not_Need:
              Calc_Scanlines_Phong
              Draw_Poly_Phong_
@@Not_Visible:
              popad
              retn
Draw_Poly_Phong endp

.DATA?
Poly_V_Num     dd   ?

Poly_Data      db MAX_POLY_VERTEX_NUM*SIZE POLY_VERTEX_DATA dup(?)
Tmp_Poly_Data  db MAX_POLY_VERTEX_NUM*SIZE POLY_VERTEX_DATA dup(?)

Poly_Edge_Start_X   dd   ?
Poly_Edge_Start_Y   dd   ?
Poly_Edge_DX        dd   ?

Poly_Edge_DU        dd   ?
Poly_Edge_DV        dd   ?
Poly_Edge_DFU       dd   ?
Poly_Edge_DFV       dd   ?

Poly_Edge_Ex_Y      dd   ?

Poly_Edge_Start_U   dd   ?
Poly_Edge_Start_V   dd   ?
Poly_Edge_Start_FU  dd   ?
Poly_Edge_Start_FV  dd   ?

Left_Scanline_X   dd VERT_RAZR dup (?)
Left_Scanline_U   dd VERT_RAZR dup (?)
Left_Scanline_V   dd VERT_RAZR dup (?)
Left_Scanline_FU  dd VERT_RAZR dup (?)
Left_Scanline_FV  dd VERT_RAZR dup (?)

Right_Scanline_X  dd VERT_RAZR dup (?)

Poly_Line_DU       dd ?
Poly_Line_DV       dd ?
Poly_Line_DFU      dd ?
Poly_Line_DFV      dd ?

Poly_Edge_Y_Height dd ?

.DATA
TRI_FIX_P_COEF_XY_  dd   TRI_FIX_P_COEF_XY__
TRI_FIX_P_COEF_UV_  dd   TRI_FIX_P_COEF_UV__