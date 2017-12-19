.CODE

No_Low_Mem:
         Print 'Cannot get low memory'
         jmp Vesa_Err
ModeNotGood:
         Print 'Cannot map LFB buf'
         jmp Vesa_Err
;@@ModeNotFound:
 ;        Print 'Mode not found'
  ;       jmp Abort2
NoVESA:
         Print 'Not VESA at all'
         jmp Vesa_Err
Vesa_Err:

         popad
         stc
         retn

SetVbeMode PROC
     ;
     ; Set VIDEO mode
     ;
         pushad

         cmp Mode_LFB_Flg,1
         jnz @@tt1
         or    eax,4000h
         jmp  @@tt2
@@tt1:
         pushad
         mov ax,4f06h
         xor bx,bx
         mov cx,HOR_RAZR
         int 10h
         popad
@@tt2:

         mov ebx,eax
;         mov    BX,VideoModeNumber

         xor    eax,eax
         xor    ecx,ecx
         xor    edx,edx
         mov    ax,4F02h
         Int    10h
         popad

         Ret
SetVbeMode ENDP

Check_VESA  proc
         pushad

         mov    bx,SIZE VBEINFOBLOCK
         mov    ax,100h
         int    31h

         jc     No_Low_Mem

         mov    Real_Mode_Regs.R_ES,ax
         mov    VESA_Buf_Sel,dx

         mov    ax,4F00h
         mov    di,0            ; (real mode) ES:DI -> 256 byte buffer
         push   10h
         call   DOSinterrupt     ;  GET VESA INFORMATION

         jc     NoVESA
         Cmp    AX,004Fh
         jne    NoVESA

         push   ds
         mov    ax,VESA_Buf_Sel
         mov    ds,ax
         mov    ecx,SIZE VBEINFOBLOCK
         mov    esi,0
         mov    edi,offset My_VESA_Info
         rep    movsb
         pop    ds

;         cmp    My_VESA_Info.VbeVersion,0200h
 ;        jb     NoVESA2

         mov    ax,101h
         mov    dx,VESA_Buf_Sel
         int    31h

         popad
         clc

         mov    ax,My_VESA_Info.VbeVersion

         retn
Check_VESA endp

CheckVbeMode PROC

         pushad

         mov    My_X_Res,cx
         mov    My_Y_Res,dx
         mov    My_bpp,al

         mov    bx,SIZE MODEINFOBLOCK
         mov    ax,100h
         int    31h
         jc     No_Low_Mem
         mov    Real_Mode_Regs.R_ES,ax
         mov    Mode_Buf_Sel,dx

         mov    ax,MIN_MODE_NUM
@@Mode_Loop:
         mov    VideoModeNumber,ax

        mov    ax,4F01h
        mov    cx,VideoModeNumber
        and    cx,0111111111b               ; read bits 0..8
        mov    di,0
        push   10h
        call   DOSinterrupt
        cmp    AX,004Fh
        jne    @@Next_Mode

         push   ds
         mov    ax,Mode_Buf_Sel
         mov    ds,ax
         mov    ecx,SIZE MODEINFOBLOCK
         mov    esi,0
         mov    edi,offset My_Mode_Info
         rep    movsb
         pop    ds


         mov    ax,My_X_Res
         cmp    My_Mode_Info.XResolution,ax
         jne    @@Next_Mode
         mov    ax,My_Y_Res
         cmp    My_Mode_Info.YResolution,ax
         jne    @@Next_Mode
         mov    al,My_bpp
         cmp    My_Mode_Info.BitsPerPixel,al
         jne    @@Next_Mode

         mov    ax,My_Mode_Info.ModeAttributes
         test   al,1                   ; Режим поддерживается ?
         jz     @@Next_Mode

         cmp    Mode_LFB_Flg,1
         jnz    @@Is_Mode

         test   al,80h                   ; LFB есть ?
         jz     @@Next_Mode
         cmp    My_Mode_Info.PhysBasePtr,0
         jne    @@Is_Mode


@@Next_Mode:
         mov    ax,VideoModeNumber
         inc    ax
         cmp    ax,MAX_MODE_NUM
         jna    @@Mode_Loop

         stc
         jmp    @@No_Mode

