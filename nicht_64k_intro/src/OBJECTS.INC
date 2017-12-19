RAD1  equ 100
LIT_CYL_Y_MOV   equ   -50

.DATA
Thing1:
        dd 30
        dd 30

        db LINE_ROUTE_COM
        dw 100
        db CYL_COM
        dw 27,27,27

        db OBJ_PARAMS_COM
        dd 30.0
        dd NULL
        dd ALPHA_OBJ
        dd NULL

        db END_COM

Thing2:
        dd 27
        dd 20;3

        db LINE_ROUTE_COM
        dw 100
        db CYL_COM
        dw 27,27,27

        db OBJ_PARAMS_COM
        dd 24.0
        dd NULL
        dd ALPHA_OBJ
        dd NULL

        db END_COM

Thing3:
        dd 7
        dd 10

        db LINE_ROUTE_COM
        dw 100
        db CYL_COM
        dw 27,27,27

        db OBJ_PARAMS_COM
        dd 30.0
        dd NULL
        dd ALPHA_OBJ
        dd NULL

        db END_COM

Part_Thing2:
        dd 30
        dd 30

        db SIN_ENV_COM,ENV_X
        dd 0.5,1.0,0.8
        db SIN_ENV_COM,ENV_Y
        dd 0.3,1.0,2.0
        db SIN_ENV_COM,ENV_Z
        dd 0.5,1.0,0.8

        db LINE_ROUTE_COM
        dw 100
        db CYL_COM
        dw 27,27,27

        db OBJ_PARAMS_COM
        dd 30.0
        dd NULL
        dd ALPHA_OBJ
        dd Alpha_Sprite1

        db END_COM


Lit_Cyl3_1:
        dd 3
        dd 7

        db LINE_ROUTE_COM
        dw 200
        db CYL_COM
        dw 6,6,6
        db MOVE_OBJ_COM
        dw 70,LIT_CYL_Y_MOV,0
        db ROT_OBJ_COM
        dw 0,0,180

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 100.0


        db END_COM

Lit_Cyl3_2:
        dd 3
        dd 7

        db LINE_ROUTE_COM
        dw 200
        db CYL_COM
        dw 6,6,6
        db MOVE_OBJ_COM
        dw -70,LIT_CYL_Y_MOV,0
        db ROT_OBJ_COM
        dw 0,0,180

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 100.0


        db END_COM


Lit_Cyl3_3:
        dd 3
        dd 7

        db LINE_ROUTE_COM
        dw 200
        db CYL_COM
        dw 6,6,6
        db MOVE_OBJ_COM
        dw 0,LIT_CYL_Y_MOV,70
        db ROT_OBJ_COM
        dw 0,0,180

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 100.0


        db END_COM

Lit_Cyl3_4:
        dd 3
        dd 7

        db LINE_ROUTE_COM
        dw 200
        db CYL_COM
        dw 6,6,6
        db MOVE_OBJ_COM
        dw 0,LIT_CYL_Y_MOV,-70
        db ROT_OBJ_COM
        dw 0,0,180

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 100.0


        db END_COM

Cup3_1:
        dd 10
        dd 10

        db SIN_ENV_COM,ENV_X
        dd 0.8,1.0,-0.8
        db SIN_ENV_COM,ENV_Y
        dd 0.4,1.0,2.0
        db SIN_ENV_COM,ENV_Z
        dd 0.8,1.0,-0.8

        db LINE_ROUTE_COM
        dw 30
        db CYL_COM
        dw 14,14,14
        db MOVE_OBJ_COM
        dw 0,-60,0

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP_COM
;       dd 100.0

        db FLIP_NORM_COM

        db END_COM

Big_Cyl3_1:
        dd 10
        dd 10

        db SIN_ENV_COM,ENV_Y
        dd 0.1,1.0,2.0
        db SIN_ENV_COM,ENV_X
        dd 0.2,1.0,1.0
        db SIN_ENV_COM,ENV_Z
        dd 0.2,1.0,1.0

        db LINE_ROUTE_COM
        dw 900;800
        db CYL_COM
        dw 927,927,927
        db MOVE_OBJ_COM
        dw 0,-400,0
        db ROT_OBJ_COM
        dw 0,0,180

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd TEX_OBJ
Big_Cyl6_1_Patch:
        dd Texture4_1

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
dd 1200.0


        db END_COM

