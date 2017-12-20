
set13 macro					; set mode 13h
        mov     ax,13h
	int	10h
endm
	
Textmode macro                                  ; set text mode back again
      mov     ax,03h
      int     10h
endm
quittodos macro                                 ; quit back to dos
retn
;      mov   ah,4Ch                              ; MUST be called
 ;     int   21h                                 ; gives back memory and
endm                                            ; control to dos

WaitRetrace macro
local @bettan,@bettan2
       mov  dx,03dah
;@bettan:
 ;      in   al,dx
  ;     test al,8
   ;    je   @bettan
@bettan2:
       in   al,dx
       test al,8
       jne  @bettan2                    ; Wait for vertical retrace
endm
