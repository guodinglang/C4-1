
/****************************************************************************************
*
*	模块名称 : BSP(Board Support Package)
*	文件名称 : 
*	版    本 : V0.1
*	说    明 : BSP 板级支持包(Board Support Package)
*	修改记录 :
*   版 本 号 :  
*   日    期 :     
*   作    者 :   
*	说    明 :
*
*****************************************************************************************/

#include "Initial.h"

//--------------------------------------------------------------------------
ParameterSturct  params;
LevelSturct  sLevel;//[LEVEL_NUM+1];	//


/***************************************************************************
;** 函数名称: 	SysTickConfigure
;** 功能描述:   SysTick Time Configure
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:  2022-5-26
;** 修改原因：
;** 说    明:  	
;***************************************************************************/
void SysTickConfigure(void)
{
  /* configure systick */
	systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
//  SysTick->LOAD = (uint32_t)(system_core_clock / (1000U));
//  SysTick->VAL = 0x00;
//  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
	SysTick_Config(system_core_clock / 10U);

}


/***************************************************************************
;** 函数名称: 	configWDT
;** 功能描述:   config WDT
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:  2022-5-26
;** 修改原因:
;** 说    明:  	
;***************************************************************************/
void configWDT(uint16_t d)
{
	/* reset from wdt */
	crm_flag_clear(CRM_WDT_RESET_FLAG);
	/* disable register write protection */
	wdt_register_write_enable(TRUE);

	/* set the wdt divider value */
	wdt_divider_set(WDT_CLK_DIV_32);	//0.8ms*d

	/* set reload value

	timeout = reload_value * (divider / lick_freq )    (s)

	lick_freq    = 40000 Hz
	divider      = 64		//1.6ms*d
	reload_value = d		//12bit=4095

	timeout = d * (64 / 40000 ) = max:6.552s = 6552ms
	*/
	wdt_reload_value_set(d - 1);

	/* reload wdt counter */
	wdt_counter_reload();

	/* enable wdt */
	wdt_enable();
	
}

/***************************************************************************
;** 函数名称: 
;** 功能描述:  取出EEPRAM 数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:  2013-4-17
;** 修改原因：
;** 说    明:  	AT24C256	  (65535 byte )
;***************************************************************************/
void epramInitialize(void)
{
#ifdef SimulateIIC0	
	
	uint8_t i,arry[12];
	
	IICx=IIC0;
	if(0xed != IIC_Read_Data(0))
	{							
		IIC_Write_Data(0,0xed);
//		
//		IIC_Write_Data(EMPOWER_ADD,250);//未授权只能使用200次//破解为250
//		
		setup.upperLimit=1000;
		IIC_Write_4Byte(UPPERLIMIT_ADD,setup.upperLimit);
		setup.lowerLimit=0;
		IIC_Write_4Byte(LOWERLIMIT_ADD,setup.lowerLimit);
		setup.autoOff=300;
		IIC_Write_2Byte(AUTO_OFF_ADD,setup.autoOff);		
		setup.buzz=1;
		IIC_Write_Data(BUZZER_ADD,setup.buzz);
		setup.bluet=1;
		IIC_Write_Data(BLUET_ADD,setup.bluet);
	
		pHall->coeff[0]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD1,pHall->coeff[0]);
		pHall->coeff[1]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD2,pHall->coeff[1]);
		pHall->coeff[2]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD3,pHall->coeff[2]);
		pHall->coeff[3]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD4,pHall->coeff[3]);
		pHall->coeff[4]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD5,pHall->coeff[4]);
		pHall->coeff[5]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD6,pHall->coeff[5]);
		pHall->coeff[6]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD7,pHall->coeff[6]);
		pHall->coeff[7]=0.0001;
		IIC_Write_double(HALL_COEFF_ADD8,pHall->coeff[7]);
		pBat->coeff=0.001614;
		IIC_Write_double(BAT_COEFF_ADD,pBat->coeff);
		
		pHall->segm[0]=1048575;
        IIC_Write_4Byte(HALL_SEGM_ADD1,pHall->segm[0]);
		pHall->segm[1]=2097150;
        IIC_Write_4Byte(HALL_SEGM_ADD2,pHall->segm[1]);
		pHall->segm[2]=3145725;
        IIC_Write_4Byte(HALL_SEGM_ADD3,pHall->segm[2]);
		pHall->segm[3]=4194300;
        IIC_Write_4Byte(HALL_SEGM_ADD4,pHall->segm[3]);
		pHall->segm[4]=5242875;
        IIC_Write_4Byte(HALL_SEGM_ADD5,pHall->segm[4]);
		pHall->segm[5]=6291450;
        IIC_Write_4Byte(HALL_SEGM_ADD6,pHall->segm[5]);		
		pHall->segm[6]=7340025;
        IIC_Write_4Byte(HALL_SEGM_ADD7,pHall->segm[6]);		
		pHall->segm[7]=8388600;
        IIC_Write_4Byte(HALL_SEGM_ADD8,pHall->segm[7]);		
//		IIC_Write_Data(MEM_NUM_ADD,0);	//memory number
		memset(setup.type,0,12);
		memcpy(setup.type,"TS-100",6);// machine type
		IIC_Write_Nbyte(setup.type,MTYPE_ADD,6);
		
		memset(setup.unit,0,5);
		memcpy(setup.unit,"N/cm",4);// unit
		IIC_Write_Nbyte(setup.unit,UNIT_ADD,4);
		
		for(i=0;i<12;i++)
		{
			IIC_Write_Data(SN_ADD+i,0); //SN
			IIC_Write_Data(ID_ADD+i,0); //UniqueID
		}
		exADC.Rconf=0x10;	//40HZ,PGA-1,CH-A
		IIC_Write_Data(EXADC_ADD,exADC.Rconf);		
	}
	
	setup.upperLimit = IIC_Read_4Byte(UPPERLIMIT_ADD);
	setup.lowerLimit = IIC_Read_4Byte(LOWERLIMIT_ADD);
	setup.autoOff = IIC_Read_2Byte(AUTO_OFF_ADD);	
	setup.buzz = IIC_Read_Data(BUZZER_ADD);
	setup.bluet = IIC_Read_Data(BLUET_ADD);

	pHall->coeff[0] = IIC_Read_double(HALL_COEFF_ADD1);
	pHall->coeff[1] = IIC_Read_double(HALL_COEFF_ADD2);
	pHall->coeff[2] = IIC_Read_double(HALL_COEFF_ADD3);
	pHall->coeff[3] = IIC_Read_double(HALL_COEFF_ADD4);
	pHall->coeff[4] = IIC_Read_double(HALL_COEFF_ADD5);	
	pHall->coeff[5] = IIC_Read_double(HALL_COEFF_ADD6);
	pHall->coeff[6] = IIC_Read_double(HALL_COEFF_ADD7);
	pHall->coeff[7] = IIC_Read_double(HALL_COEFF_ADD8);
	pBat->coeff = IIC_Read_double(BAT_COEFF_ADD);
	
	pHall->segm[0] = IIC_Read_4Byte(HALL_SEGM_ADD1);
	pHall->segm[1] = IIC_Read_4Byte(HALL_SEGM_ADD2);
	pHall->segm[2] = IIC_Read_4Byte(HALL_SEGM_ADD3);
	pHall->segm[3] = IIC_Read_4Byte(HALL_SEGM_ADD4);
	pHall->segm[4] = IIC_Read_4Byte(HALL_SEGM_ADD5);
	pHall->segm[5] = IIC_Read_4Byte(HALL_SEGM_ADD6);
	pHall->segm[6] = IIC_Read_4Byte(HALL_SEGM_ADD7);
	pHall->segm[7] = IIC_Read_4Byte(HALL_SEGM_ADD8);
	
//	
	exADC.Rconf = IIC_Read_Data(EXADC_ADD); 
	exADC_WriteRegister(exADC.Rconf);	//
//	
	memset(setup.type,0,12);
	IIC_Read_Nbyte(setup.type,MTYPE_ADD,12); //类型	
	if(strlen((char *)setup.type)>0)
	{
		i=machineTypeDiscern((char *)setup.type,0);  //识别出机器类型
		if(i==0)
		{erCode.bit.b1=1;}	//error 1 没有符合的机器类型
	}
	else	
	{erCode.bit.b0=1;} //error 0	没有机器类型
	
	memset(setup.unit,0,5);
	IIC_Read_Nbyte(setup.unit,UNIT_ADD,5); //类型	
	if(strlen((char *)setup.unit)<=0)
	{erCode.bit.b2=1;} //error no have unit 
	
	IIC_Read_Nbyte(arry,SN_ADD,12); //SN
	if(strlen((char *)arry)<=0)
	{erCode.bit.b3=1;} //error no have SN 
	IIC_Read_Nbyte(arry,ID_ADD,12); //UniqueID
	if(strlen((char *)arry)<=0)
	{erCode.bit.b4=1;} //error  no have UniqueID 
	
	
//	empower = IIC_Read_Data(EMPOWER_ADD);//取出授权
#else

	uint8_t i,j;

	flash_read_byte(FLASH_SECTION_PARAMETER + 0,&params.fInitial,1);
	if(0x31 != params.fInitial)
	{	
		params.fInitial=0x31;	//标记初始化
		memset(params.IDN,0,24);
		memcpy(params.IDN,"OE MEMS 1XN",11);
		memset(params.PN,0,17);
		memcpy(params.PN,"PN:123456789",12);
		memset(params.SN,0,17);
		memcpy(params.SN,"SN:123456789",12);
		memset(params.ver,0,sizeof(params.ver));
		memcpy(params.ver,"PCB:C4-1;PRG:V01.4",20);
		
		params.nCH = 8;
		params.pps = 0x0f;

		// 初始化所有分段的校准系数为0
		for(i=0;i<4;i++)
		{
			for(j=0;j<SEGMENT_NUM;j++)
			{
				params.k[i][j]=0;
				params.ol[i][j]=0;
			}
		}	//保留3位小数点
		ParameterSave();
		
//		for(i=0;i<LEVEL_NUM;i++)
//		{
//			sLevel[i].xp = 0x1000;
//			sLevel[i].xn = 0x2000;
//			sLevel[i].yp = 0x3000;
//			sLevel[i].yn = 0x4000;
//		}
//		LevelSave();
	}
	
	flash_read_byte(FLASH_SECTION_PARAMETER + 0,&params.fInitial,1);
	memset(params.IDN,0,25);
	flash_read_byte(FLASH_SECTION_PARAMETER + IDN_ADDR,params.IDN,24);
	memset(params.PN,0,17);
	flash_read_byte(FLASH_SECTION_PARAMETER + PN_ADDR,params.PN,16);
	memset(params.SN,0,17);
	flash_read_byte(FLASH_SECTION_PARAMETER + SN_ADDR,params.SN,16);
	flash_read_halfword(FLASH_SECTION_PARAMETER + NCH_ADDR,&params.nCH,1);
	flash_read_byte(FLASH_SECTION_PARAMETER + PPS_ADDR,&params.pps,1);
	// 读取版本信息
	flash_read_byte(FLASH_SECTION_PARAMETER + VER_ADDR,params.ver,24);
	// 读取分段偏移量 ol[4][60]
	flash_read_halfword(FLASH_SECTION_PARAMETER + OFL_ADDR,(uint16_t *)params.ol,4*SEGMENT_NUM);

	// 读取分段校准系数 k[4][60] - FLASH_SECTION_CALI_K (sector 127)
	flash_read_word(FLASH_SECTION_CALI_K + 0,(uint32_t *)params.k,4*SEGMENT_NUM);

//	flash_read_level(0,&sLevel.xp);		
#endif	
}

/***************************************************************************
;** 函数名称: 
;** 功能描述:  硬件的初始化
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:  2013-3-17
;** 修改原因:
;** 说    明:  
;***************************************************************************/
void bsp_Init(void)
{
	DAC_CS(1);
	BOOST_FSET(1);	//1=1.1MHZ;0=650KHZ
	delay_ms(20);
	BOOST_SHDN(1);	//1=ON;0=OFF;
	delay_ms(100);
	DAC_R_SET(0);	//0--60V		//
	DAC_LDAC(1);	//数量实时更新
	DAC_CLR(1);	
	delay_ms(100);	//
	DAC_LDAC(0);
	delay_ms(20);
	DAC_CLR(0);		//output 0V
	delay_ms(100);
	DAC_CLR(1);		//1=normal
	delay_ms(100);
	swMess.parallel = false;
	memset(&swMess,0,sizeof(swMess));
	AD5504_PowerUp(0xff);	//all channel on
//	AD5504_PowerDown(0xff);	//all off
	delay_ms(100);
	swMess.ch = 0;
	flash_read_level(swMess.ch,&sLevel.xp);
	SPI1_WriteAD5504(&sLevel.xp,4); //LevelValueDispose("lvd w 1 60 60 60 60");
	BUSY(0);	//initial ok
	
	printfx("Initial OK\r\n");
	
}


/***************************************************************************
;** 函数名称: 
;** 功能描述:  浮点型格式化小数点 带四舍五入
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:  2013-3-17
;** 修改原因：
;** 说    明:  	 leng 保几位小数点
;***************************************************************************/
double ffp(double fm, int leng)
{
	double  ft=0.0;
    unsigned long ul=0;

	if(fabs(fm)>0.0)
	{
		if(leng>=0)
		{
			if(leng>=5)	 //只允许保留5位小数点
			{leng=5;}
			leng++;
			ul=(unsigned long)(fabs(fm)*pow(10,leng));	  //10的leng次方
			if((ul%10)>=5)
			{ul=ul/10+1;}	  //四舍五入
			else
			{ul=ul/10;}
			ft=(double)(ul/pow(10,leng-1)); //保留的小数点
			if(fm<0.0)
			{ft=-ft;}
		}
		else 
		{ft=fm*pow(10,leng);}	  //负次方
	}
	return ft;
}

/***************************************************************************
** 函数名称:  	
** 功能描述:  	转换浮点数n 为字符串str。
** 输入参数: 	 
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2013-4-25
** 修改原因：
** 说    明:    字符串表示为科学计数法形式，由decimals指定四舍五入保留小数位（最多五位）。
;***************************************************************************/
char ftoa(double fv,unsigned char decimals,char *str)
{
	double fd;
	unsigned char i,n,c,k;
	unsigned int v,pw;
	
	c=0;
	if(fv<0.0)
	{str[c++]='-';}
	fv=fabs(fv);
	v=fv;
	fd=fv-v;
	v=fd*pow(10,decimals+1); //四舍五入
	if((v%10)>=5)
	{fv=fv+1/pow(10,decimals);}		
	v=fv;k=0;
	for(i=0;i<10;i++)	//0xffffffff=4 294 967 295
	{
		pw=pow(10,9-i);
		n=v/pw;
		v=v%pw;
		if((n>0)||(k>0))
		{str[c+k]=n+'0';k++;}
	}	
	c=c+k;
	if(k<=0)
	{str[c++]='0';}	
	if(decimals>0)
	{
		str[c++]='.';
		v=fv;
		fd=fv-v;
		for(i=0;i<decimals;i++)
		{
			fd=fd*10;
			v=fd;
			n=v%10;
			str[c++]=n+'0';
		}	
	}
	return c;

}
/***************************************************************************
** 函数名称:  	char itoa(int tv,char *str)
** 功能描述:  	整数 为字符串string。
** 输入参数:    tv--需要转换的整型；
** 返 回 值: 	c--返回个数； *str--返回字符串
** 全局变量: 	
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2019-9-13
** 修改原因：
** 说    明:    
			0xffffffff = 4 294 967 295;
;***************************************************************************/
char itoa(int tv,char *str)
{
	unsigned char i,n,c;
	unsigned int v;
	
	v=abs(tv);
	for(i=0;i<10;i++)	//0xffffffff=4 294 967 295
	{
		str[i]=(v%10)+'0';
		v=v/10;
		if(v<=0)
		{break;}
	}
	c=i+1;	
	if(tv<0)
	{str[c]='-';c++;}	
	for(i=0;i<(c/2);i++)
	{//高低位互换
		n=str[i];
		str[i]=str[c-i-1];
		str[c-i-1]=n;
	}	
	
	return c;
}


