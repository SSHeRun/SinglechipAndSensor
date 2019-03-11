     
	 
	 
	    ORG     0000H
        JMP     MAIN
        ORG     000BH
        JMP     INTT0
        ORG     0100H        
MAIN:   MOV     SP,#60H
        MOV     TMOD,#01H
        SETB    ET0
        SETB    EA
        SETB    TR0
START0: ;clr    P1.5
        MOV     30H,#00H
NEXT:   MOV     A,30H
        MOV     DPTR,#TABLE
        MOVC    A,@A+DPTR
        MOV     R2,A
        JZ      ENDD
        ANL     A,#0FH
        MOV     R5,A
        MOV     A,R2
        SWAP    A
        ANL     A,#0FH
        JNZ     SING
        CLR     TR0
        JMP     D1
SING:   DEC     A
        MOV     22H,A
        RL      A
        MOV     DPTR,#TABLE1
        MOVC    A,@A+DPTR
        MOV     TH0,A
        MOV     21H,A
        MOV     A,22H
        RL      A
        INC     A
        MOVC    A,@A+DPTR
        MOV     TL0,A
        MOV     20H,A
        SETB    TR0
D1:     CALL    DELAY
        INC     30H
        JMP     NEXT
ENDD:   CLR     TR0
        JMP     START0

INTT0:
        PUSH    PSW
        PUSH    ACC
        MOV     TL0,20H
        MOV     TH0,21H
        CPL     P1.5
        POP     ACC
        POP     PSW
        RETI
DELAY:  MOV     R7,#02
DELAY0: MOV     R4,#187
DELAY1: MOV     R3,#248
        DJNZ    R3,$
        DJNZ    R4,DELAY1
        DJNZ    R7,DELAY0
        DJNZ    R5,DELAY
        RET
TABLE:  DB 82H,01H,81H,94H,84H,0B4H,0A4H,04H
        DB 82H,01H,81H,94H,84H,0C4H,0B4H,04H
        DB 82H,01H,81H,0F4H,0D4H,0B4H,0A4H,94H
        DB 0E2H,01H,0E1H,0D4H,0B4H,0C4H,0B4H,04H
        DB 82H,01H,81H,94H,84H,0B4H,0A4H,04H
        DB 82H,01H,81H,94H,84H,0C4H,0B4H,04H
        DB 82H,01H,81H,0F4H,0D4H,0B4H,0A4H,94H
        DB 0E2H,01H,0E1H,0D4H,0B4H,0C4H,0B4H,04H
//TABLE1: DW 64260,64400,64524,64580,64684,64777,64820,64898
 //       DW 64968,65030,65058,65110,65157,65178,65217        
	END                                                                    
