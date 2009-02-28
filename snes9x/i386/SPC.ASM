[BITS 32]

;Bit-Rate Expand Waveform
;
;Desc:
;   Decompresses a 9-byte bit-rate reduced block into 16 16-bit samples.
;   This procedure is designed to be recursively called to decompress a series of blocks.
;In:
;   ESI-> Sample Block
;   EDI   -> Output buffer
;   EDX   =3D Last sample of previous block (32-bit)
;   EBX   =3D Next to last sample (sign extended from 16-bits)
;Out:
;   ESI -> Next Block
;   EDI -> After last sample
;   EDX =3D Last sample (32-bit)
;   EBX =3D Next to last sample (16-bit)
;Destroys:
;   EAX

%ifdef __DJGPP__
%define DecodeBlockAsm _DecodeBlockAsm
%define DecodeBlockAsm2 _DecodeBlockAsm2
%endif

SECTION .text
	
global DecodeBlockAsm

DecodeBlockAsm:
	push ebx
	push esi
	push edi
	push ebp

	mov esi,dword[esp+20]
	mov edi,dword[esp+24]
	mov edx,dword[esp+28]
	mov edx,dword[edx]
	mov ebx,dword[esp+32]
	mov ebx,dword[ebx]

	call BREWave

	mov eax,dword[esp+28]
	mov dword[eax],edx
	mov eax,dword[esp+32]
	mov dword[eax],ebx
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

BREWave:
ALIGN	16
	Mov	AL,[ESI]                           ;Get header byte
	Inc	ESI
	Mov	CL,0CFh
	Sub	CL,AL
	SetC	AH
	Dec	AH
	And	CL,AH
	ShR	CL,4                               ;Isolate range

	Mov	CH,8                               ;Decompress 8 bytes (16 nybbles)
	Test	AL,0Ch                             ;Does block use ADPCM compression?
	JZ 	Short @@Method0                    ;  No

	Test	AL,08h                             ;Does block use method 1?
	JZ 	Short @@Method1                    ;  Yes

	Test	AL,04h                             ;Does block use method 2?
	JZ 	short @@ZZZ                        ;  Yes

	Jmp	@@Method3                          ;Must use method 3

ALIGN 16
@@ZZZ:
	jmp	@@Method2

ALIGN	16
	;[Smp] ----------------------------------
@@Method0:
	XOr	EAX,EAX
	XOr	EDX,EDX
	Mov	AH,byte[ESI]                        ;Get byte
	Mov	DH,AH
	And	AH,0F0h                         ;AH = High nybble << 12
	ShL	DH,4                            ;DH = Low nybble << 12

	SAR	AX,CL                           ;Reduce samples according to range
	SAR	DX,CL
	Mov	word[EDI],AX
	Mov	word[2+EDI],DX
	Add	EDI,4

	Inc	ESI

	Dec	CH
	JNZ	Short @@Method0
	MovSX	EDX,DX
	MovSX	EBX,AX
	Ret

ALIGN	16
	;[Delta]+[Smp-1](15/16) -----------------
@@Method1:
		MovSX	EBX,byte[ESI]              ;Sign extend upper nybble into EBX
		And	BL,0F0h
		ShL	EBX,8
		SAR	EBX,CL

		MovSX	EAX,DX
		Add	EBX,EAX
		SAR	EAX,4
		Sub	EBX,EAX

		Mov	word[EDI],BX

		Mov	DL,byte[ESI]
		ShL	EDX,12
		MovSX	EDX,DX
		SAR	EDX,CL

		MovSX	EAX,BX
		Add	EDX,EAX
		SAR	EAX,4
		Sub	EDX,EAX

		Mov	word[2+EDI],DX
		Add	EDI,4

		Inc	ESI

	Dec	CH
	JNZ	Short @@Method1
	MovSX	EBX,BX
	Ret

ALIGN	16
	;[Delta]+[Smp-1](61/32)-[Smp-2](30/32) --
