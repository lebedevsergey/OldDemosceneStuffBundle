.DATA

inv_coef	dw	INV_COEF_

Init_Flag  db 0
Radian_Coef     dd 0.0122718463030851298377447007159356
seed dd  0A1232134h

Sin_Coef        dw 128
Table_Scale   dw  2000

.data?


All_Data_S	struc
Cur_X1	dd ?
Cur_Y1	dd ?
Lookup_Table    dw  HOR_RAZR*VERT_RAZR dup (?)
Rotoz_Angle     dw ?
Rotoz_Distant   dw ?
Tmp3 		dd ?
Tmp4 		dw ?
d2X             dd	?
Alpha		db	?
ends

All_Data  All_Data_S <?>