@@Is_Mode:

         cmp    Mode_LFB_Flg,1
         jz     @@tt4

         movzx  ebx,My_Mode_Info.WinGranularity

         shl    ebx,10-2

         cmp    My_bpp,32
         je     @@tt5
         shl    ebx,1
@@tt5:
         mov    My_Bank_Size,ebx
         mov    eax,DRAW_AREA_SIZE
         cdq
         idiv   ebx
         mov    Banks_Num,eax
         mov    Last_Bank_Num,edx

         mov    ebx,VGA_ADDR
         jmp     @@tt6
@@tt4:
         mov    ebx,My_Mode_Info.PhysBasePtr
         mov    cx,bx
         shr    ebx,16
         mov    si,My_VESA_Info.TotalMemory
         xor    edi,edi
         mov    ax,0800h
         int    31h
         jc     ModeNotGood
         shl    ebx,16
         mov    bx,cx
@@tt6:
         mov    My_VideoPtr,ebx

         mov    ax,101h
         mov    dx,Mode_Buf_Sel
         int    31h

         clc

@@No_Mode:
         popad
         mov    eax,My_VideoPtr
         mov    bx,VideoModeNumber
         mov    dl,My_Mode_Info.NumberOfBanks
         mov    dh,My_Mode_Info.BankSize

         ret
CheckVbeMode endp


.DATA?

ProgramBaseAddress      DD ?
_BankSetTable           DD 128 DUP (?)
VideoModeNumber         DW ?
Current_Bank            DB ?
VBE_StarPerPixelFactor  DB ?

VESA_Buf_Seg            DW ?
VESA_Buf_Sel            DW ?

Mode_Buf_Seg            DW ?
Mode_Buf_Sel            DW ?

My_VESA_Info            VBEINFOBLOCK <?>
My_Mode_Info            MODEINFOBLOCK <?>

My_VideoPtr                dd  ?

My_X_Res                dw    ?
My_Y_Res                dw    ?
My_bpp                  db    ?

Banks_Num                      dd    ?
Last_Bank_Num                  dd    ?
My_Bank_Size                   dd    ?

Mode_LFB_Flg                   db    ?


.CODE

DOSinterrupt PROC
        mov     Real_Mode_Regs.R_EAX,eax
        mov     Real_Mode_Regs.R_EBX,ebx
        mov     Real_Mode_Regs.R_ECX,ecx
        mov     Real_Mode_Regs.R_EDX,edx
        mov     Real_Mode_Regs.R_EBP,ebp
        mov     Real_Mode_Regs.R_ESI,esi
        mov     Real_Mode_Regs.R_EDI,edi

        lea     edi,Real_Mode_Regs
        mov     ax,0300h
        xor     cx,cx
        movzx   ebx,byte ptr [esp+4]
        int     31h                     ; Emulate Real Mode Interrupt

        mov     eax,Real_Mode_Regs.R_EAX
        mov     ebx,Real_Mode_Regs.R_EBX
        mov     ecx,Real_Mode_Regs.R_ECX
        mov     edx,Real_Mode_Regs.R_EDX
        mov     ebp,Real_Mode_Regs.R_EBP
        mov     esi,Real_Mode_Regs.R_ESI
        mov     edi,Real_Mode_Regs.R_EDI
        ret 4
DOSinterrupt ENDP

.DATA?

REAL_MODE_REGS_TYPE struc
  R_EDI      dd  ?
  R_ESI      dd  ?
  R_EBP      dd  ?
  R_None     dd  ?
  R_EBX      dd  ?
  R_EDX      dd  ?
  R_ECX      dd  ?
  R_EAX      dd  ?
  R_Flags    dw  ?
  R_ES       dw  ?
  R_DS       dw  ?
  R_FS       dw  ?
  R_GS       dw  ?
  R_IP       dw  ?
  R_CS       dw  ?
  R_SP       dw  ?
  R_SS       dw  ?
ENDS

Real_Mode_Regs  REAL_MODE_REGS_TYPE <?,?,?,?,?,?,?,?,?,?,?,?,?>


