
		.data?
		BlitDescription	DDBLTFX <>

		.data
		BlitFlags dd DDBLT_WAIT 

		.code

MainDrawingProcedure:

		call _MAIN

		cmp [DDrawIsInitialized],0
		jz NoBlit

		pushad

		DoTheBlit:


;write to offscreen surface 
;1.lock the offscreen surface so that we can write to it

		mov [SurfaceDescription.dwsurfFlags], DDSD_LPSURFACE + DDSD_PIXELFORMAT 
		mov [SurfaceDescription.ddpfPixelFormat+4], DDPF_RGB 

		xor  eax,eax
		push eax
		push DDLOCK_WAIT
		push offset SurfaceDescription
		push eax
		DXfunction_ OurOffscreenSurface,DDSLOCK
		or eax,eax
		jnz nosurflock


;now the drawing of the waveform:
;obtain a pointer to display memory
		mov edi, [SurfaceDescription.lpSurface]

include draw.inc

		AllDrawn:

		nolock:

;unlock surface
		xor  eax,eax
		push eax
		DXfunction_ OurOffscreenSurface,DDSUNLOCK
		
		nosurflock:

;set some blitting flags for the Offscreen to Primary blit
		mov [BlitDescription.dwSize], size DDBLTFX		
		mov [BlitDescription.dwDDFX], DDBLTFX_NOTEARING

		cmp TIME,VOM_TIME
		jbe	@@No2
		include text.inc
@@No2:

;do the blit
		push offset BlitDescription
		push [BlitFlags]
		xor  eax,eax
		push eax
		push [OurOffscreenSurface]
		push eax;[BlitRectPtr]
		DXfunction OurPrimarySurface, DDSBLT



		 popad

NoBlit:


		 ret


		.CODE
Create_Surface:
;  EAX : first param
;  EDI : description
;  EBP : interface ptr

		push edi
		push edi
		DXfunction OurPrimarySurface, DDSGETCAPS
		pop edi

		mov dword ptr [edi+DDSURFACEDESC.dwSize], size DDSURFACEDESC
		mov dword ptr [edi+DDSURFACEDESC.dwsurfFlags], DDSD_HEIGHT OR DDSD_WIDTH OR DDSD_CAPS
		mov dword ptr [edi+DDSURFACEDESC.ddssurfCaps], DDSCAPS_OFFSCREENPLAIN  
		mov dword ptr [edi+DDSURFACEDESC.dwHeight],VERT_RAZR
		mov dword ptr [edi+DDSURFACEDESC.dwWidth],HOR_RAZR

		xor eax,eax
Create_Surface2:

		push eax
		push ebp
		push edi
		DXfunction OurDirectDrawInterface, DDCREATESURFACE

		retn

