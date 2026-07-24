/**
  **************************************************************************
  * @file     flash.c
  * @version  v2.0.5
  * @date     2022-05-20
  * @brief    flash program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
 */

#include "flash.h"

/***************************************************************************
;** 函数名称:  	flash_read
;** 功能描述:  	read data using halfword mode
;** 输入参数: 	read_addr: the address of reading ;  num_read: the number of reading data
;** 返 回 值: 	p_buffer: the buffer of reading data
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：
;***************************************************************************/
void flash_read_byte(uint32_t addr, uint8_t *pd, uint8_t num)
{
	uint8_t i;
	
	for(i = 0; i < num; i++)
	{
		pd[i] = (*(__IO uint8_t *)(addr));
		addr ++;
	}
}

/***************************************************************************
;** 函数名称:  	flash_read
;** 功能描述:  	read data using halfword mode
;** 输入参数: 	read_addr: the address of reading ;  num_read: the number of reading data
;** 返 回 值: 	p_buffer: the buffer of reading data
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：
;***************************************************************************/
void flash_read_halfword(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read)
{
  uint16_t i;
  for(i = 0; i < num_read; i++)
  {
    p_buffer[i] = (*(uint16_t*)(read_addr));
    read_addr += 2;
  }
}

/***************************************************************************
;** 函数名称:  	flash_read
;** 功能描述:  	read data using halfword mode
;** 输入参数: 	read_addr: the address of reading ;  num_read: the number of reading data
;** 返 回 值: 	p_buffer: the buffer of reading data
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：
;***************************************************************************/
void flash_read_word(uint32_t addr, uint32_t *pd, uint16_t num)
{
	uint16_t i;
	for(i = 0; i < num; i++)
	{
		pd[i] = (*(uint32_t*)(addr));
		addr += 4;
	}
}



/***************************************************************************
;** 函数名称:  	flash_read
;** 功能描述:  	read data using halfword mode
;** 输入参数: 	read_addr: the address of reading ;  num_read: the number of reading data
;** 返 回 值: 	p_buffer: the buffer of reading data
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：

level[i].xp = (*(float *)(addr)); //读取float

;***************************************************************************/
void flash_read_level(uint16_t ch,uint16_t *rd)
{
	uint32_t addr=0;
	
	if(ch <128)
	{addr=FLASH_SECTION_LEVEL1;}
	else if(ch <256)
	{
		addr=FLASH_SECTION_LEVEL2;
		ch = ch-128;
	}
	
    addr += ch*8;
	rd[0] = (*(uint16_t *)(addr));
	addr += 2;
	rd[1] = (*(uint16_t *)(addr));
	addr += 2;
	rd[2] = (*(uint16_t *)(addr));
	addr += 2;
	rd[3] = (*(uint16_t *)(addr));
	addr += 2;
	
	if(rd[0] >=0xffff)
	{rd[0]=0;}
	if(rd[1] >=0xffff)
	{rd[1]=0;}
	if(rd[2] >=0xffff)
	{rd[2]=0;}
	if(rd[3] >=0xffff)
	{rd[3]=0;}
}

/**
  * @brief  write data using halfword mode without checking
  * @param  write_addr: the address of writing
  * @param  p_buffer: the buffer of writing data
  * @param  num_write: the number of writing data
  * @retval none
  */
void flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write)
{
  uint16_t i;
  for(i = 0; i < num_write; i++)
  {
    flash_halfword_program(write_addr, p_buffer[i]);
    write_addr += 2;
  }
}

/**
  * @brief  write data using word mode without checking
  * @param  write_addr: the address of writing
  * @param  p_buffer: the buffer of writing data
  * @param  num_write: the number of writing data
  * @retval none
  */
void flash_write_word_nocheck(uint32_t write_addr, uint32_t *p_buffer, uint16_t num_write)
{
  uint16_t i;
  for(i = 0; i < num_write; i++)
  {
    flash_word_program(write_addr, p_buffer[i]);
    write_addr += 4;
  }
}

/**
  * @brief  write data using halfword mode with checking
  * @param  write_addr: the address of writing
  * @param  p_buffer: the buffer of writing data
  * @param  num_write: the number of writing data
  * @retval none
  */
//void flash_write(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write)
//{
//  uint32_t offset_addr;
//  uint32_t sector_position;
//  uint16_t sector_offset;
//  uint16_t sector_remain;
//  uint16_t i;

