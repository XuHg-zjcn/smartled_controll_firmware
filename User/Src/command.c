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

const uint8_t strinfo[] = "smartled controll v0.1";
uint8_t resp_buff[MAXSIZE_RESP];
uint32_t resp_len = 0;

typedef struct{
  uint8_t code;
  uint8_t min_remain;
  uint32_t (*func)(uint8_t *p, uint32_t remain);
}CmdType;

uint32_t CmdFunc_GetInfo(uint8_t *p, uint32_t remain)
{
  if(resp_len + strlen(strinfo)+1 <= MAXSIZE_RESP){
    strcpy(resp_buff+resp_len, strinfo);
    resp_len += strlen(strinfo)+1;
  }
  return 0;
}

uint32_t CmdFunc_SetOnOff(uint8_t *p, uint32_t remain)
{
  uint8_t mode = *p;
  uint32_t oldStat = LED_GetOutputEnable();
  uint32_t newStat = (oldStat&(mode&0x0f)) | (((~oldStat)&0x0f)&(mode>>4));
  uint32_t charge = oldStat ^ newStat;
  for(int i=0;i<4;i++){
    if(charge & (1U<<i)){
      LED_SetOutputEnable(i, (newStat&(1U<<i))?1:0);
    }
  }
  return 1;
}

uint32_t CmdFunc_GetOnOff(uint8_t *p, uint32_t remain)
{
  uint32_t oldStat = LED_GetOutputEnable();
  if(resp_len + 1 <= MAXSIZE_RESP){
    resp_buff[resp_len++] = oldStat & 0xff;
  }
  return 0;
}

uint32_t CmdFunc_SetPWM(uint8_t *p, uint32_t remain)
{
  uint16_t data = (*p++)|((*p++)<<8);
  uint16_t compval = data & 0x3fff;
  uint32_t id = data>>14;
  LED_SetOutputCompare(id, compval);
  return 2;
}

uint32_t CmdFunc_GetPWM(uint8_t *p, uint32_t remain)
{
  uint8_t id = *p;
  uint32_t compval;
  compval = LED_GetOutputCompare(id);
  if(resp_len + 2 <= MAXSIZE_RESP){
    resp_buff[resp_len++] = compval & 0xff;
    resp_buff[resp_len++] = (compval>>8) & 0xff;
  }
  return 1;
}

CmdType CmdArr[] = {
  {0x00, 0, CmdFunc_GetInfo},
  {0x11, 1, CmdFunc_SetOnOff},
  {0x12, 0, CmdFunc_GetOnOff},
  {0x13, 2, CmdFunc_SetPWM},
  {0x14, 1, CmdFunc_GetPWM},
};

uint32_t process_cmd(uint8_t *p, int32_t size)
{
  resp_len = 0;
  while(size > 0){ //size使用有符号类型，防止下溢后继续循环
    uint32_t code = *p++;
    size--;
    uint32_t a = 0;
    uint32_t cmd_i = -1;
    uint32_t b = sizeof(CmdArr)/sizeof(CmdType)-1;
    while(a<=b){
      uint32_t mid = (a+b)/2;
      if(CmdArr[mid].code < code){
	a = mid+1;
      }else if(CmdArr[mid].code > code){
	b = mid-1;
      }else{
	cmd_i = mid;
        break;
      }
    }
    if(cmd_i == -1){
      //未知命令
      return 1;
    }
    if(size >= CmdArr[cmd_i].min_remain){
      uint32_t used = CmdArr[cmd_i].func(p, size);
      p += used;
      size -= used;
    }
  }
  return 0;
}
