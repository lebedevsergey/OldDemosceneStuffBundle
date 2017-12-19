.CODE
Debug_L           Proc   Near
                pushad

push edi
mov ecx,TEX_SIZE
xor eax,eax
mov eax,0ffffffffh
@@ll1:
mov [edi],eax
add edi,TEX_SIZE *4
loop @@ll1
pop edi

push edi
mov ecx,TEX_SIZE
xor eax,eax
mov eax,0ffffffffh
rep stosd
pop edi

                popad
                retn
endp