//  flash_unlock();
//  offset_addr = write_addr - FLASH_BASE;
//  sector_position = offset_addr / SECTOR_SIZE;
//  sector_offset = (offset_addr % SECTOR_SIZE) / 2;
//  sector_remain = SECTOR_SIZE / 2 - sector_offset;
//  if(num_write <= sector_remain)
//    sector_remain = num_write;
//  while(1)
//  {
//    flash_read(sector_position * SECTOR_SIZE + FLASH_BASE, flash_buf, SECTOR_SIZE / 2);
//    for(i = 0; i < sector_remain; i++)
//    {
//      if(flash_buf[sector_offset + i] != 0xFFFF)
//        break;
//    }
//    if(i < sector_remain)
//    {
//      flash_sector_erase(sector_position * SECTOR_SIZE + FLASH_BASE);
//      for(i = 0; i < sector_remain; i++)
//      {
//        flash_buf[i + sector_offset] = p_buffer[i];
//      }
//      flash_write_nocheck(sector_position * SECTOR_SIZE + FLASH_BASE, flash_buf, SECTOR_SIZE / 2);
//    }
//    else
//    {
//      flash_write_nocheck(write_addr, p_buffer, sector_remain);
//    }
//    if(num_write == sector_remain)
//      break;
//    else
//    {
//      sector_position++;
//      sector_offset = 0;
//      p_buffer += sector_remain;
//      write_addr += (sector_remain * 2);
//      num_write -= sector_remain;
//      if(num_write > (SECTOR_SIZE / 2))
//        sector_remain = SECTOR_SIZE / 2;
//      else
//        sector_remain = num_write;
//    }
//  }
//  flash_lock();
//}
/***************************************************************************
;** 函数名称:  	ParameterSave
;** 功能描述:  	Flash Write Section
;** 输入参数: 	
;** 返 回 值: 	
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：
;***************************************************************************/
void ParameterSave(void)
{
	uint16_t i;	
	uint32_t addr=FLASH_SECTION_PARAMETER;

	flash_unlock();
	flash_sector_erase(addr);
	
	addr=FLASH_SECTION_PARAMETER + 0;   //保存初始化动作
	flash_byte_program(addr, params.fInitial);
	
	addr=FLASH_SECTION_PARAMETER+IDN_ADDR;
	for(i = 0; i < 25; i++)
	{
		flash_byte_program(addr, params.IDN[i]);
		addr++;
	}
	
	addr=FLASH_SECTION_PARAMETER+PN_ADDR;
	for(i = 0; i < 17; i++)
	{
		flash_byte_program(addr, params.PN[i]);
		addr++;
	}
	
	addr=FLASH_SECTION_PARAMETER+SN_ADDR;
	for(i = 0; i < 17; i++)
	{
		flash_byte_program(addr, params.SN[i]);
		addr++;
	}
	
	addr=FLASH_SECTION_PARAMETER + NCH_ADDR;
	flash_halfword_program(addr, params.nCH);

	// 存储版本信息ver[25] = 25 bytes
	addr=FLASH_SECTION_PARAMETER + VER_ADDR;
	for(i = 0; i < 24; i++)
	{
		flash_byte_program(addr, params.ver[i]);
		addr++;
	}

	addr=FLASH_SECTION_PARAMETER + PPS_ADDR;
	flash_byte_program(addr, params.pps);
	
	// 存储偏移量2字节ol[4][60] = 480 bytes
	flash_write_nocheck(FLASH_SECTION_PARAMETER + OFL_ADDR,(uint16_t *)params.ol,4*SEGMENT_NUM);

	flash_lock();

	// 分段校准参数存储在sector 122 (INTERIM)，因为数据量太大(960 bytes)
	flash_unlock();
	flash_sector_erase(FLASH_SECTION_CALI_K);

	// 存储校准系数4字节k[4][60] = 960 bytes
	flash_write_word_nocheck(FLASH_SECTION_CALI_K + 0,(uint32_t *)params.k,4*SEGMENT_NUM);

	flash_lock();
}
/***************************************************************************
;** 函数名称:  	LevelSave
;** 功能描述:  	Flash Write Section
;** 输入参数: 	
;** 返 回 值: 	
;** 优 先 级：  
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   M-L-A
;** 日　  期:   2022-5-26
;** 修改原因：

flash_word_program(addr, *(uint32_t *)&level[i].xp); //存入float

;***************************************************************************/
uint8_t flash_LevelSave(LevelSturct *ls)
{
	uint8_t r=0;
	uint16_t i,c;	
	uint32_t add,t;
	
	
	flash_unlock();
	flash_sector_erase(FLASH_SECTION_INTERIM);
	
	add=0;
	if(ls->ch <128)
	{add=FLASH_SECTION_LEVEL1;}
	else if(ls->ch <256)
	{
		add = FLASH_SECTION_LEVEL2;
		ls->ch = ls->ch-128;
	}
	
	if(add)
	{
		for(i = 0; i <256; i++)
		{
			t = (*(uint32_t*)(add+4*i));
			flash_word_program(FLASH_SECTION_INTERIM+i*4,t);
		}
		flash_sector_erase(add);
		c = ls->ch*8;	//*8;
		for(i=0;i<256;i++)
		{
			if((i*4)==c)
			{
				flash_halfword_program(add+c,ls->xp);
				flash_halfword_program(add+c+2,ls->xn);
				flash_halfword_program(add+c+4,ls->yp);
				flash_halfword_program(add+c+6,ls->yn);
				i++;
			}
			else
			{
				t = (*(uint32_t*)(FLASH_SECTION_INTERIM+4*i));
				flash_word_program(add+i*4,t);
			}			
		}
		
	}
	else
	{r=1;}
	
	flash_lock();
	
	return r;
}




