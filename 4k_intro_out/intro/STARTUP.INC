;init DirectDraw

include myddraw.def
.code
		push offset dxdllname
		call LoadLibraryA
		push offset dxfuncname
		push eax
		call GetProcAddress
		xor  edx,edx
		push edx
		push offset OurDirectDrawInterface
		push edx
		call eax
		or eax,eax
		jnz WM_QUIT_received

		push DDSCL_FULLSCREEN or DDSCL_EXCLUSIVE
		push [Windowhandle]
		DXfunction OurDirectDrawInterface, DDSETCOOPERATIVELEVEL 

		xor esi,esi
@@Find_Mode:
pushad
		mov  eax,Vidmode_Flg
		mov  al,Vidmode_Depths_Tab[esi]
		push eax
		push VERT_RAZR
		push HOR_RAZR
		DXfunction_ OurDirectDrawInterface, DDSETDISPLAYMODE
		or  eax,eax
popad
		jz  @@Ok

IFDEF	DEBUG_VER
		cmp  edi,MAX_VIDMODE_NUM
                jae   Set_Vid_Mode_Err
ENDIF

		inc  esi
		jmp  @@Find_Mode
@@Ok:
		mov [Vidmode_Flg],esi


		mov dword ptr SurfaceDescription.dwSize, size DDSURFACEDESC
		mov dword ptr SurfaceDescription.ddssurfCaps,DDSCAPS_PRIMARYSURFACE 

		xor  eax,eax
		lea  ebp,OurPrimarySurface
		lea  edi,SurfaceDescription
		call Create_Surface2

		mov dword ptr SurfaceDescription.dwSize, size DDSURFACEDESC
		mov dword ptr SurfaceDescription.dwsurfFlags, DDSD_HEIGHT OR DDSD_WIDTH OR DDSD_CAPS OR DDSD_PIXELFORMAT
		lea edi,SurfaceDescription
		lea ebp,OurOffscreenSurface
		call Create_Surface

.code


push offset myHDC
DXfunction_ OurOffscreenSurface,DDSGETDC

.data?
hfont   dd  ?		
myHDC dd  ?

.code

invoke SetBkMode,myHDC,TRANSPARENT

;invoke SetBkColor,myHDC,0FFFFFFh    ; bbggrr
                
invoke SetTextColor,myHDC,0FFFFFFh    ; bbggrr

DXfunction_ OurOffscreenSurface, DDSRELEASEDC

		mov [DDrawIsInitialized],1
