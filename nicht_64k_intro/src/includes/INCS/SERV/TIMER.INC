.CODE
Get_Time proc near

;inc Time

IFDEF INTRO_SB_VER

call GETPOS_

and eax,0FFFFFF00h
mov Time__,eax
shr eax,8
mov Time,eax

call GETTIME_
mov Time2,eax
fild Time2
fstp Time2

ELSE

call XMPGetpos_
mov   Time,eax

shl  eax,8
mov   Time__,eax

call XMPGettimer_
mov   Time2,eax
fild Time2
fdiv Time_Coef
fstp Time2
ENDIF

retn
endp

.DATA
Time_Coef dd  17.0 ; 625.0

.DATA?
Time   dd  ?

Time__   dd  ?

Time2  dd  ?

