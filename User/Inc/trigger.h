/************************************************************************
 * 触发感应控制
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
#ifndef TRIGGER_H
#define TRIGGER_H

#include <stdint.h>
#include "board_config.h"

typedef struct{
  uint32_t keep_time;
  uint32_t brigress_on;
  uint32_t tau_turn_on;
  uint32_t tau_turn_off;
}trigger_setting;

typedef struct{
  int isEnabled;
  uint32_t auto_off_tick;
  uint32_t trig_count;
}trigger_handler;

void Trigger_Init();
void Trigger_Set_PowerOff(uint32_t ms);
void Trigger_Callback(int index);
void Trigger_Tick_Callback();

#endif
