
set13 macro					; set mode 13h
        mov     ax,13h
	int	10h
endm
	
Textmode macro                                  ; set text mode back again
      mov     ax,03h
      int     10h
endm


Waitforescape macro                             ; waits for escape key
;      local     waitferit                       ; not used in this code
;waitforit:
      in    al,60h
      cmp   al,1
      jne   waitforit
endm

Waitforescape2 macro                             ; waits for escape key
;      local     waitferit                       ; not used in this code
;waitforit:
      in    al,60h
      cmp   al,1
      jne   waitforit2
endm

quittodos macro                                 ; quit back to dos
      mov   ax,4C00h                              ; MUST be called
      int   21h                                 ; gives back memory and
endm                                            ; control to dos



WaitRetrace macro
local @bettan,@bettan2
       mov  dx,03dah
@bettan:
       in   al,dx
       test al,8
       je   @bettan
@bettan2:
       in   al,dx
       test al,8
       jne  @bettan2                    ; Wait for vertical retrace
endm


;
;  Macro to print some text on the screem
;
Print MACRO string
local @text,@skip
    mov edx,offset @text
    mov ah,9
    int 21h
    jmp @skip
@text db string,13,10,36
@skip:
ENDM

KeyDelay macro                                  ; set text mode back again
      mov     ah,0
      int     16h
endm

clrbuf macro       
local @gg1,@gg2                         
@gg2:
      mov     ah,1
      int     16h
      jz      @gg1
      mov     ah,0
      int     16h
      jmp      @gg2
	

@gg1:
endm