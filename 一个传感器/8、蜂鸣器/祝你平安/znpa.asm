
        ORG     0000H
        LJMP    START
        ORG     000BH
        INC     20H     ;�жϷ���,�жϼ�������1
        MOV     TH0,#0D8H
        MOV     TL0,#0EFH  ;12M�����γ�10�����ж�
        RETI
START:  
        MOV     SP,#50H
        MOV     TH0,#0D8H
        MOV     TL0,#0EFH
        MOV     TMOD,#01H
        MOV     IE,#82H
MUSIC0:
        NOP
        MOV     DPTR,#DAT      ;��ͷ��ַ��DPTR
        MOV     20H,#00H        ;�жϼ�������0
        MOV     B,#00H          ;�������0
MUSIC1:
        NOP
        CLR     A
        MOVC    A,@A+DPTR       ;���ȡ����
        JZ      END0     ;��00H,�����
        CJNE    A,#0FFH,MUSIC5
        LJMP    MUSIC3
MUSIC5:
        NOP
        MOV     R6,A
        INC     DPTR
        MOV     A,B
        MOVC    A,@A+DPTR       ;ȡ���Ĵ�����R7
        MOV     R7,A
        SETB    TR0             ;��������
MUSIC2:
        NOP
        CPL     P1.5
        MOV     A,R6
        MOV     R3,A
        LCALL   DEL
        MOV     A,R7
        CJNE    A,20H,MUSIC2    ;�жϼ�����(20H)=R7��?
                                ;����,�����ѭ��
        MOV     20H,#00H        ;����,��ȡ��һ����
        INC     DPTR
;        INC     B
        LJMP    MUSIC1
MUSIC3:
        NOP
        CLR     TR0     ;��ֹ100����
        MOV     R2,#0DH
MUSIC4:
        NOP
        MOV     R3,#0FFH
        LCALL   DEL
        DJNZ    R2,MUSIC4
        INC     DPTR
        LJMP    MUSIC1
END0:
        NOP
        MOV     R2,#10H ;��������,��ʱ1������
MUSIC6:
        MOV     R3,#00H
        LCALL   DEL
        DJNZ    R2,MUSIC6
        LJMP    MUSIC0
DEL:
        NOP
DEL3:                  
        MOV     R4,#02H
DEL4:   
        NOP
        DJNZ    R4,DEL4
        NOP
        DJNZ    R3,DEL3
        RET
        NOP
DAT:
 db 26h,20h,20h,20h,20h,20h,26h,10h,20h,10h,20h,80h,26h,20h,30h,20h
 db 30h,20h,39h,10h,30h,10h,30h,80h,26h,20h,20h,20h,20h,20h,1ch,20h
 db 20h,80h,2bh,20h,26h,20h,20h,20h,2bh,10h,26h,10h,2bh,80h,26h,20h
 db 30h,20h,30h,20h,39h,10h,26h,10h,26h,60h,40h,10h,39h,10h,26h,20h
 db 30h,20h,30h,20h,39h,10h,26h,10h,26h,80h,26h,20h,2bh,10h,2bh,10h
 db 2bh,20h,30h,10h,39h,10h,26h,10h,2bh,10h,2bh,20h,2bh,40h,40h,20h
 db 20h,10h,20h,10h,2bh,10h,26h,30h,30h,80h,18h,20h,18h,20h,26h,20h
 db 20h,20h,20h,40h,26h,20h,2bh,20h,30h,20h,30h,20h,1ch,20h,20h,20h
 db 20h,80h,1ch,20h,1ch,20h,1ch,20h,30h,20h,30h,60h,39h,10h,30h,10h
 db 20h,20h,2bh,10h,26h,10h,2bh,10h,26h,10h,26h,10h,2bh,10h,2bh,80h
 db 18h,20h,18h,20h,26h,20h,20h,20h,20h,60h,26h,10h,2bh,20h,30h,20h
 db 30h,20h,1ch,20h,20h,20h,20h,80h,26h,20h,30h,10h,30h,10h,30h,20h
 db 39h,20h,26h,10h,2bh,10h,2bh,20h,2bh,40h,40h,10h,40h,10h,20h,10h
 db 20h,10h,2bh,10h,26h,30h,30h,80h,00H

END 

