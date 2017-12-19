.CODE

Init_Sin2       proc

                fldpi
                mov  Tmp1,180
                fild  Tmp1
                fdivp st(1),st
                fstp  Radian_Coef

                mov  Tmp1,256
                fild  Tmp1
                fldpi
                fdivp st(1),st
                fstp  Radian_Coef2

                mov  Tmp1,255
                fild  Tmp1
                fldpi
                fdivp st(1),st
                fstp  Radian_Coef4

        retn
endp
.DATA?
Radian_Coef      dd ?
Radian_Coef2     dd ?
Radian_Coef4     dd ?
