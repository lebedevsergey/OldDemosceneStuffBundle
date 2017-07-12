
.code

push offset myHDC
DXfunction OurOffscreenSurface, DDSGETDC


IFDEF  MYFONT

.data?
hfont   dd  ?		
myHDC dd  ?

.code

xor eax,eax
                invoke CreateFontA,FONT_HEIGHT,FONT_WIDTH,eax,eax,400,eax,eax,eax,DEFAULT_CHARSET,\
                            OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,\
                            DEFAULT_QUALITY,DEFAULT_PITCH or FF_DONTCARE,\
                            eax        ;ADDR FontName
                mov    hfont,eax

invoke SelectObject,myHDC, eax  ;hfont



invoke SetBkMode,myHDC,TRANSPARENT

invoke SetBkColor,myHDC,0FFFFFFh    ; bbggrr
                
invoke SetTextColor,myHDC,0FFFFFFh    ; bbggrr

ENDIF

DXfunction OurOffscreenSurface, DDSRELEASEDC
