/************************************************************************
 * 参数加载和储存
 * Copyright (C) 2026  Xu Ruijun
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
#include "params.h"
#include "flash.h"
#include <string.h>

#define ADDR_MAX         (0x08010000-sizeof(Params_t))
#define GET_MAGIC(addr)  (((Params_t *)addr)->magic)

const Params_t params_default = {
  .magic = PARAMS_MAGIC,
  .length = sizeof(Params_t) - 6,
  .count = 0,
  .write_ts = 0,
  .trig_en_bitmap = 0xffffffff,
  .trig_poweron = {
    .brigress_on = 100,
    .keep_time =    30*1000,
    .tau_turn_on =   5*1000,
    .tau_turn_off = 30*1000},
  .trig_key = {
    .brigress_on = 300,
    .keep_time = 4*3600*1000,
    .tau_turn_on =    2*1000,
    .tau_turn_off =  30*1000},
  .trig_ext = {
    .brigress_on = 200,
    .keep_time =    30*1000,
    .tau_turn_on =   5*1000,
    .tau_turn_off = 30*1000},
};

Params_t params; //当前参数,可临时修改
uint32_t addr_curr;


void Params_Init()
{
  uint32_t addr = 0x0800fc00;
  if(GET_MAGIC(addr) == 0xffffffff){
    Params_Load_Default();
  }
  while((GET_MAGIC(addr) == PARAMS_MAGIC_OVERWRITE) &&
	((uint32_t)addr <= ADDR_MAX)){
    addr += sizeof(Params_t);
    if((addr&0x7f) + sizeof(Params_t) > 0x80){
      addr = (addr&0xffffff80) + 0x80;
    }
  }
  if(addr > ADDR_MAX){
    Params_Load_Default();
  }else if(GET_MAGIC(addr) == PARAMS_MAGIC){
    addr_curr = addr;
    params = *((Params_t *)addr);
  }
}

void Params_Load_Default()
{
  params = params_default;
}

void Params_set(int pos, uint16_t data)
{
  uint16_t *p = ((uint16_t *)&params) + pos;
  *p = data;
}

void Params_Write_To_Flash()
{
  uint32_t addr;
  uint32_t buff[128];
  if(addr_curr){
    addr = addr_curr;
  }else{
    addr = 0x0800fc00;
  }
  while((GET_MAGIC(addr) != 0xffffffff) &&
	((uint32_t)addr <= ADDR_MAX)){
    addr += sizeof(Params_t);
    if((addr&0x7f) + sizeof(Params_t) > 0x80){
      addr = (addr&0xffffff80) + 0x80;
    }
  }
  if(addr > ADDR_MAX){
    return;
  }
  memset((void *)buff, 0xff, 128);
  memcpy(((void *)buff)+(addr&0x7f), &params, sizeof(Params_t));
  FLASH_PageProgram_Block(addr&0xffffff80, buff);
}
