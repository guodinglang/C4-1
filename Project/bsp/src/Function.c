
#include "Function.h"

//--------------------------------------------------------------------------
SwitchStruct  swMess;



/***************************************************************************
;** 函数名称:  	WorkStateLED
;** 功能描述:  	
;** 输入参数: 	
;** 返 回 值: 	
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：
;***************************************************************************/
void WorkStateLED(void)
{
	static uint16_t count;
	
	if(count > 65500)
	{SLED;count=0;}
	else
	{count++;}
}

/***************************************************************************
;** 函数名称:  EmpowerCheck
;** 功能描述: 　授权审核
;** 输入参数: 	empower
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2019-12-12
;** 修改原因：
;** 说    明:  

;***************************************************************************/
void EmpowerCheck(void)
{

}
/***************************************************************************
;** 函数名称:  void SwitchMessage(void) 
;** 功能描述:  switch message 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:  M-L-A 
;** 日　  期:  2022-2-14
;** 修改原因：
;** 说    明: 
;***************************************************************************/
void SwitchMessage(void)
{
	if(swMess.parallel==true)
	{
		BitStructure16 t,o;	
		
		t.regis = swMess.buff;
		o.regis = 0;
		o.bit.b0 = t.bit.b5;
		o.bit.b1 = t.bit.b8;
		o.bit.b2 = t.bit.b6;
		o.bit.b3 = t.bit.b4;
		o.bit.b4 = t.bit.b3;
		o.bit.b5 = t.bit.b7;
		
		o.regis &= params.pps;
		if(o.regis <= params.nCH)
		{
			uint16_t i,rd[4]={0,0,0,0};
			
			swMess.ch = o.regis + 1;	//0 -> CH1
			for(i=0;i<4;i++)
			{
				flash_read_level(swMess.ch,rd);
				if(rd[0]||rd[1]||rd[2]||rd[3])
				{
					SPI1_WriteAD5504(rd,4);
				}
			}
			sprintfx("\r\nCHAN:%03d\r\n",swMess.ch);
		}
		else
		{printfx("\r\nCHAN:err\r\n");}				
		BUSY(0);
		swMess.parallel=false;
	}
}






