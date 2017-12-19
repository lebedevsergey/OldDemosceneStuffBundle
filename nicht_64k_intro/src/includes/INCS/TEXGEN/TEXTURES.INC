.DATA
Text1:
      db DESERT_COM,TX1,2
      db DESERT_COM,TX2,-1
      db ALPHA_COM,TX1,TX2,60,60
      db WOBBLE_COM,TX1,100,0

      db RIPPLE_COM,TX1,10

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db COLOR_COM,TX1,80,60,100

      db END_COM,TX1

Text2:
      db DESERT_COM,TX1,2
      db FPLASMA_COM,TX2
      db ALPHA_COM,TX1,TX2,60,60

;      db TLO_COM,TX1
 ;     dw 6500
  ;    db 26,26,5
      db RIPPLE_COM,TX1,40

      db BLUR_COM,TX1
      db BLUR_COM,TX1
   ;   db COLOR_COM,TX1,100,30,100
      db END_COM,TX1

Text3:

      db DESERT_COM,TX2,2

      db WOBBLE_COM,TX2,100,100

      db FPLASMA_COM,TX1
      db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX2
      dw 1500
      db 26,26,1

      db ALPHA_COM,TX1,TX2,60,60

      db BLUR_COM,TX1
      db BLUR_COM,TX1
  ;    db BLUR_COM,TX1


;     db COLOR_COM,TX1,90,30,90
      db END_COM,TX1

Text4:

      db DESERT_COM,TX2,2

      db WOBBLE_COM,TX2,100,100

      db FPLASMA_COM,TX1
      db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX2
      dw 1500
      db 26,26,1

      db INVERT_COM,TX1

      db ALPHA_COM,TX1,TX2,60,60

      db BLUR_COM,TX1
      db BLUR_COM,TX1
  ;    db BLUR_COM,TX1


;     db COLOR_COM,TX1,90,30,90
      db END_COM,TX1

Text5:

      db DESERT_COM,TX2,2

      db POLAR_COM,TX2
      db WOBBLE_COM,TX2,50,10

      db FPLASMA_COM,TX1
      db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX2
      dw 1500
      db 26,26,2

      db ALPHA_COM,TX1,TX2,60,60

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1


  ;   db COLOR_COM,TX1,50,50,120
      db END_COM,TX1

Text6:
      db DESERT_COM,TX1,2
      db DESERT_COM,TX2,-1
      db WOBBLE_COM,TX2,100,10

      db ALPHA_COM,TX1,TX2,60,60

      db TWIRL_COM,TX1

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db END_COM,TX1

Text7:
      db FPLASMA_COM,TX1
      db DESERT_COM,TX2,2

      db ALPHA_COM,TX1,TX2,100,60

      db DESERT_COM,TX2,-1
      db WOBBLE_COM,TX2,100,10

      db ALPHA_COM,TX1,TX2,60,60

      db TWIRL_COM,TX1
      db WOBBLE_COM,TX1,20,20

      db BLUR_COM,TX1
      db BLUR_COM,TX1



      db END_COM,TX1

Text8:
;      db DESERT_COM,TX1,-1

      db TLO_COM,TX1
      dw 6500
      db 6,6,40
      db WOBBLE_COM,TX1,130,0
      db RIPPLE_COM,TX1,5

      db BLUR_COM,TX1
      db BLUR_COM,TX1
 ;     db COLOR_COM,TX1,100,30,100
      db END_COM,TX1


Text9:
;      db DESERT_COM,TX1,-1

      db TLO_COM,TX1
      dw 6500
      db 40,40,7


      db WOBBLE_COM,TX1,130,40
      db RIPPLE_COM,TX1,5

      db FPLASMA_COM,TX2
      db ALPHA_COM,TX1,TX2,60,150

      db WOBBLE_COM,TX1,43,40

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
 ;     db COLOR_COM,TX1,100,30,100
      db END_COM,TX1

Text10:
;      db DESERT_COM,TX1,2
 ;     db DESERT_COM,TX2,-1
  ;    db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX1
      dw 1500
      db 6,6,255

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db WOBBLE_COM,TX1,100,0

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db END_COM,TX1

Text11:

      db TLO_COM,TX1
      dw 1500
      db 6,6,255


      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db WOBBLE_COM,TX1,100,0

      db FPLASMA_COM,TX2
      db ALPHA_COM,TX1,TX2,60,170

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db END_COM,TX1

Text12:

      db TLO_COM,TX1
      dw 1500
      db 6,6,255

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db WOBBLE_COM,TX1,50,0
      db TWIRL_COM,TX1

      db WOBBLE_COM,TX1,15,10

      db FPLASMA_COM,TX2
      db ALPHA_COM,TX1,TX2,60,170

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db END_COM,TX1


