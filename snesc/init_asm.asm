.include "hdr.asm"

.macro bankin
    phb
    lda.w #0
    pha
    plb
    plb
.endm

.macro bankout
    plb
.endm

.ramsection ".registers" bank 0 slot 1
move_snesc dsb 5
.ends

.ramsection ".bss" bank $7e slot 2
snesc_do_copy dsb 1	; bit 7: do sprite copy, bit 6: do palette copy
			; bits 0-5: DMA transfer count
snesc_timer_enabled: dsb 1	; each bit one timer
snesc_timers: dsb 32		; 16 timers (two bytes each)
snesc_oam_table1: dsb 512	; OAM low table
snesc_oam_table2: dsb 32	; OAM high table
snesc_palette: dsb 512		; palette
snesc_dma_transfers: dsb 64 * 8 ; DMA transfers
				; 0 src address (24 bit)
				; 3 dest address (16 bit)
				; 5 size (16 bit)
				; 7 type (8 bit, 0 == VRAM, 1 == OAM, 2 ==
				; CGRAM, >2 == ???)
snesc_controllers dsb 8		; data from 4 controllers (16 bits each)
.ends

.section ".snesc_vblank"
; Needed to satisfy interrupt definition in "Header.inc".
.accu 8
_dodma:
    sep #$20
    PHA		; push do something flags
    BIT #$80	; copy sprites?
    BEQ __local_copy_palette
    REP #$20
    LDA #$0400
    STA $4300; DMA channel 0 Bus B addr $2104 (OAM write)
    LDA #0 ; the original code loads $8000, turning on sprite priority and
           ; messing up the gfx (some emulators don't emulate this
           ; correctly, which is probably why it has slipped through)
    STA $2102 ; OAM address 0
    LDA #snesc_oam_table1
    STA $4302 ; DMA source address
    LDA #$0220
    STA $4305 ; DMA size
    SEP #$20
    LDA #:snesc_oam_table1
    STA $4304 ; DMA source bank
    LDA #$01
    STA $420B ; enable DMA 0
__local_copy_palette:
    LDA $01,S
    BIT #$40	; copy palette?
    BEQ __local_enddma
    REP #$20
    LDA #$2200 ; DMA chan 0 Bus B addr $2122 (CGRAM write)
    STA $4300
    LDA #snesc_palette
    STA $4302  ; DMA source address
    LDA #$0200
    STA $4305  ; DMA size
    SEP #$20
    LDA #$00
    STA $2121  ; CGRAM address 0
    LDA #:snesc_palette
    STA $4304  ; DMA source bank
    LDA #$01
    STA $420B  ; enable DMA 0
__local_enddma:
    PLA
    rep #$20
    RTS

.accu 16
__snesc_vblank:
;    JMP $80805C
    REP #$30
    SEP #$20
    LDA #$00
    PHA
    PLB
    LDX #$0229
    LDA.L snesc_do_copy ; $0000,X
    BEQ __local_wait_for_joypad	; flags clear -> nothing to copy

    pha
    lda.b #0
    STA.L snesc_do_copy ;$0000,X	; clear do something flags
    pla

    ;INX	; X is now $22a
    ldx.w #0
    rep #$20
    JSR _dodma
    AND #$003F	; mask out palette and sprite flags
    BEQ __local_wait_for_joypad
    TAY
__local_label5:
    REP #$20
    LDA.l snesc_dma_transfers + 7,X
    AND #$00FF
    BNE __local_label2
    LDA #$1801	; 2 regs write once, Bus B addr $2118 (VRAM data write)
    STA $4300
    LDA.l snesc_dma_transfers + 3,X
    STA $2116	; VRAM address
__local_label2:
    DEC A
    BNE __local_label3
    LDA #$0400
    STA $4300	; DMA OAM write
    LDA.l snesc_dma_transfers + 3,X
    STA $2102	; OAM address
__local_label3:
    DEC A
    BNE __local_label4
    LDA #$2200
    STA $4300	; CGRAM write
    SEP #$20
    LDA.l snesc_dma_transfers + 3,X
    STA $2121	; CGRAM address
    REP #$20
__local_label4:
    LDA.l snesc_dma_transfers + 0,X
    STA $4302	; DMA source address
    LDA.l snesc_dma_transfers + 5,X
    STA $4305	; DMA size
    SEP #$20
    LDA.l snesc_dma_transfers + 2,X
    STA $4304	; DMA source bank
    LDA #$01
    STA $420B	; enable DMA
    REP #$20
    TXA
    CLC
    ADC #$0008
    TAX		; next memcopy structure
    SEP #$20
    DEY		; done?
    BNE __local_label5
__local_wait_for_joypad:
    LDA $4212
    LSR A	; autojoy done yet?
    BCS __local_wait_for_joypad
    REP #$30
    LDX #$0000
__local_read_controller:
    LDA $4218,X	; controller port X data
    ORA.l snesc_controllers,X	; add to data for pad X
    STA.l snesc_controllers,X
    INX
    INX
    CPX #$0008	; 4 pads
    BNE __local_read_controller
    LDX #$0000
    sep #$20
    LDA.l snesc_timer_enabled	; timer enable bits
    rep #$20
__local_do_timer:
    LSR A
    BCC __local_timer_disabled	; timer disabled
    PHP
    pha
    lda.l snesc_timers,X
    ina
    sta.l snesc_timers,X
    ;INC.l snesc_timer_enabled,X	; increment timer
    pla
    PLP
__local_timer_disabled:
    BEQ __local_done
    INX
    INX
    BNE __local_do_timer
__local_done:
    RTL

.ends
