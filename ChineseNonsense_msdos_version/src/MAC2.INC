Draw_Bezier_Seg  macro

                 lea  si,Control_Points
                 mov  cx,CONTROL_POINTS_NUM
@@Loop:
		 mov ax,SYM_WIDTH ;HOR_RAZR/8
		 call Random
		 mov [si].X,ax

		 mov ax,SYM_HEIGHT  ;VERT_RAZR/4
		 call Random
		 mov [si].Y,ax

;		 add si,4
lodsd
                 loop  @@Loop

                 mov  cx,Bezier_Point_Num
@@Do:           
		 mov  Tmp1,cx
 		 fild Tmp1
		 fidiv Bezier_Point_Num
                 call Draw_Bezier
                 loop  @@Do

;                 retn
;		 endp
endm
