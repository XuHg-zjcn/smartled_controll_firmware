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
  uint16_t brigress_on;
  uint16_t resved;
  uint32_t keep_time;
  uint32_t tau_turn_on;
  uint32_t tau_turn_off;
}trigger_setting;

void Trigger_Init();
void Trigger_Exec(trigger_setting *p);
void Trigger_cancal_auto_off();
void Trigger_Tick_Callback();

#endif
