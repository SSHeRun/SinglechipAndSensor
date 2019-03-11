/*******************************************************************************
*  点阵显示（译码器位选 共阴极）            *
********************************************************************************/

#include <reg51.h> 

sbit LS138A = P2^0;  	//定义138译码器的输入A脚由P2.0控制 
sbit LS138B = P2^1;	    //定义138译码器的输入脚B由P2.1控制
sbit LS138C = P2^2; 	//定义138译码器的输入脚C由P2.2控制

void delay(unsigned int i);      //函数声名



/************主函数**********************/ 
main()
{  
   unsigned int i,j ;

  while(1)  //进入循环状态
  {	
	 for( i=0; i<8; i++)  //实现8位动态扫描循环
	 {	 		
	  switch(i)	  //使用switch 语句控制位选  也可以是用查表的方式 学员可以试着自己修改				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; P0 = 0X18; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; P0 = 0X1C; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; P0 = 0X18; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; P0 = 0X18; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; P0 = 0X18; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; P0 = 0X18; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; P0 = 0X3C; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; P0 = 0X3C;	break;	  
	     }
		  delay(150);
	 }
	 //delay(150000);


/*	 for( j=0; j<8; j++)  //实现8位动态扫描循环
	 {	 
	  switch(j)	  //使用switch 语句控制位选  也可以是用查表的方式 学员可以试着自己修改				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; P0 = 0XFF; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; P0 = 0XFF; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; P0 = 0XFF; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; P0 = 0XFF; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; P0 = 0XFF; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; P0 = 0XFF; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; P0 = 0XFF; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; P0 = 0XFF;	break;	  
	     }
		  delay(150);
	 }
	delay(150000);			*/

	 
  }   
}	

/***************************************************************************
*																		   *
*		                    延时程序						               *
****************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


