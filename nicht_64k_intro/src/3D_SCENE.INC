.CODE
locals

Draw_3D_Scene    proc
; ESI - ptr to 3D scene
                 pushad

                 mov  dword ptr [Poly_Virt_Screen],edi
                 mov  dword ptr [Poly_Virt_Screen2],edi
                 mov  Sorted_Poly_Num,0
                 lea  ecx,Sorted_Faces
                 mov  Sorted_Faces_Cur_Ptr,ecx
                 mov  Sorted_Obj.FIRST_FACE,ecx
                 mov  Z_Sum_Ptr,offset Z_Sum
@@Do_Scene:                                 ; Проецируем и отсекаем сцену
                 mov  ebx,[esi]
                 cmp  ebx,NULL
                 jz   @@No_More_Obj

                 push esi
                 mov  eax,[ebx].OBJECT_SHADING_TYPE
                 call eax
                 pop  esi

                 add esi,4
                 jmp @@Do_Scene

@@No_More_Obj:
                 cmp     Sorted_Poly_Num,0
                 jz      @@ex                 ;ни одного полигона не осталось ?

                 mov  ecx,Sorted_Faces_Cur_Ptr
                 sub  ecx,4
                 mov  Sorted_Obj.LAST_FACE,ecx

IFDEF  SORT_POLYS
                 call Sort_All_Scene
ENDIF
                 lea  ebx,Sorted_Obj
                 lea  ebp,Z_Sum
                                               ; рисуем всю сцену
                 mov  edx,[ebx].LAST_FACE
                 mov  esi,[ebx].FIRST_FACE
@@Draw_Scene:
                 mov  eax,[ebp]
                 mov  Cur_Poly_Z,eax

                 mov  eax,[esi]
                 mov  eax,[eax].Poly_Draw_Proc
                 call eax                       ; Рисуем полигон

                 add  ebp,4

                 add  esi,4
                 cmp  esi,edx
                 jbe  @@Draw_Scene
@@ex:


                 popad
                 retn
Draw_3D_Scene    endp

Exec_Object_Phong  proc near
                call project_object
                call Chk_Vid
                call Calc_Phong_UV
                retn
                endp

Exec_Object_Tex  proc near
                call project_object
                call Chk_Vid
                retn
                endp

Exec_Object_Wire  proc near
                call project_object
                call Chk_Vid
                retn
                endp

Exec_Alpha_Obj  proc near

                call project_object
;               call Chk_Vid

                mov  ecx,Sorted_Faces_Cur_Ptr
                mov  edi,Z_Sum_Ptr          ; Добавляем все полигоны
                                            ;в список рисуемых
                mov  ebp,[ebx].FIRST_FACE
                mov  edx,[ebx].LAST_FACE
@@B_S_Loop1_:
                mov [ecx],ebp

                mov eax,[ebp].PA
                fld [eax].New_Z
;                fmul @@Z_Coef           ; Якобы это сумма 3-х вершин
                fmul TRI_FIX_P_COEF_XY_
                fistp dword ptr [edi]

                add  ecx,4
                add  edi,4
                inc  Sorted_Poly_Num

                add  ebp, SIZE TFACE
                cmp  ebp,edx
                jbe  @@B_S_Loop1_

                mov  Sorted_Faces_Cur_Ptr,ecx
                mov  Z_Sum_Ptr,edi

                retn
                endp

Calc_Phong_UV    proc    near
;in EBX->object
                 pushad

                 mov  ebp,[ebx].FIRST_VERTEX
                 mov  ecx,[ebx].LAST_VERTEX
@@B_S_Loop3:
                 cmp  [ebp].VERTEX_VIS_FLG,1
                 jnz  @@tt1
                                        ; Вычисление текстурных кооординат
                 Conv_X_U  [ebp].New_NZ,[ebp].New_NY,[ebp].FVV
                 Conv_X_U  [ebp].New_NZ,[ebp].New_NX,[ebp].FVU
@@tt1:
                 add  ebp, SIZE TVERTEX
                 cmp  ebp,ecx
                 jbe  @@B_S_Loop3

                 popad
                 retn
                 endp