Part_Obj3_4:
        dd 30
        dd 30

        db LINE_ROUTE_COM
        dw 100
        db CYL_COM
        dw 27,27,27
        db MOVE_OBJ_COM
        dw 0,-35,0

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd ALPHA_OBJ
;        dd PHONG_OBJ
        dd Alpha_Sprite1

        db CALC_NORM_COM
        db SPHERE_MAPP_COM
;       dd 100.0

        db END_COM

Man_Obj_1:
        dd  NULL
Man_Obj_Offs1:
        dd  Man1_Obj

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd TEX_OBJ
        dd Texture;4_2

        db CALC_NORM_COM
;       db SPHERE_MAPP_COM
        db SPHERE_MAPP2_COM
        dd 200.0

        db FLIP_NORM_COM

        db END_COM

Top3_1:
        dd 3
        dd 15

        db LINE_ROUTE_COM
        dw 10
        db CYL_COM
        dw RAD1,RAD1,RAD1
        db MOVE_OBJ_COM
        dw 0,-94,0

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 170.0
        dd NULL
        dd TEX_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP_COM

        db END_COM

Lit_Cyl6_1:
        dd 3
        dd 15;7

        db LINE_ROUTE_COM
        dw 50
        db CYL_COM
        dw RAD1,RAD1,RAD1
        db MOVE_OBJ_COM
        dw 0,50,0
        db ROT_OBJ_COM
        dw 0,0,180

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd PHONG_OBJ

        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 600.0

        db END_COM


Part_Obj6_1:
        dd 2
        dd 8;6

        db LINE_ROUTE_COM
        dw 10
        db CYL_COM
        dw 200,200,200

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd ALPHA_OBJ
;        dd PHONG_OBJ
        dd Alpha_Sprite1

        db END_COM



Lit_Cyl7_2:
        dd 4
        dd 4;7

;        db SIN_ENV_COM,ENV_X
 ;       dd 0.5,1.0,2.0
  ;      db SIN_ENV_COM,ENV_Y
   ;     dd 0.5,1.0,3.0
    ;    db SIN_ENV_COM,ENV_Z
     ;   dd 0.5,1.0,3.0
        db CIRC_ROUTE_COM
        dw 180,180
        db TOR_COM
        dw 57,57,57

        db ROT_OBJ_COM
        dw 90,0,0
        db MOVE_OBJ_COM
        dw 0,270,0

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd TEX_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 600.0

        db END_COM

Lit_Cyl7_1:
        dd 4
        dd 4;7

;       db SIN_ENV_COM,ENV_X
 ;       dd 0.5,1.0,2.0
  ;      db SIN_ENV_COM,ENV_Y
   ;     dd 0.5,1.0,3.0
    ;    db SIN_ENV_COM,ENV_Z
     ;   dd 0.5,1.0,3.0
        db CIRC_ROUTE_COM
        dw 180,180
        db TOR_COM
        dw 57,57,57

        db ROT_OBJ_COM
        dw 90,0,0
        db MOVE_OBJ_COM
        dw 0,-270,0

        db FLIP_NORM_COM

        db OBJ_PARAMS_COM
        dd 200.0
        dd NULL
        dd TEX_OBJ
        dd Texture4_2

        db CALC_NORM_COM
        db SPHERE_MAPP2_COM
        dd 600.0

        db END_COM

Part_Obj7:
        dd 35
        dd 3

        db LINE_ROUTE_COM
        dw 400
        db CYL_COM
        dw 107,107,107
        db MOVE_OBJ_COM
        dw 0,-200,0

        db OBJ_PARAMS_COM
        dd 300.0
        dd NULL
        dd ALPHA_OBJ
;        dd PHONG_OBJ
        dd Alpha_Sprite1

        db END_COM

Part_Obj7_2:
        dd 40
        dd 3

        db LINE_ROUTE_COM
        dw 400
        db CYL_COM
        dw 4,4,4
        db MOVE_OBJ_COM
        dw 0,-200,0

        db OBJ_PARAMS_COM
        dd 300.0
        dd NULL
        dd ALPHA_OBJ
;        dd PHONG_OBJ
        dd Alpha_Sprite2

        db END_COM
