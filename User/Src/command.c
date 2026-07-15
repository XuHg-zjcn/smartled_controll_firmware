/************************************************************************
 * 命令解析
 * Copyright (C) 2025  Xu Ruijun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ************************************************************************/
#include "command.h"
#include <stdint.h>
#include <string.h>
#include "led.h"
#include "modbus.h"
#include "params.h"

uint8_t resp_buff[MAXSIZE_RESP];
extern uint16_t adc_isum, adc_vsum;
extern Params_t params;

int MB_ReadCoilCB_single(uint16_t addr)
{
  if((addr >= 0) && (addr < 4)){
    if(LED_GetOutputEnable(addr)){
      return 1;
    }else{
      return 0;
    }
  }else{
    return MB_ERR_ILL_ADDR;
  }
}

int MB_WriteCoilCB_single(uint16_t addr, int state)
{
  if((addr >= 0) && (addr < 4)){
    LED_SetOutputEnable(addr, state);
    return 0;
  }else{
    return MB_ERR_ILL_ADDR;
  }
  return 0;
}

int MB_ReadHoldCB_single(uint16_t addr)
{
  if((addr >= 0) && (addr < 4)){
    return LED_GetOutputCompare(addr);
  }else if((addr > 16) && (addr < 16+(sizeof(Params_t)/2))){
    return ((uint16_t *)&params)[addr-16];
  }else{
    return MB_ERR_ILL_ADDR;
  }
}

int MB_WriteHoldCB_single(uint16_t addr, uint16_t value)
{
  if((addr >= 0) && (addr < 4)){
    LED_SetOutputCompare(addr, value);
  }else if((addr > 16) && (addr < 16+(sizeof(Params_t)/2))){
    Params_set(addr-16, value);
  }else{
    return MB_ERR_ILL_ADDR;
  }
  return 0;
}

int MB_ReadInputCB_single(uint16_t addr)
{
  switch(addr){
  case 0x0010:
    return adc_isum;
  case 0x0011:
    return adc_vsum;
  default:
    return MB_ERR_ILL_ADDR;
  }
}

uint32_t process_cmd(uint8_t *p, int32_t size)
{
  return MB_ProcessRecvWithCRC(p, size, resp_buff);
}
