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
   
.section ".sstring" superfree
.accu 16
.index 16
.16bit

;void addbcd(long long *bcd, unsigned int n) {
addbcd:
  bankin
    ; copy bcd to r10
    lda 5,s
    sta tcc__r10
    lda 7,s
    sta tcc__r10h
    lda 9,s
    sta tcc__r9 ; n

    LDA [tcc__r10] ;[bcd]
    STA.b tcc__r0;			// low
    LDY #$0002;
    LDA [tcc__r10],y ;[bcd],Y
    STA.b tcc__r1;			// high

    LDY #$0008;
--    LDA.b tcc__r0;    
      AND #$000F;		// isolate one BCD
      CLC;
      ADC.b tcc__r9 ;n		// add n
      STZ.b tcc__r9 ;n		// set carry to 0 for next digit
      CMP #$000A;
      BCC +		; if >= 10, subtract 10
        SEC;
        SBC #$000A;
        INC.b tcc__r9 ;n		// set carry to 1 for next digit
+     LDX #$0004;
-       LSR;
        ROR.b tcc__r3;		// shift BCD into c and d
        ROR.b tcc__r2;
        LSR.b tcc__r1;		// move to next BCD
        ROR.b tcc__r0;
        DEX;
      BNE -
      DEY;
    BNE --

    LDA.b tcc__r2;			// low
    STA [tcc__r10] ;[bcd]
    LDA.b tcc__r3;			// high
    LDY #$0002;
    STA [tcc__r10],y ;[bcd],Y
  bankout
  rtl

.ends
