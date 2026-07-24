 /*
*********************************************************************************************************
*	                                  
*	模块名称 :     
*	文件名称 : 
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef __IOSTREAM_H__
#define	__IOSTREAM_H__

#include "projectCommon.h"

//--------------------------------------------------------------------------
#define ALR_LED(n)       n>0? (GPIOD->scr = GPIO_PINS_1):(GPIOD->clr = GPIO_PINS_1)  //ALARM LED 
#define DAC_R_SET(n)     n>0? (GPIOA->scr = GPIO_PINS_1):(GPIOA->clr = GPIO_PINS_1)	 //0=60V;1=30V
#define DAC_CLR(n)       n>0? (GPIOA->scr = GPIO_PINS_3):(GPIOA->clr = GPIO_PINS_3)	 //下降沿清除寄存器数量，输出为0V;正常为H
#define DAC_CS(n)        n>0? (GPIOA->scr = GPIO_PINS_4):(GPIOA->clr = GPIO_PINS_4)	 //
#define DAC_LDAC(n)     n>0? (GPIOB->scr = GPIO_PINS_0):(GPIOB->clr = GPIO_PINS_0)  //0=允许寄存器变化到端口；1=保持端口电压；
#define BOOST_FSET(n)    n>0? (GPIOB->scr = GPIO_PINS_1):(GPIOB->clr = GPIO_PINS_1)	 //1=1.1MHZ;0=650KHZ
#define BOOST_SHDN(n)    n>0? (GPIOB->scr = GPIO_PINS_2):(GPIOB->clr = GPIO_PINS_2)	 //1=ON;0=OFF

#define BUSY(n)          n>0? (GPIOA->scr = GPIO_PINS_11):(GPIOA->clr = GPIO_PINS_11)
#define ALARM(n)         n>0? (GPIOA->scr = GPIO_PINS_12):(GPIOA->clr = GPIO_PINS_12)

#define STROBE           (GPIO_PINS_8 & GPIOA->idt)
#define DAC_ALARM        (GPIO_PINS_2 & GPIOA->idt)



//--------------------------------------------------------------------------
typedef union 
{
	unsigned short int regis;	//16 bit
	struct 
	{
		unsigned short int b0:1;
		unsigned short int b1:1;
		unsigned short int b2:1;
		unsigned short int b3:1;
		unsigned short int b4:1;
		unsigned short int b5:1;
		unsigned short int b6:1;
		unsigned short int b7:1;	
		unsigned short int b8:1;
		unsigned short int b9:1;
		unsigned short int b10:1;
		unsigned short int b11:1;
		unsigned short int b12:1;
		unsigned short int b13:1;
		unsigned short int b14:1;
		unsigned short int b15:1;		
	}bit;
}BitStructure16;

extern BitStructure16  erCode; //
//-------------------------------------------------------------------------------
void ioConfig(void);
void BatteryConfig(void);
void InputIOScan(void);
//void SwitchCallback(void);
void exint_line15_config(void);





#endif




