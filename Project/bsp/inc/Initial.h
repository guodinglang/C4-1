
#ifndef  __INITIAL__
#define  __INITIAL__


#include "projectCommon.h"
//--------------------------------------------------------------------------
#define ON          	1
#define OFF         	0
#define LEVEL_NUM   	255	//电平数量
#define REFERENCE	 	60.000
#define SEGMENT_NUM		60	//分段校准数量,0-60V每段1V

//--------------------------------------------------------------------------
//typedef struct{		
//	unsigned int   cn[4];
//	unsigned int   tOFF;
//	unsigned short time;	//time
//	unsigned char  value;	
//	union {
//		unsigned short reg;
//		struct {
//			unsigned short power:1;
//			unsigned short Poff:1;	//power off
//			unsigned short set:1;	//
//			unsigned short zero:1;
//			unsigned short up:1;
//			unsigned short down:1;
//			unsigned short bud:1;	//both up and down key
//		}bit;
//	}flag;
//}ButtonManage;

//extern ButtonManage *pkey;
//--------------------------------------------------------------------------
typedef struct{
	uint8_t 	fInitial;	//flash initial
	uint8_t  	IDN[25];	//产品信息24 字节
	uint8_t  	PN[17];	    //产品编号16 字节
	uint8_t  	SN[17];	    //产品序列号16 字节
	uint16_t    nCH;		//最大通道数
	uint8_t     pps;		//Parallel port strobe
	int32_t     k[4][SEGMENT_NUM];		//校准点系数 保留5位小数点 [通道][分段]
	uint16_t    ol[4][SEGMENT_NUM];		//offset level voltage [通道][分段]
//	uint16_t    tvd;		//transform voltages  delay ms
	uint8_t     ver[25];	//version
}ParameterSturct;

extern ParameterSturct  params;
//--------------------------------------------------------------------------
typedef struct{	
	uint16_t	xp;
	uint16_t	xn;
	uint16_t	yp;
	uint16_t	yn;	
	uint16_t    ch;
//	uint16_t	tvn;	//transform voltages number
	
}LevelSturct;

extern LevelSturct  sLevel;
//-------------------------------------------------------------------------





//-------------------------------------------------------------------------
void SysTickConfigure(void);
void epramInitialize(void);
void bsp_Init(void);
void CheckUniqueID(void);
double ffp(double fm, int leng);
char ftoa(double fv,unsigned char decimals,char *str);
char itoa(int tv,char *str);
void configWDT(uint16_t d);
uint8_t flash_LevelSave(LevelSturct *ls);




#endif





