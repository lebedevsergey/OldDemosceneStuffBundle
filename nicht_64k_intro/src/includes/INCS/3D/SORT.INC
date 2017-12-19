.CODE
locals

.DATA?
Sorted_Poly_Num dd  ?
Sorted_Faces_Cur_Ptr  dd  ?
Z_Sum_Ptr       dd   ?

 Sorted_Obj TMODEL <?>

Sorted_Faces:  
        db MAX_POLY_SCENE_NUM*SIZE TFACE dup (?)

Z_Sum   dd MAX_POLY_SCENE_NUM dup (?)