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

;void writebcd(long long bcd, unsigned int *map, unsigned int p, unsigned int offset) {
writebcd:
  bankin
    STZ tcc__r2;
    LDA 5,s ;bcd	// low
    STA tcc__r0;
    LDA 7,s ;bcd	// high
    STA tcc__r1;
    lda 9,s ; map lo
    sta tcc__r9
    lda 11,s ; map hi
    sta tcc__r9h
    lda 13,s ; p
    sta tcc__r10

    LDX #$0008;
--    CPX #$0001;
      BNE +
        INC tcc__r2
+     LDA tcc__r1
      XBA
      LSR
      LSR
      LSR
      LSR
      AND #$000F
      TAY
      CLC
      ADC tcc__r2
      BEQ +
        TYA;
        CLC;
        ADC 15,s ;offset
        LDY.b tcc__r10 ;p
        STA [tcc__r9],Y; [map],y
        INC tcc__r2;
+     LDY #$0004;
-       ASL tcc__r0;
        ROL tcc__r1;
        DEY;
      BNE -
      INC.b tcc__r10 ;p
      INC.b tcc__r10 ;p
      DEX;
    BNE --
  bankout
  rtl

;void writestring(unsigned char *st, unsigned int *map, unsigned int p, unsigned int offset) {
writestring:
  bankin
  ;unsigned int sp;
    lda 5,s		; st lo
    sta.b tcc__r10
    lda 7,s		; st hi
    sta.b tcc__r10h
    lda 9,s		; map lo
    sta.b tcc__r9
    lda 11,s		; map hi
    sta.b tcc__r9h
    LDY #$0000;
    LDA 13,s ;p			// position
    sta.b tcc__r1
    STA.b tcc__r0 ;sp			// start pos
-     LDA.b [tcc__r10],y ;[st],Y
      AND #$00FF;
      BEQ ++
      CMP #$000A;
      BNE +
        LDA.b tcc__r0 ;sp		// newline
        CLC
        ADC #$0040
        STA.b tcc__r1 ;p
        STA.b tcc__r0 ;sp
        INY
        BRA -
+     TYX
      CLC
      ADC 15,s ;offset
      LDY.b tcc__r1 ;p
      STA.b [tcc__r9],y ;[map],Y;
      INC.b tcc__r1; p
      INC.b tcc__r1; p
      TXY
      INY
      BRA -
++ bankout
  rtl

.ends
