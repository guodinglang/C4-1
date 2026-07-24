
/**********************************************************************************************************
*
*	模块名称 : 	程序 BSP(Board Support Package)
*	文件名称 : 	
*	版    本 :  V1.0
*	说    明 :  硬件底层驱动程序集合
*	修改记录 :
*   版 本 号 :  
*   日    期 :     
*   作    者 :   M-L-A
*	说    明 :
*
**********************************************************************************************************/


//#include "at32f415_clock.h"
#include "projectCommon.h"
#include "main.h"



/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-25
;** 修改原因：
;** 说    明:	   
;***************************************************************************/
int main(void)
{
	system_clock_config();
	SysTickConfigure();
	ioConfig();	
	usart_configuration(115200);
	SPI1_Config();
	exint_line15_config();
	epramInitialize();
	bsp_Init();
//	configWDT(2500);	//1.6ms*x

	
	
	while(1)
	{
		InputIOScan();
		SwitchMessage();
		RX0_CommandDispose();	
//		WorkStateLED();
//		wdt_counter_reload();
	}
}