@@Method2:
		MovSX	EAX,Byte[ESI]              ;EAX = Delta

		And	AL,0F0h
		ShL	EAX,8
		SAR	EAX,CL

		;Subtract 15/16 of second sample -----
		Sub	EAX,EBX
		SAR	EBX,4
		Add	EAX,EBX
		MovSX	EBX,DX

		;Add 61/32 of last sample ------------
		And	DL,~3
		Add	EAX,EDX
		Add	EAX,EDX
		SAR	EDX,4
		Sub	EAX,EDX
		SAR	EDX,1
		MovSX	EDX,DX
		Sub	EAX,EDX

		Mov	word[EDI],AX

		Mov	DL,byte[ESI]

		ShL	EDX,12
		MovSX	EDX,DX
		SAR	EDX,CL

		Sub	EDX,EBX
		SAR	EBX,4
		Add	EDX,EBX
		MovSX	EBX,AX

		And	AL,~3
		Add	EDX,EAX
		Add	EDX,EAX
		SAR	EAX,4
		Sub	EDX,EAX
		SAR	EAX,1
		MovSX	EAX,AX
		Sub	EDX,EAX

		Mov	word[2+EDI],DX
		Add	EDI,4

		Inc	ESI

	Dec	CH
	JNZ	@@Method2
	Ret

ALIGN	16
	;[Delta]+[Smp-1](115/64)-[Smp-2](52/64) -
@@Method3:
		MovSX	EAX,Byte[ESI]

		And	AL,0F0h
		ShL	EAX,8
		SAR	EAX,CL

		;Subtract 13/16 of second sample -----
		Sub	EAX,EBX
		SAR	EBX,3
		Add	EAX,EBX
		SAR	EBX,1
		Add	EAX,EBX
		MovSX	EBX,DX

		;Add 115/64 of last sample -----------
		And	DL, ~3
		Add	EAX,EDX
		Add	EAX,EDX
		SAR	EDX,3
		Sub	EAX,EDX
		SAR	EDX,1
		Sub	EAX,EDX
		SAR	EDX,2
		Sub	EAX,EDX

		Mov	word[EDI],AX

		Mov	DL,byte[ESI]

		ShL	EDX,12
		MovSX	EDX,DX
		SAR	EDX,CL

		Sub	EDX,EBX
		SAR	EBX,3
		Add	EDX,EBX
		SAR	EBX,1
		Add	EDX,EBX
		MovSX	EBX,AX

		And	AL, ~3
		Add	EDX,EAX
		Add	EDX,EAX
		SAR	EAX,3
		Sub	EDX,EAX
		SAR	EAX,1
		Sub	EDX,EAX
		SAR	EAX,2
		Sub	EDX,EAX

		Mov	word[2+EDI],DX
		Add	EDI,4

		Inc	ESI

	Dec	CH
	JNZ	@@Method3
	Ret

global DecodeBlockAsm2

DecodeBlockAsm2:
	push ebx
	push esi
	push edi
	push ebp

	mov esi,dword[esp+20]
	mov edi,dword[esp+24]
	mov edx,dword[esp+28]
	mov edx,dword[edx]
	mov ebx,dword[esp+32]
	mov ebx,dword[ebx]

	call BREWave2

	mov eax,dword[esp+28]
	mov dword[eax],edx
	mov eax,dword[esp+32]
	mov dword[eax],ebx
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

ALIGN	16
BREWave2:

	Push	CX

	Mov	AL,byte [esi]                         ;Get header byte
	Inc	esi
	Mov	CL,0CFh
	Sub	CL,AL                              ;Invert range
	ShR	CL,4                               ;Isolate range

	Mov	CH,8                               ;Decompress 8 bytes (16 nybbles)
	Test	AL,0Ch                             ;Does block use ADPCM compression?
	JZ 	@@Method02                    ;  No

	Test	AL,8                               ;Does block use method 1?
	JZ 	@@Method12                    ;  Yes

	Test	AL,4                               ;Does block use method 2?
	jnz 	@@Method332                          ;  Yes
	jmp	@@Method22

@@Method332:
	Jmp	@@Method32                          ;Must use method 3

ALIGN	16
	;[Smp] ----------------------------------
@@Method02:
	shr	al, 4
	mov	cl, al
	cmp	cl, 12
	jbe	@@Method022
	sub cl, 4
@@Method022:
	XOr	AX,AX
	XOr	DX,DX
	Mov	al,byte [esi]                      ;Get byte
	mov	dl, al
	sar	al, 4
	sal	dl, 4
	sar	dl, 4
	movsx eax, al
	movsx edx, dl
	sal	eax,cl                           ;Reduce samples according to range
	sal	edx,cl

	call SMP12CLIP16

	Mov	[edi],AX
	Mov	[2+edi],DX
	Add	edi,4

	Inc	esi

	Dec	CH
	JNZ	@@Method022
	Pop	CX
	MovSX	EDX,DX
	MovSX	EBX,AX
	Ret

ALIGN	16
	;[Delta]+[Smp-1](15/16) -----------------
