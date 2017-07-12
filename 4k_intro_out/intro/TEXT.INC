.486
.data
strr	db	'!!! COOL !!!'
time_coef dw B_T/4 +100
time_add  dw VOM_TIME
.data?
tmp4 dw ?
.code
fild  time_coef
fild TIME
fisub  time_add
	fprem
fistp tmp4
fstp st

cmp tmp4,B_T/8
ja @@Not

invoke TextOutA, myHDC, 390, 290, offset strr, sizeof strr
@@Not:
