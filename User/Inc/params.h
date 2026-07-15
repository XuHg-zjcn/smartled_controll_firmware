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
#ifndef PARAMS_H
#define PARAMS_H

#include "trigger.h"

#define PARAMS_MAGIC           (0x6d506c73)
#define PARAMS_MAGIC_OVERWRITE (0x00006c73)

typedef enum{
  Light_Mode_Off = 0,
  Light_Mode_Keep,
  Light_Mode_Trig, //触发模式
}Light_Mode;

typedef struct{
  uint32_t magic;         //magic头
  uint16_t length;        //长度
  uint16_t count;         //写入计数
  uint32_t write_ts;      //写入UNIX时间
  Light_Mode light_mode;  //灯光模式
  trigger_setting trig_sett;//触发设置
}Params_t;

void Params_Init();
void Params_Load_Default();
void Params_set(int pos, uint16_t data);
void Params_Write_To_Flash();

#endif
