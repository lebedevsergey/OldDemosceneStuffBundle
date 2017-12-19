
;HOR_RAZR          equ 640 ;512 ;320;320;320
;VERT_RAZR         equ 480 ;384 ;200

HOR_RAZR          equ 320;320;320
VERT_RAZR         equ 200

VGA_ADDR          equ  0A0000h
DRAW_AREA_SIZE    equ HOR_RAZR*VERT_RAZR*4/4

MIN_MODE_NUM   equ  100h
MAX_MODE_NUM   equ  200h



.CODE

Draw_Virt_Screen macro
		WaitRetrace

                mov esi,Virt_Screen1
                mov edi,Videoptr
                mov  ecx,DRAW_AREA_SIZE

call [Cur_Draw_Screen_Proc]
endm


;***********************************************************

; SET VIDEO DISPLAY START MACRO
; ECX=pixel in scan line
; EDX=scan line number
;***********************************************************
SetStart MACRO
        call   [VbeSetStart]
ENDM

;***********************************************************
; PUT PIXEL MACRO
; ECX=pixel x
; EDX=pixel y
; BL= color
;***********************************************************
PutPixel MACRO
local @1

   cmp ecx,HOR_RAZR
   ja  @1
   cmp edx,VERT_RAZR
   jae  @1

push edx
push edi
push eax


   mov eax,HOR_RAZR
   mul edx
   add eax,ecx
   mov [edi+eax*4],ebx

pop eax
pop edi
pop edx

@1:

ENDM


Put256Pixel MACRO
; IN : al - pixel color
push esi
movzx esi,al
lea esi,[esi*2+esi]
add esi,offset Palette

lodsd
stosd
pop esi
ENDM

Put256Pixel2 MACRO
; IN : al - pixel color
push esi
movzx esi,al
lea esi,[esi*2+esi]
add esi,offset Palette

lodsd
mov [edi+HOR_RAZR*4],eax
stosd
pop esi

ENDM

Put256Pixel3 MACRO
; IN : al - pixel color
push esi
movzx esi,al
lea esi,[esi*2+esi]
add esi,offset Palette

lodsd
mov [edi+256*4],eax
stosd
pop esi

ENDM


Put256Pixel_ MACRO

; IN : al - pixel color

movzx esi,ah
lea esi,[esi*2+esi]
add esi,offset Palette

lodsd
stosd

ENDM


; Data structure for VESA function 4F00h ( Get VBE Information )
;

     VbeInfoBlock struc
     VbeSignature        db   'VVVV';'VESA'    ; VBE Signature
     VbeVersion          dw   ?     ; VBE Version
     OemStringPtr        dd   ?         ; Pointer to OEM String
     Capabilities        db   4 dup (?) ; Capabilities of graphics cont.
     VideoModePtr        dd   ?         ; Pointer to Video Mode List
     TotalMemory         dw   ?         ; Number of 64kb memory blocks
                                        ; Added for VBE 2.0
     OemSoftwareRev      dw   ?         ; VBE implementation Software revision
     OemVendorNamePtr    dd   ?         ; Pointer to Vendor Name String
     OemProductNamePtr   dd   ?         ; Pointer to Product Name String
     OemProductRevPtr    dd   ?         ; Pointer to Product Revision String
     _Reserved_          db   222 dup (?); Reserved for VBE implementation
                                        ; scratch area
     OemData             db   256 dup (?); Data Area for OEM Strings
     VbeInfoBlock ends



; Data structure for VESA function 4F01h ( Get mode Information )
;
     ModeInfoBlock   struc

     ; Mandatory information for all VBE revisions
     ModeAttributes      dw ?      ; mode attributes
     WinAAttributes      db ?      ; window A attributes
     WinBAttributes      db ?      ; window B attributes
     WinGranularity      dw ?      ; window granularity
     WinSize             dw ?      ; window size
     WinASegment         dw ?      ; window A start segment
     WinBSegment         dw ?      ; window B start segment
     WinFuncPtr          dd ?      ; pointer to window function
     BytesPerScanLine    dw ?      ; bytes per scan line

     ; Mandatory information for VBE 1.2 and above
     XResolution         dw ?      ; horizontal resolution in pixels or chars
     YResolution         dw ?      ; vertical resolution in pixels or chars
     XCharSize           db ?      ; character cell width in pixels
     YCharSize           db ?      ; character cell height in pixels
     NumberOfPlanes      db ?      ; number of memory planes
     BitsPerPixel        db ?      ; bits per pixel
     NumberOfBanks       db ?      ; number of banks
     MemoryModel         db ?      ; memory model type
     BankSize            db ?      ; bank size in KB
     NumberOfImagePages  db ?      ; number of images
     _Reserved           db ?      ; reserved for page function

     ; Direct Color fields (required for direct/6 and YUV/7 memory models)
     RedMaskSize         db ?      ; size of direct color red mask in bits
     RedFieldPosition    db ?      ; bit position of lsb of red mask
     GreenMaskSize       db ?      ; size of direct color green mask in bits
     GreenFieldPosition  db ?      ; bit position of lsb of green mask
     BlueMaskSize        db ?      ; size of direct color blue mask in bits
     BlueFieldPosition   db ?      ; bit position of lsb of blue mask
     RsvdMaskSize        db ?      ; size of direct color reserved mask in bits
     RsvdFieldPosition   db ?      ; bit position of lsb of reserved mask
     DirectColorModeInfo db ?      ; direct color mode attributes

     ; Mandatory information for VBE 2.0 and above
     PhysBasePtr         dd ?      ; physical address for flat frame buffer
     OffScreenMemOffset  dd ?      ; pointer to start of off screen memory
     OffScreenMemSize    dw ?      ; amount of off screen memory in 1k units
     __Reserved          db 206 dup (?)  ; remainder of ModeInfoBlock
     ModeInfoBlock ends

