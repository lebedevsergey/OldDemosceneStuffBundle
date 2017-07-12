;here all messages can be processed in a better looking way than before.

;the blit can go to a limited rectangle of the surface as well.
;it looks better if it is set according to the program window so
;everytime when the window appearance changes we adjust the rectangle
;to blit to
                cmp dword ptr [esp+4+MSGmsg],WM_CREATE
                jnz _@@tt1
_@@tt1:
                cmp dword ptr [esp+4+MSGmsg],WM_KEYUP
		jz  Selfdestruction
 
AllMessagesProcessed:

