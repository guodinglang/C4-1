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

#include "Command.h"
#include "CommandList.h"

float volt_table[60] = {
    1.010742188f,
    2.006835938f,
    3.002929688f,
    4.013671875f,
    5.009765625f,
    6.005859375f,
    7.001953125f,
    8.012695313f,
    9.008789063f,
    10.00488281f,
    11.00097656f,
    12.01171875f,
    13.0078125f,
    14.00390625f,
    15.0f,
    16.01074219f,
    17.00683594f,
    18.00292969f,
    19.01367188f,
    20.00976563f,
    21.00585938f,
    22.00195313f,
    23.01269531f,
    24.00878906f,
    25.00488281f,
    26.00097656f,
    27.01171875f,
    28.0078125f,
    29.00390625f,
    30.0f,
    31.01074219f,
    32.00683594f,
    33.00292969f,
    34.01367188f,
    35.00976563f,
    36.00585938f,
    37.00195313f,
    38.01269531f,
    39.00878906f,
    40.00488281f,
    41.00097656f,
    42.01171875f,
    43.0078125f,
    44.00390625f,
    45.0f,
    46.01074219f,
    47.00683594f,
    48.00292969f,
    49.01367188f,
    50.00976563f,
    51.00585938f,
    52.00195313f,
    53.01269531f,
    54.00878906f,
    55.00488281f,
    56.00097656f,
    57.01171875f,
    58.0078125f,
    59.00390625f,
    60.0f
};

/***************************************************************************
** 函数名称:  	
** 功能描述:   大写转小写	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2014-4-29
** 修改原因：
** 说    明:    
;***************************************************************************/
int gLowerCaseString(char *str)
{
    unsigned char i,len;
    
    len=strlen(str);
    for(i=0;i<len;i++)
    {
	    if((str[i]>='A')&&(str[i]<='Z'))
	    {
		    str[i]=str[i]+0x20; 
		}
	 }
	 return 0;
}

/***************************************************************************
** 函数名称:  	
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2014-4-29
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
void RX0_CommandDispose(void)
{
	uint8_t n,c=0xff;
	char Tstr[10]={0,0,0,0,0,0,0,0,0,0};	
	
	
	if(pUART->len >= RX_MAX_FIFO)
	{	//超出长度
		memset(pUART->rxFIFO,0,RX_MAX_FIFO);
		pUART->len = 0;
		pUART->rxn = 0;
		pUART->reTx = 0;
	}
//--------返回每一个字符处理----------------------------------------------------    
	if(pUART->feelback==true)
	{
		while(pUART->len > pUART->reTx) 
		{printx(pUART->rxFIFO[pUART->reTx++]);}	
		if(pUART->rxn)
		{printfx("\r\n");}
	}
//--------指令判断--------------------------------------------------------------  
	if(pUART->rxn > 0)
	{
		if(pUART->len > 0)
		{
			BUSY(1);	//busying
			
//			sscanf((const char *)pUART->rxFIFO,"%s",Tstr);
			for(n=0;n<10;n++)
			{
				c = pUART->rxFIFO[n];
				if((c == ' ')||(c == '_')||(c == '\0'))	
				{break;}
				else
				{Tstr[n] = c;}
			}
			gLowerCaseString(Tstr);
			
			for(n=0;n<cmdStrLen;n++)  ///匹配命令编号
			{						
				if(strcmp(Tstr,cmdMessage[n].cmdStr)==0)
				{
					c = n;
					break;
				}
				else if(n==6)
				{//*sw001
					if(strncmp(Tstr,cmdMessage[n].cmdStr,3)==0)
					{
						c = n;
						break;
					}
				}
				else if(n==7)
				{//solt
					if(strncmp(Tstr,cmdMessage[n].cmdStr,4)==0)
					{
						c = n;
						break;
					}
				}
			}
			if(n >= cmdStrLen)
			{				
			   memset(pUART->rxFIFO,0,RX_MAX_FIFO);
			   printfx("command illegal\r\n"); //输入的命令非法 
			}
			else if(c!=0xff)
			{	//执行函数
				cmdMessage[c].func(pUART->rxFIFO);
				memset(pUART->rxFIFO,0,RX_MAX_FIFO);				
			} 
			pUART->len = 0;		
			BUSY(0);	//exit busy 
		}
		else
		{printfx("\r\n@_@\r\n");}
		
		pUART->reTx=0;
		pUART->rxn=0;
	}	
}
/***************************************************************************
** 函数名称:  	help
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2019-9-16
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char help(void *p,...)
{
	uint8_t i;
	for(i=0; i<cmdStrLen; i++)
	{
		printfx((char *)cmdMessage[i].rChar);	
	}	

	return 0;
}

/***************************************************************************
** 函数名称:  	help
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2019-6-8
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char RebackCharacter(void *p,...)
{
	int s;
	
	sscanf(p, "%*s %d",&s);	
	if(s==0)
	{
		pUART->feelback = false;
		pUART->reTx=0;
		printfx(p);
		printfx(" pass\r\n");
	}
	else if(s==1)
	{
		pUART->feelback = true;
		pUART->reTx=pUART->len;
		printfx(p);
		printfx(" pass\r\n");
	}
	else
	{
		printfx(p);
		printfx(" fail\r\n");
	}
	
	return 0;
}
/***************************************************************************
** 函数名称:  	ReadVersion
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2019-9-16
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char ReadVersion(void *p,...)
{
//	printfx("\r\nPCB:B3-1,PGR:V29\r\n");	
	char i,c,n,j,str[25]={0};
	
	memset(str,0,sizeof(str));
	for(i=0,j=0,n=0;c!='\0';i++)
	{
		c =((char *)p)[i];
		if(c=='_' || c==' ')
		{n++;}
		else if(n > 0)
		{str[j++]=c;}
		if(n > 1)
		{break;}
	}
	gLowerCaseString(str);
	
	if(strcmp(str,"?")==0)
	{
		printfx("\r\n");
		printfx((char *)params.ver);
		printfx("\r\n"); 		
	}
	else if(strcmp(str,"w")==0)
	{
		memset(str,0,sizeof(str));
		for(i=0,j=0,n=0;c!='\0';i++)
		{
			c =((char *)p)[i];
			if((c=='_' || c==' ')&&(n < 2))
			{n++;}
			else if(n > 1)
			{str[j++]=c;}
		}
		n=strlen(str);
		if(n>0 && n<24)
		{
			memcpy(params.ver,str,sizeof(params.ver));
			ParameterSave();
			printfx(p);printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail");}
	}
	else
	{printfx(p);printfx(" fail");}
	
	return 0;
	
}
/***************************************************************************
** 函数名称:  	SetPassWord
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2020-1-29
** 修改原因：
** 说    明:    
;***************************************************************************/
char ReadWriteBoardIDN(void *p,...)
{
	int k=0;
	char str[48]={0};//,str2[25]={0};
	
	memset(str,0,sizeof(str));
	sscanf(p,"%*s %s",str);
	
	gLowerCaseString(str);
	if((strcmp(str,"?")==0)||(strlen(str)<=0))
	{
		printfx("\r\n");
		printfx((char *)params.IDN);
		printfx("\r\n"); 		
	}
	else if(strcmp(str,"w")==0)
	{
		memset(str,0,sizeof(str));
		sscanf(p,"%*s %*s %[^'\']",str);
		k = strlen(str);	
		if((k > 0)&&(k <= 24))
		{
			memset(params.IDN,0,25);
			memcpy(params.IDN,str,k);
			ParameterSave();
			printfx(p);
			printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");}  
	}
	else
	{printfx(p);printfx(" fail\r\n");}
	
	return 0;
}

/***************************************************************************
** 函数名称:  	SetPassWord
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2020-1-29
** 修改原因：
** 说    明:    
;***************************************************************************/
char ReadWriteBoardPN(void *p,...)
{
	int k=0;
	char str[48]={0};//,str2[17]={0};
	
	memset(str,0,sizeof(str));
	sscanf(p,"%*s %s",str);
	gLowerCaseString(str);
	if((strcmp(str,"?")==0)||(strlen(str)<=0))
	{
		printfx("\r\n");
		printfx((char *)params.PN);
		printfx("\r\n"); 
	}
	else if(strcmp(str,"w")==0)
	{
		memset(str,0,sizeof(str));
		sscanf(p,"%*s %*s %[^'\']",str);
		k = strlen(str);			
		if((k > 0)&&(k <= 16))
		{
			memset(params.PN,0,17);
			memcpy(params.PN,str,k);
			ParameterSave();
			printfx(p);
			printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");}  
	}
	else
	{printfx(p);printfx(" fail\r\n");}
	
	return 0;
}

/***************************************************************************
** 函数名称:  	SetPassWord
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2020-1-29
** 修改原因：
** 说    明:    
;***************************************************************************/
char ReadWriteBoardSN(void *p,...)
{
	int k=0;
	char str[48]={0};//,str2[17]={0};
	
	sscanf(p,"%*s %s",str);
	gLowerCaseString(str);
	if((strcmp(str,"?")==0)||(strlen(str)<=0))
	{
		printfx("\r\n");
		printfx((char *)params.SN);
		printfx("\r\n"); 
	}
	else if(strcmp(str,"w")==0)
	{
		memset(str,0,sizeof(str));
		sscanf(p,"%*s %*s %[^'\']",str);
		k = strlen(str);			
		if((k > 0)&&(k <= 16))
		{
			memset(params.SN,0,17);
			memcpy(params.SN,str,k);
			ParameterSave();
			printfx(p);
			printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");} 
	}
	else
	{printfx(p);printfx(" fail\r\n");}	
	return 0;
}
/***************************************************************************
** 函数名称:   
** 功能描述:   
** 输入参数:    无
** 返 回 值:    无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2019-9-16
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char ReadSelectSwitch(void *p,...)
{
	int k=0;
	char str[4]={0,0,0,0},str1[6]={0};
	
	sscanf(p,"%s %s",str1,str);
	
	if(strcmp(str,"?")==0)
	{
		sprintfx("\r\nCHAN:%03d\r\n",swMess.ch);
	}
	else if(strlen(str1) >= 6)
	{	
		gLowerCaseString(str1);
		sscanf(str1,"*sw%d",&k);		
		if(k <= params.nCH)
		{
			uint16_t rd[4]={0,0,0,0};
			
			swMess.ch = k;
			for(k=0;k<4;k++)
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
	}
	else
	{printfx(p);printfx(" fail\r\n");}
	return 0;
}

/***************************************************************************
** 函数名称:   
** 功能描述:   
** 输入参数:    无
** 返 回 值:    无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2025-12-5
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char ReadSelectSwitch1(void *p,...)
{
	int k=0;
	char str[6]={0,0,0,0};
	
	if(strstr(p,"?")!=0)
	{
		sprintfx("0,1,%d\r\n",swMess.ch);
	}
	else 
	{	
		sscanf(p,"%*s %*[^,],%[^'\']",str);
		if(strlen(str)>0)
		{
			k=atoi(str);
			if(k <= params.nCH)
			{
				uint16_t rd[4]={0,0,0,0};
				
				swMess.ch = k;
				for(k=0;k<4;k++)
				{
					flash_read_level(swMess.ch,rd);
					if(rd[0]||rd[1]||rd[2]||rd[3])
					{
						SPI1_WriteAD5504(rd,4);
					}
				}
				sprintfx("0,1,%d\r\n",swMess.ch);
			}
			else
			{sprintfx("1,1,%d\r\n",k);}	
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	
	return 0;
}

/***************************************************************************
** 函数名称:   
** 功能描述:   
** 输入参数:    无
** 返 回 值:    无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2022-3-1
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char ReadWriteMaxCH(void *p,...)
{
	int k=0;
	char str[4]={0,0,0,0};
	
	sscanf(p,"%*s %s",str);	
	gLowerCaseString(str);
	if(strcmp(str,"?")==0)
	{
		sprintfx("\r\nmax %d\r\n",params.nCH);
	}
	else if(strcmp(str,"w")==0)
	{
		sscanf(p,"%*s %*s %d",&k);
		if(k <= LEVEL_NUM)
		{
			uint8_t c=0xff,t=k;
			
			if(t>0)
			{t--;}
			params.nCH = k;
			if(params.nCH <= 0)
			{params.pps=0;}
			else
			{
				for(k=0;k<8;k++)
				{
					if(t & 0x80)
					{
						params.pps=c;
						break;
					}
					else
					{t <<= 1;c>>=1;}
				}
			}
			ParameterSave();
			printfx(p);
			printfx(" pass\r\n");
		}
		else 
		{printfx(p);printfx(" fail\r\n");}	
	}	
	else 
	{printfx(p);printfx(" fail\r\n");}
	
	return 0;
}
/***************************************************************************
** 函数名称:
** 功能描述:
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量:
** 调用模块:
** 作　  者:   M-L-A
** 日　  期:   2022-3-1
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char LevelValueDispose(void *p,...)
{
	int i=0;//,k=0;
	char str[8]={0};
	double ft=0.0,fv[4]={0.0,0.0,0.0,0.0};
	
	sscanf(p,"%*s %s %d %lf %lf %lf %lf",str,&i,&fv[0],&fv[1],&fv[2],&fv[3]);	
	if(i <= params.nCH)
	{
		gLowerCaseString(str);	
		if(strcmp(str,"w")==0)
		{//write
//			if(i==0)
//			{
//				sLevel[i].xp = DAC_CH_A;
//				sLevel[i].xn = DAC_CH_B;
//				sLevel[i].yp = DAC_CH_C;
//				sLevel[i].yn = DAC_CH_D;			
//			}
//			else
		{
			uint8_t seg_idx;
			float interp_k;

			// X+ 通道 - 使用插值k系数和偏移量
			seg_idx = GetSegmentIndex(fv[0]);
			interp_k = GetInterpolatedK(0, seg_idx, fv[0]);
			ft = interp_k / 100000.0;
			ft = fv[0] - ft;
//			ft = volt_table[(uint8_t)fv[0] - 1];  // cb校准时使用
			ft = ft * 4096 / REFERENCE;
			if((ft - (uint16_t)ft) > 0.5) 
			{
				ft++;
			}
			sLevel.xp = ft;
			sLevel.xp &= 0x0fff;
			sLevel.xp |= DAC_CH_A;

			// X- 通道
			seg_idx = GetSegmentIndex(fv[1]);
			interp_k = GetInterpolatedK(1, seg_idx, fv[1]);
			ft = interp_k / 100000.0;
			ft = fv[1] - ft;
//			ft = volt_table[(uint8_t)fv[1] - 1];  // cb校准时使用
            ft = ft * 4096 / REFERENCE;
			if((ft - (uint16_t)ft) > 0.5) 
			{
				ft++;
			}
			sLevel.xn = ft;
			sLevel.xn &= 0x0fff;
			sLevel.xn |= DAC_CH_B;

			// Y+ 通道
			seg_idx = GetSegmentIndex(fv[2]);
			interp_k = GetInterpolatedK(2, seg_idx, fv[2]);
			ft = interp_k / 100000.0;
			ft = fv[2] - ft;
//			ft = volt_table[(uint8_t)fv[2] - 1];  // cb校准时使用
			ft = ft * 4096 / REFERENCE;
			if((ft - (uint16_t)ft) > 0.5) 
			{
				ft++;
			}
			sLevel.yp = ft;
			sLevel.yp &= 0x0fff;
			sLevel.yp |= DAC_CH_C;

			// Y- 通道
			seg_idx = GetSegmentIndex(fv[3]);
			interp_k = GetInterpolatedK(3, seg_idx, fv[3]);
			ft = interp_k / 100000.0;
			ft = fv[3] - ft;
//			ft = volt_table[(uint8_t)fv[3] - 1];  // cb校准时使用
			ft = ft * 4096 / REFERENCE;
			if((ft - (uint16_t)ft) > 0.5) 
			{
				ft++;
			}
			sLevel.yn = ft;
			sLevel.yn &= 0x0fff;
			sLevel.yn |= DAC_CH_D;
		}
			swMess.ch = i;
			sLevel.ch=i;
			SPI1_WriteAD5504(&sLevel.xp,4);	
			printfx(p);printfx(" pass\r\n");		
		}
		else if(strcmp(str,"r")==0)
		{//read
			uint16_t rd[4]={0,0,0,0};
//				uint8_t seg_idx;
//				int16_t interp_k, interp_ol;
				uint16_t raw_v;

			sscanf(p,"%*s %*s %d",&i);//,&k);
			if(i<=params.nCH)	//((i<=params.nCH)&&(k<=4))
			{
				flash_read_level(i,rd);
				raw_v = rd[0] & 0x0fff;
				fv[0] = raw_v * REFERENCE/4096;
//				fv[0] = params.ol[0];
//				fv[0] = fv[0]/10000;
//				ft = ft + fv[0];
//				fv[0] = ft + ft*params.k[0]/10000;
				
				raw_v = rd[1] & 0x0fff;
				fv[1] = raw_v * REFERENCE/4096;
//				fv[1] = params.ol[1];
//				fv[1] = fv[1]/10000;
//				ft = ft + fv[1];
//				fv[1] = ft + ft*params.k[1]/10000;
				
				raw_v = rd[2] & 0x0fff;
				fv[2] = raw_v * REFERENCE/4096;
//				fv[2] = params.ol[2];
//				fv[2] = fv[2]/10000;
//				ft = ft + fv[2];
//				fv[2]= ft + ft*params.k[2]/10000;
				
				raw_v = rd[3] & 0x0fff;
				fv[3] = raw_v * REFERENCE/4096;
//				fv[3] = params.ol[3];
//				fv[3] = fv[3]/10000;
//				ft = ft + fv[3];
//				fv[3]= ft + ft*params.k[3]/10000;
				sprintfx("X+:%0.3lf, X-:%0.3lf, Y+:%0.3lf, Y-:%0.3lf\r\n",fv[0],fv[1],fv[2],fv[3]);		
			}
			else
			{printfx(p);printfx(" fail\r\n");}
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else
	{printfx(p);printfx(" fail\r\n");}	
	
	return 0;
}

/***************************************************************************
** 函数名称:  	
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2022-5-26
** 修改原因:   增加X+ X- Y+ Y-电压校准相关命令
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char OffsetChannel(void *p,...)
{
	char str[4]={0,0,0,0};
	double fv=0.0,fv1=0.0,fk=0.0;
	int k;
	uint16_t tar[4]={0,0,0,0};

    sscanf(p,"%*s %s %d %lf %lf",str,&k,&fv,&fv1);

	gLowerCaseString(str);
	if(strcmp(str,"od")==0)
	{//dac output data
		if(k>0 && k<=4)
		{
			tar[0] = DAC_CH_A;
			tar[1] = DAC_CH_B;
			tar[2] = DAC_CH_C;
			tar[3] = DAC_CH_D;
			
			fv = fv * 4096 / REFERENCE;
			if((fv - (uint16_t)fv) > 0.5) 
			{
				fv++;
			}
			tar[k-1] |= (((uint16_t)(fv))&0x0fff);

			SPI1_WriteAD5504(tar,4);
			printfx(p);printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
    else if(strcmp(str,"wk")==0)
    {	//写分段系数 wk 通道 分段 系数值
		if(k>0 && k<=4 && fv<SEGMENT_NUM)
		{
			params.k[k-1][(uint8_t)fv] =  fv1*100000;	//保留5位小数点
			params.ol[k-1][(uint8_t)fv] = (fv*1000);	//保留3位小数点
			ParameterSave();
			printfx(p);printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
    else if(strcmp(str,"wl")==0)
    {	//写分段偏移量 wl 通道 偏移值
		if(k>0 && k<=4 && fv<SEGMENT_NUM)
		{
			params.ol[k-1][(uint8_t)fv] = fv*1000;	//保留3位小数点
			ParameterSave();
			printfx(p);printfx(" pass\r\n");
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else if(strcmp(str,"rk")==0)
	{	//读分段系数 rk 通道 分段
		if(k>0 && k<5 && fv<SEGMENT_NUM)
		{
			for(uint8_t i = 0;i < SEGMENT_NUM;i++)
			{
				if(params.ol[k-1][i] != 0)
				{
					fv = params.k[k-1][i];
					fv = fv/100000;
					fv1 = params.ol[k-1][i];
					fv1 = fv1/1000;
					sprintfx("\r\nch = %d  ol = %0.3f  k = %0.5f", k, fv1, fv);
				}
			}
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else if(strcmp(str,"clk")==0)
	{	//清除所有分段系数
		if(k>0 && k<5)
		{
			for(uint8_t i = 0;i < SEGMENT_NUM;i++)
			{
				params.k[k-1][i] = 0;
				params.ol[k-1][i] = 0;
			}
			ParameterSave();
			sprintfx("\r\nCH %d k clear success\r\n", k);
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else if(strcmp(str,"rl")==0)
	{//读分段偏移量 rl 通道 分段
		if(k>0 && k<5 && fv<SEGMENT_NUM)
		{
			fv=params.ol[k-1][(uint8_t)fv];
			fv=fv/1000;
			sprintfx("\r\n%0.3f\r\n",fv);
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else if(strcmp(str,"cb")==0)
	{//分段校准计算 cb 通道 理论值 实测值
		if(k>0 && k<5 && fv<SEGMENT_NUM)
		{
			params.ol[k-1][(uint8_t)fv] = fv*1000;
			fk = (fv1-fv)*100000;
			params.k[k-1][(uint8_t)fv] = fk;
			ParameterSave();
			printfx(p);printfx(" pass\r\n");
//			sprintfx("ch %d fv = %d k = %d\r\n", k, (uint8_t)fv, params.k[k-1][(uint8_t)fv]);
		}
		else
		{printfx(p);printfx(" fail\r\n");}
	}
	else
	{printfx(p);printfx(" fail\r\n");}

	return 0;
}

/***************************************************************************
** 函数名称:  	
** 功能描述:  	 
** 输入参数: 	 无
** 返 回 值: 	 无
** 全局变量: 
** 调用模块: 
** 作　  者:   M-L-A
** 日　  期:   2022-5-26
** 修改原因：
** 说    明:    字数--ASCII;  0--30; 9--39;
;***************************************************************************/
char SetReadWrite(void *p,...)
{
	char str1[2]={0,0},str2[8]={0,0,0,0,0,0,0,0};
	int k=0;
	
    sscanf(p,"%*s %s %s",str1,str2);
    
	gLowerCaseString(str1);	
	gLowerCaseString(str2);
	if(strcmp(str1,"w")==0)
	{
		if(strcmp(str2,"ls")==0)
		{
			if(flash_LevelSave(&sLevel)==0)
			{
				printfx(p);
				printfx(" pass\r\n");
			}
			else
			{
				printfx(p);
				printfx(" fail\r\n");
			}
		}
		else if(strcmp(str2,"bs")==0)
		{
			sscanf(p,"%*s %*s %*s %d",&k);
			switch(k)
			{
				case 0:break;
				case 1:BOOST_SHDN(0);break; //BOOST OFF
				case 2:BOOST_SHDN(1);break;	//BOOST ON
				case 3:BOOST_FSET(0);break; //boost 650khz
				case 4:BOOST_FSET(1);break; //boost 1.1mhz
			}
			printfx(p);
			if(k>0)
			{printfx(" pass\r\n");}
			else
			{printfx(" fail\r\n");}
		}
		else if(strcmp(str2,"pps")==0)
		{//Parallel port strobe
			memset(str1,0,sizeof(str1));
			sscanf(p,"%*s %*s %*s %d",&k);
			if(k>0)
			{
				params.pps=k;
				ParameterSave();
				printfx(p);printfx(" pass\r\n");
			}
			else
			{printfx(p);printfx(" fail\r\n");}
		}
		else
		{printfx(p);printfx(" fail\r\n");}		
	}
	else if(strcmp(str1,"r")==0)
	{
		if(strcmp(str2,"ec")==0)
		{
			sprintfx("\r\nEC %d\r\n",erCode.regis);
		}
		else if(strcmp(str2,"pps")==0)
		{//Parallel port strobe
			sprintfx("PPS 0x%02X\r\n",params.pps);
		}
		else
		{printfx(p);printfx(" fail\r\n");}  
	}	
	else
	{printfx(p);printfx(" fail\r\n");}  
	return 0;
}

/***************************************************************************
** 函数名称:   GetInterpolatedK
** 功能描述:   获取插值后的k系数（前后两个有效段k系数的线性插值）
** 输入参数:   ch - 通道(0-3), seg - 当前段索引, voltage - 当前电压值
** 返 回 值:   插值后的k系数（已缩放100000倍）
** 全局变量:
** 调用模块:
** 作　  者:   G-D-L
** 日　  期:   2026-7-2
** 修改原因：
** 说    明:    取前后两个有效段的k系数进行线性插值
;***************************************************************************/
float GetInterpolatedK(uint8_t ch, uint8_t seg, double voltage)
{
	uint16_t vol = voltage*1000;
	if(params.ol[ch][seg] == vol) // 如果当前段有效，直接返回当前段的k值
	{
		return (float)params.k[ch][seg];
	}
	
	// 向左右两边搜索最近的非零系数段
	int8_t left = seg;//seg - 1;
	int8_t right = seg + 1;

	while(left >= 0 && params.k[ch][left] == 0) // 查找左边最近的非零段
	{
		left--;
	}

	while(right < SEGMENT_NUM && params.k[ch][right] == 0) // 查找右边最近的非零段
	{
		right++;
	}

	if(left >= 0 && right < SEGMENT_NUM) // 根据查找结果进行插值 左右都有效，进行线性插值
	{
		// left_seg: left段的起始电压, right_seg: right段的起始电压
		double left_v = params.ol[ch][left];      // 左段起始电压
		double right_v = params.ol[ch][right];    // 右段起始电压
		int16_t left_k = params.k[ch][left];
		int16_t right_k = params.k[ch][right];
		
		// 线性插值公式: k = k1 + (k2-k1) * (v-v1) / (v2-v1)
		float ratio = (right_k - left_k);
		return (float)(left_k + (((voltage*1000 - left_v)/(right_v - left_v)) * ratio));
	}
	else if(left >= 0) // 只有左边有效
	{
		return (float)params.k[ch][left];
	}
	else if(right < SEGMENT_NUM) // 只有右边有效
	{
		return (float)params.k[ch][right];
	}
	
	return 0; // 都没有找到，返回0
}

/***************************************************************************
** 函数名称:   GetSegmentIndex
** 功能描述:   根据电压值获取分段校准索引
** 输入参数:   voltage - 电压值(0~60V)
** 返 回 值:   分段索引(0~59)
** 全局变量:
** 调用模块:
** 作　  者:   G-D-L
** 日　  期:   2026-7-2
** 修改原因：
** 说    明:    将0-60V分为60段，每段1V
;***************************************************************************/
uint8_t GetSegmentIndex(double voltage)
{
	if(voltage < 0) voltage = 0;
	if(voltage > REFERENCE) voltage = REFERENCE;

	// 60段，每段1V: 0-1V用seg0, 1-2V用seg1, ..., 59-60V用seg59
	uint8_t seg = (uint8_t)(voltage);
	if(seg >= SEGMENT_NUM) seg = SEGMENT_NUM - 1;
	return seg;
}

