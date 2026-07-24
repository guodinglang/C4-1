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

#ifndef  __COMMAND_H__
#define  __COMMAND_H__

#include "projectCommon.h"

/*********************************************************************************/
int gLowerCaseString(char *str);
void RX0_CommandDispose(void);
char help(void *p,...);
char ReadVersion(void *p,...);
char RebackCharacter(void *p,...);
char ReadWriteBoardIDN(void *p,...);
char ReadWriteBoardPN(void *p,...);
char ReadWriteBoardSN(void *p,...);
char ReadSelectSwitch(void *p,...);
char ReadWriteMaxCH(void *p,...);
char LevelValueDispose(void *p,...);
char OffsetChannel(void *p,...);
char SetReadWrite(void *p,...);
char ReadSelectSwitch1(void *p,...);
uint8_t GetSegmentIndex(double voltage);
float GetInterpolatedK(uint8_t ch, uint8_t seg, double voltage);

#endif