@@Method12:
	MovSX	EBX,byte [esi]            ;Sign extend upper nybble into EBX
	And	BL,0F0h
	ShL	BX,8
	SAR	BX,CL

	MovSX	EAX,DX
	Add	EBX,EAX
	SAR	EAX,4
	Sub	EBX,EAX

	Mov	[edi],BX

	Mov	DL,byte [esi]
	ShL	DX,12
	SAR	DX,CL
	MovSX	EDX,DX

	MovSX	EAX,BX
	Add	EDX,EAX
	SAR	EAX,4
	Sub	EDX,EAX

	Mov	[2+edi],DX
	Add	edi,4

	Inc	esi

	Dec	CH
	JNZ	@@Method12
	Pop	CX
	MovSX	EBX,BX
	Ret

ALIGN	16
	;[Delta]+[Smp-1](61/32)-[Smp-2](30/32) --
@@Method22:
	MovSX	EAX,byte [esi]            ;EAX =3D Delta
	And	AL,0F0h
	ShL	AX,8
	SAR	AX,CL

	;Subtract 15/16 of second sample -----
	Sub	EAX,EBX
	SAR	EBX,4
	Add	EAX,EBX
	MovSX	EBX,DX                          ;Truncate lower 16-bits

	;Add 61/32 of last sample ------------
	And	DL,~3                        ;(Lose lower 2-bits of precision)
	Add	EAX,EDX
	Add	EAX,EDX
	SAR	EDX,5
	Sub	EAX,EDX
	ShL	EDX,1
	MovSX	EDX,DX
	Sub	EAX,EDX

	Mov	[edi],AX

	Mov	DL,byte [esi]
	ShL	DX,12
	SAR	DX,CL
	MovSX	EDX,DX

	Sub	EDX,EBX
	SAR	EBX,4
	Add	EDX,EBX
	MovSX	EBX,AX

	And	AL,~3
	Add	EDX,EAX
	Add	EDX,EAX
	SAR	EAX,5
	Sub	EDX,EAX
	ShL	EAX,1
	MovSX	EAX,AX
	Sub	EDX,EAX

	Mov	[2+edi],DX
	Add	edi,4

	Inc	esi

	Dec	CH
	JNZ	@@Method22
	Pop	CX
	Ret

ALIGN	16
	;[Delta]+[Smp-1](115/64)-[Smp-2](52/64) -
@@Method32:
	MovSX	EAX,byte [esi]
	And	AL,0F0h
	ShL	AX,8
	SAR	AX,CL

	;Subtract 13/16 of second sample -----
	Sub	EAX,EBX
	SAR	EBX,3
	Add	EAX,EBX
	SAR	EBX,1
	Add	EAX,EBX
	MovSX	EBX,DX

	;Add 115/64 of last sample -----------
;	And	DL,~3
	Add	EAX,EDX
	Add	EAX,EDX
	SAR	EDX,3
	Sub	EAX,EDX
	SAR	EDX,1
	Sub	EAX,EDX
	SAR	EDX,2
	Sub	EAX,EDX

	Mov	[edi],AX

	Mov	DL, [esi]
	ShL	DX,12
	SAR	DX,CL
	MovSX	EDX,DX

	Sub	EDX,EBX
	SAR	EBX,3
	Add	EDX,EBX
	SAR	EBX,1
	Add	EDX,EBX
	MovSX	EBX,AX

;	And	AL,~3
	Add	EDX,EAX
	Add	EDX,EAX
	SAR	EAX,3
	Sub	EDX,EAX
	SAR	EAX,1
	Sub	EDX,EAX
	SAR	EAX,2
	Sub	EDX,EAX

	Mov	[2+edi],DX
	Add	edi,4

	Inc	esi

	Dec	CH
	JNZ	@@Method32
	Pop	CX
	Ret

SMP12CLIP16:
	cmp eax, -32768
	jg CLIP16SMP1SKIP0
	je CLIP16SMP1SKIP1
	mov eax, -32768
	jmp CLIP16SMP1SKIP1
CLIP16SMP1SKIP0:
	cmp eax, 32767
	jle CLIP16SMP1SKIP1
	mov eax, 32767
CLIP16SMP1SKIP1:
	cmp edx, -32768
	jg CLIP16SMP2SKIP0
	je CLIP16SMP2SKIP1
	mov edx, -32768
	jmp CLIP16SMP2SKIP1
CLIP16SMP2SKIP0:
	cmp edx, 32767
	jle CLIP16SMP2SKIP1
	mov edx, 32767
CLIP16SMP2SKIP1:
	ret