Text14:

      db TLO_COM,TX1
      dw 2500
      db 1,6,255

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db WOBBLE_COM,TX1,50,0
      db TWIRL_COM,TX1

      db WOBBLE_COM,TX1,25,10

      db FPLASMA_COM,TX2

      db ALPHA_COM,TX1,TX2,60,120

      db BLUR_COM,TX1


      db END_COM,TX1


Lightmap1:
      db PHONG1_COM,TX1

      db BLUR_COM,TX1

;      db COLOR_COM,TX1,100,100,0

      db END_COM,TX1


Text15:

;      db PHONG1_COM,TX1

      db TLO_COM,TX1
      dw 12500
      db 3,6,255

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db WOBBLE_COM,TX1,50,0
      db TWIRL_COM,TX1

      db WOBBLE_COM,TX1,25,10

      db FPLASMA_COM,TX2

      db ALPHA_COM,TX1,TX2,60,40

;      db COLOR_COM,TX1,50,50,50


      db DESERT_COM,TX1,2
;      db DESERT_COM,TX2,-1
      db FPLASMA_COM,TX2
      db ALPHA_COM,TX1,TX2,50,50
      db WOBBLE_COM,TX1,100,0

      db RIPPLE_COM,TX1,20

      db TWIRL_COM,TX1

      db BLUR_COM,TX1
      db BLUR_COM,TX1
 ;     db BLUR_COM,TX1


;      db INVERT_COM,TX1



      db END_COM,TX1

Displace_Tex:
       db CLEAR_TEX_COM,TX1
       db PHONG2_COM,TX1,10,10
       dd 0.2,0.0,0.0,1.0

       db PHONG2_COM,TX1,100,10
       dd 0.2,0.0,1.0,0.0

       db PHONG2_COM,TX1,10,100
       dd 0.2,1.0,0.0,0.0

       db PHONG2_COM,TX1,100,100
       dd 0.2,1.0,1.0,0.0

       db PHONG2_COM,TX1,60,60
       dd 0.2,1.0,0.3,1.0

       db PHONG2_COM,TX1,60,10
       dd 0.2,0.0,1.0,1.0

       db PHONG2_COM,TX1,10,60
       dd 0.2,0.5,0.4,1.0

       db PHONG2_COM,TX1,150,60
       dd 0.2,0.0,0.0,1.0

       db PHONG2_COM,TX1,150,100
       dd 0.2,0.3,0.2,0.1

       db PHONG2_COM,TX1,100,30
       dd 0.2,0.6,0.5,0.6

       db PHONG2_COM,TX1,0,0
       dd 0.4,0.2,0.2,0.9

       db PHONG2_COM,TX1,0,150
       dd 0.6,0.3,0.8,0.1

       db PHONG2_COM,TX1,150,0
       dd 0.2,0.9,0.9,0.0

       db PHONG2_COM,TX1,150,150
       dd 0.1,0.9,1.0,0.9

       db PHONG2_COM,TX1,60,150
       dd 0.2,0.4,0.0,1.0

      db BLUR_COM,TX1
      db BLUR_COM,TX1

;      db COLOR_COM,TX1,100,100,30

      db END_COM,TX1


Text7_2:
      db FPLASMA_COM,TX1
      db DESERT_COM,TX2,2

      db ALPHA_COM,TX1,TX2,100,60

      db DESERT_COM,TX2,-1
      db WOBBLE_COM,TX2,100,10

      db ALPHA_COM,TX1,TX2,60,60

      db TWIRL_COM,TX1
      db WOBBLE_COM,TX1,20,20

      db BLUR_COM,TX1
      db BLUR_COM,TX1

      db COLOR_COM,TX1,80,60,100

      db END_COM,TX1

Text2_:
      db DESERT_COM,TX2,2

      db POLAR_COM,TX2
      db WOBBLE_COM,TX2,50,10

      db FPLASMA_COM,TX1
      db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX2
      dw 1500
      db 26,26,2

      db ALPHA_COM,TX1,TX2,60,60

      db BLUR_COM,TX1
      db BLUR_COM,TX1
      db BLUR_COM,TX1


;     db COLOR_COM,TX1,50,50,120
      db END_COM,TX1


Text4_:

      db DESERT_COM,TX2,2

      db WOBBLE_COM,TX2,100,100

      db FPLASMA_COM,TX1
      db ALPHA_COM,TX1,TX2,60,60

      db TLO_COM,TX2
      dw 1500
      db 26,26,1

      db INVERT_COM,TX1

      db ALPHA_COM,TX1,TX2,60,60

      db BLUR_COM,TX1
      db BLUR_COM,TX1
  ;    db BLUR_COM,TX1


     db COLOR_COM,TX1,120,120,120
      db END_COM,TX1
