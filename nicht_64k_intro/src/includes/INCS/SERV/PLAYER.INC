.CODE
locals

IFDEF INTRO_GUS_VER

global xmpInit_:near
global xmpSetVolume_:near
global xmpGetSync_:near
global xmpGetPos_:near
global xmpPlay_:near
global xmpStop_:near
global xmpGetTimer_:near
global xmpGetEnvPtrDOS32_:near
global xmpGetGUSPort_:near
global xmpGetEnvPtrDPMI_:near

MXMINTMODEDPMI=0
MXMINTMODEDOS=1
MXMINTMODEEOS=2

Player_Init  proc near
  movzx eax,__psp
  call  xmpGetEnvPtrDPMI_
  call  xmpGetGUSPort_
  lea   ebx,mxmdata
  mov   ecx,65536
  mov   esi,offset swop
  mov   dl,MXMINTMODEDOS
  call  xmpInit_
  retn
Player_Init  endp

Player_Get_Time  proc near

  call  XMPGetpos_
  mov   Time,eax
  shl   eax,8
  mov   Time__,eax

  call  XMPGettimer_
  mov   Time2,eax
  fild  Time2
  fdiv  XMP_Time_Coef
  fstp  Time2

  retn
Player_Get_Time  endp

Player_Play  proc near
   xor eax,eax
IFDEF DEBUG_POS
   mov eax,DEBUG_POS
ENDIF
    call xmpPlay_
    retn
Player_Play  endp

Player_Deinit  proc near
@@flush:
    mov ah,11
    int 21h
    cmp al,0
    je  @@bye
    mov ah,7
    int 21h
    jmp @@flush
@@bye:
    call xmpStop_
    retn
Player_Deinit  endp

.DATA
   XMP_Time_Coef  dd    17.0
   include F:\DEMO_SB\PLAYER\swop.inc
ENDIF

IFDEF INTRO_SB_VER

Extrn GETPOS_:near
Extrn GETTIME_:near
Extrn PLAY_:near

Player_Init  proc near
  retn
Player_Init  endp

Player_Get_Time  proc near
  call GETPOS_
  and eax,0FFFFFF00h
  mov Time__,eax
  shr eax,8
  mov Time,eax

  call GETTIME_
  mov Time2,eax
  fild Time2
  fstp Time2
  retn
Player_Get_Time  endp

Player_Play  proc near
    call PLAY_
    or eax,eax
    jnz @@Not_Success
    clc
    retn
@@Not_Success:
    stc
    retn
Player_Play  endp

Player_Deinit  proc near
    retn
Player_Deinit  endp

ENDIF

IFDEF TTTR
Player_Init  proc near
  retn
Player_Init  endp

Player_Gettime  proc near

  retn
Player_Gettime  endp

Player_Getpos  proc near
  retn
Player_Getpos  endp

Player_Play  proc near
    retn
Player_Play  endp

Player_Deinit  proc near
    retn
Player_Deinit  endp
ENDIF


.DATA?
Time   dd  ?
Time__   dd  ?
Time2  dd  ?
