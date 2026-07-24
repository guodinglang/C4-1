
/****************************************************************************************
*
*	模块名称 : BSP(Board Support Package)
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : BSP 板级支持包(Board Support Package)
*	修改记录 :
*   版 本 号 :  
*   日    期 :     
*   作    者 :   
*	说    明 :
*
*****************************************************************************************/

//#include <pulicinclude.h>

#include "IOstream.h"

//--------------------------------------------------------------------------
BitStructure16    erCode; //记录错误代码
//ButtonManage  	  button,*pkey=&button;
uint16_t keycn[1]={0};

/***************************************************************************
;** 函数名称:  keyConfig
;** 功能描述:  
;** 输入参数:  无
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2022-2-13
;** 修改原因：
;** 说    明:
;***************************************************************************/
void ioConfig(void)
{
	gpio_init_type gpio_init_struct;
	
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
	
	/* configure PA Input*/
	gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_6 | GPIO_PINS_8;//
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOA, &gpio_init_struct);
	
	/* configure PA Output*/
	gpio_init_struct.gpio_pins = GPIO_PINS_1|GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5|GPIO_PINS_7|GPIO_PINS_11|GPIO_PINS_12;//
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOA, &gpio_init_struct);
	
	/* configure PB Input*/
	gpio_init_struct.gpio_pins = GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5|GPIO_PINS_6|GPIO_PINS_7|GPIO_PINS_8;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOB, &gpio_init_struct);	
	gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
	
	/* configure PB Output*/
	gpio_init_struct.gpio_pins = GPIO_PINS_0|GPIO_PINS_1|GPIO_PINS_2;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOB, &gpio_init_struct);
	
	/* configure PD Output*/
	gpio_init_struct.gpio_pins = GPIO_PINS_1;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_pin_remap_config(PD01_MUX, TRUE);

	BOOST_SHDN(0);
	DAC_CS(1);		
	BUSY(ON);		//output high
	ALR_LED(OFF);
}

/***************************************************************************
;** 函数名称:  keyConfig
;** 功能描述:  
;** 输入参数:  无
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2022-2-13
;** 修改原因：
;** 说    明:
;***************************************************************************/
void exint_line15_config(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_exint_line_config(GPIO_PORT_SOURCE_GPIOA, GPIO_PINS_SOURCE8);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_8;
  exint_init_struct.line_polarity = EXINT_TRIGGER_FALLING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT9_5_IRQn, 1, 0);
	
}


/***************************************************************************
;** 函数名称:  	
;** 功能描述:  	
;** 输入参数: 	无
;** 返 回 值: 	
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-2-13
;** 修改原因：
;***************************************************************************/
void InputIOScan(void)
{	
	if(!DAC_ALARM)
	{
		if(keycn[0] < 10000)
		{
			keycn[0]++;
			if(keycn[0]==9998)
			{ALARM(1);}
		}	
	}
	else
	{
		if(keycn[0] > 0)
		{
			keycn[0]--;
			if(keycn[0]==1)
			{ALARM(0);}
		}
	}

}

/***************************************************************************
;** 函数名称:  void KeyValueDispose(void) 
;** 功能描述:  按键处理 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:  M-L-A 
;** 日　  期:  2022-2-14
;** 修改原因：
;** 说    明: 
;***************************************************************************/
//void SwitchCallback(void)
//{

//}


