[GLOBAL _GrInitialize]
[GLOBAL _GrShutdown]
[GLOBAL _GrRefresh]
[GLOBAL _GrGetSlotMap]
[GLOBAL _GrGetClassMap]
[GLOBAL _GrGetOEMClassMap]
[GLOBAL _GrGetMaxIndex]
[GLOBAL _GrGetMaxValue]
[GLOBAL _GrGetValue]
[GLOBAL _GrGetPackedValues]
[GLOBAL _GrSetValue]
[GLOBAL _GrGetVendorName]
[GLOBAL _GrGetProductName]
[GLOBAL _GrGetControlName]
[GLOBAL _GrGetCaps]
[GLOBAL _Gr__Link]
[GLOBAL _Gr__Unlink]
[GLOBAL _GrGetSWVer]
[GLOBAL _GrGetHWVer]
[GLOBAL _GrGetDiagCnt]
[GLOBAL _GrGetDiagReg]
[GLOBAL _DPMI_SetCodeAR]
[GLOBAL _DPMI_SetDataAR]

[EXTERN _GRIP_Thunk]

[SEGMENT .text]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Since I don't know which registers DJGPP wants preserved, I've decided to ;;
;; save all registers that get modified, except for EAX when the function    ;;
;; returns a value. Better safe than sorry, says me.                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_GrInitialize:
   mov ax, 84A1h
   call far [_GRIP_Thunk+8]
RET

_GrShutdown:
   push eax
	mov ax, 84A2h
   call far [_GRIP_Thunk+8]
	pop eax
RET

_GrRefresh:
   push ebp
   mov ebp, esp
   push edx

   mov edx, dword [ebp+8]

   mov ax, 84A0h
   call far [_GRIP_Thunk+8]

   pop edx
   pop ebp
RET

_GrGetSlotMap:
   mov ax, 84B0h
   call far [_GRIP_Thunk+8]
RET

_GrGetClassMap:
   push ebp
   mov ebp, esp
   push ebx

   mov bh, byte [ebp+8]

   mov ax, 84B1h
   call far [_GRIP_Thunk+8]

   pop ebx
   pop ebp
RET

_GrGetOEMClassMap:
   push ebp
   mov ebp, esp
   push ebx

   mov bh, byte [ebp+8]

   mov ax, 84B2h
   call far [_GRIP_Thunk+8]

   pop ebx
   pop ebp
RET

_GrGetMaxIndex:
   push ebp
   mov ebp, esp
   push ebx

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]

   mov ax, 84B3h
   call far [_GRIP_Thunk+8]

   pop ebx
   pop ebp
RET

_GrGetMaxValue
   push ebp
   mov ebp, esp

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]

   mov ax, 84B4h
   call far [_GRIP_Thunk+8]

   pop ebp
RET

_GrGetValue
   push ebp
   mov ebp, esp
   push ebx
   push ecx

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]
   mov ch, byte [ebp+16]

   mov ax, 84C0h
   call far [_GRIP_Thunk+8]

   pop ecx
   pop ebx
   pop ebp
RET

_GrGetPackedValues
   push ebp
   mov ebp, esp
   push ebx
   push ecx

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]
   mov ch, byte [ebp+16]
   mov cl, byte [ebp+20]

   mov ax, 84C1h
   call far [_GRIP_Thunk+8]

   pop ecx
   pop ebx
   pop ebp
RET

_GrSetValue
   push ebp
   mov ebp, esp
   push eax
   push ebx
   push ecx
   push edx

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]
   mov ch, byte [ebp+16]
   mov dx, word [ebp+20]

   mov ax, 84C2h
   call far [_GRIP_Thunk+8]

   pop edx
   pop ecx
   pop ebx
   pop eax
   pop ebp
RET

_GrGetVendorName
   push ebp
   mov ebp, esp
   push esi
   push edi
   push eax
   push ebx
   push ecx
   push edx

   mov bh, byte [ebp+8]
   mov edi, dword [ebp+12]

   mov ax, 84D0h
   call dword far [_GRIP_Thunk+8]

   mov dword [edi+00h], ebx
   mov dword [edi+04h], ecx
   mov dword [edi+08h], edx
   mov dword [edi+0Ch], esi

   pop edx
   pop ecx
   pop ebx
   pop eax
   pop edi
   pop esi
   pop ebp
RET

_GrGetProductName
   push ebp
   mov ebp, esp
   push esi
   push edi
   push ebx
   push ecx
   push edx

   mov bh, byte [ebp+8]
   mov edi, dword [ebp+12]

   mov ax, 84D1h
   call far [_GRIP_Thunk+8]

   mov dword [edi+00h], ebx
   mov dword [edi+04h], ecx
   mov dword [edi+08h], edx
   mov dword [edi+0Ch], esi

   pop edx
   pop ecx
   pop ebx
   pop edi
   pop esi
   pop ebp
RET

_GrGetControlName
   push ebp
   mov ebp, esp
   push esi
   push edi
   push eax
   push ebx
   push ecx
   push edx

   mov bh, byte [ebp+8]
   mov bl, byte [ebp+12]
   mov ch, byte [ebp+16]
   mov edi, dword [ebp+20]

   mov ax, 84D2h
   call far [_GRIP_Thunk+8]

   mov dword [edi+00h], ebx
   mov dword [edi+04h], ecx
   mov dword [edi+08h], edx
   mov dword [edi+0Ch], esi

   pop edx
   pop ecx
   pop ebx
   pop eax
   pop edi
   pop esi
   pop ebp
RET

_GrGetCaps
   xor eax, eax
RET

_Gr__Link
   mov ax, 8490h
   call far [_GRIP_Thunk+8]
RET

_Gr__Unlink
	push eax
   mov ax, 8491h
   call far [_GRIP_Thunk+8]
	pop eax
RET

_GrGetSWVer
   mov ax, 84F0h
   call far [_GRIP_Thunk+8]
RET

_GrGetHWVer
   mov ax, 84F1h
   call far [_GRIP_Thunk+8]
RET

_GrGetDiagCnt
   mov ax, 84F2h
   call far [_GRIP_Thunk+8]
RET

_GrGetDiagReg
   push ebp
   mov ebp, esp
   push ebx

   mov bh, byte [ebp+8]

   mov ax, 84F3h
   call far [_GRIP_Thunk+8]

   pop ebx
   pop ebp
RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; These functions call the DPMI host to set access rights for the ;;
;; descriptor whose selector is sent as a parameter.               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_DPMI_SetCodeAR:
   push ebp
   mov ebp, esp
   push ebx
   push ecx
   pushf

   mov bx, word [ebp + 8]

   mov cx, cs
   lar ecx, ecx
   shr cx, 8
   mov eax, 09h
   int 31h
   setc al

   popf
	pop ecx
   pop ebx
   pop ebp
RET

_DPMI_SetDataAR:
   push ebp
   mov ebp, esp
   push ebx
   push ecx
   pushf

   mov bx, word [ebp + 8]

   mov cx, ds
   lar ecx, ecx
   shr cx, 8
   mov eax, 09h
   int 31h
   setc al

   popf
   pop ecx
   pop ebx
   pop ebp
RET

