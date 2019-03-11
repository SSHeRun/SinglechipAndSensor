
        ORG     0000H
        LJMP    START
        ORG     000BH
        INC     20H     ;中断服务,中断计数器加1
        MOV     TH0,#0D8H
        MOV     TL0,#0EFH  ;12M晶振，形成10毫秒中断
        RETI
START:  
        MOV     SP,#50H
        MOV     TH0,#0D8H
        MOV     TL0,#0EFH
        MOV     TMOD,#01H
        MOV     IE,#82H
MUSIC0:
        NOP
        MOV     DPTR,#DAT      ;表头地址送DPTR
        MOV     20H,#00H        ;中断计数器清0
        MOV     B,#00H          ;表序号清0
MUSIC1:
        NOP
        CLR     A
        MOVC    A,@A+DPTR       ;查表取代码
        JZ      END0     ;是00H,则结束
        CJNE    A,#0FFH,MUSIC5
        LJMP    MUSIC3
MUSIC5:
        NOP
        MOV     R6,A
        INC     DPTR
        MOV     A,B
        MOVC    A,@A+DPTR       ;取节拍代码送R7
        MOV     R7,A
        SETB    TR0             ;启动计数
MUSIC2:
        NOP
        CPL     P1.5
        MOV     A,R6
        MOV     R3,A
        LCALL   DEL
        MOV     A,R7
        CJNE    A,20H,MUSIC2    ;中断计数器(20H)=R7否?
                                ;不等,则继续循环
        MOV     20H,#00H        ;等于,则取下一代码
        INC     DPTR
;        INC     B
        LJMP    MUSIC1
MUSIC3:
        NOP
        CLR     TR0     ;休止100毫秒
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
        MOV     R2,#10H ;歌曲结束,延时1秒后继续
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

