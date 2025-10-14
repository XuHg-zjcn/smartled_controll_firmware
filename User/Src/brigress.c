/************************************************************************
 * 智能灯亮度缓变控制
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
#include "brigress.h"
#include <math.h>
#include "ops64.h"
#include "func32.h"
#include "py32f0xx_hal.h"
#include "led.h"


typedef enum{
  ChargeDisable = 0,
  ChargeLinear,
  ChargeExpone,
  ChargeDoubleExp,
}ChargeMode;

static ChargeMode mode;
static uint32_t target_pwm;
static int32_t c1;
static uint32_t c2;
static uint32_t tick0, t_max;

void Brigress_Tick_Callback()
{
  if(mode == ChargeDisable){
    return;
  }
  uint32_t tick = HAL_GetTick();
  uint32_t t_ms = tick-tick0;
  uint32_t pwm;
  if(t_ms >= t_max){
    pwm = target_pwm;
    mode = ChargeDisable;
  }else{
    switch(mode){
    case ChargeLinear:
      pwm = c2 + ((c1*t_ms)>>16);
      break;
    case ChargeExpone:
      pwm = target_pwm-I32xU32_HI32(c1, exp32x(UINT32_MAX-c2*t_ms));
      break;
    default:
      pwm = target_pwm;
      mode = ChargeDisable;
    }
  }
  LED_SetOutputCompare(LED2, pwm);
}

//L(t) = L0 + (L'-L0)*t/tc       (0<=t<=tc)
//TODO: 此函数降低亮度时会达到最高亮度，然后
void Brigress_SetGradLinear(uint32_t target, uint32_t tc_ms)
{
  uint32_t old_pwm = LED_GetOutputCompare(LED2);
  int32_t delta = target - old_pwm;
  mode = ChargeDisable;
  c1 = ((int64_t)(delta<<16))/tc_ms;
  c2 = old_pwm;
  t_max = tc_ms;
  tick0 = HAL_GetTick();
  target_pwm = target;
  mode = ChargeLinear;
}

//L(t) = L' + (L0-L')exp(-t/tc)  (t>=0)
void Brigress_SetGradExpone(uint32_t target, uint32_t tc_ms)
{
  const uint32_t const2_27_LN2 = round(((double)(1<<27))/M_LN2);
  uint32_t old_pwm = LED_GetOutputCompare(LED2);
  int32_t delta = target - old_pwm;
  mode = ChargeDisable;
  c1 = delta;
  c2 = const2_27_LN2/tc_ms;
  t_max = tc_ms*6;
  tick0 = HAL_GetTick();
  target_pwm = target;
  mode = ChargeExpone;
}

uint32_t Brigress_GetTargetPWM_Or_Curr()
{
  if(mode == ChargeDisable){
    return LED_GetOutputCompare(LED2);
  }else{
    return target_pwm;
  }
}
