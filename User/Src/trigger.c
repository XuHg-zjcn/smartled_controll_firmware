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
#include "py32f0xx.h"
#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_gpio.h"
#include "py32f0xx_ll_exti.h"
#include "params.h"
#include "trigger.h"
#include "brigress.h"

#define sett (params.trig_sett)

extern Params_t params;
uint32_t auto_off_tick = 0;
uint32_t auto_off_tau;


void Trigger_Init()
{
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);

  LL_GPIO_SetPinMode(TRIG1_GPIO_PORT, TRIG1_LL_GPIO_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(TRIG1_GPIO_PORT, TRIG1_LL_GPIO_PIN, LL_GPIO_PULL_DOWN);
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, TRIG1_LL_EXTI_CONFIG);
  LL_EXTI_EnableFallingTrig(TRIG1_LL_EXTI_LINE);
  LL_EXTI_EnableRisingTrig(TRIG1_LL_EXTI_LINE);
  LL_EXTI_EnableIT(TRIG1_LL_EXTI_LINE);

  LL_GPIO_SetPinMode(TRIG2_GPIO_PORT, TRIG2_LL_GPIO_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(TRIG2_GPIO_PORT, TRIG2_LL_GPIO_PIN, LL_GPIO_PULL_DOWN);
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, TRIG2_LL_EXTI_CONFIG);
  LL_EXTI_EnableFallingTrig(TRIG2_LL_EXTI_LINE);
  LL_EXTI_EnableRisingTrig(TRIG2_LL_EXTI_LINE);
  LL_EXTI_EnableIT(TRIG2_LL_EXTI_LINE);

  NVIC_SetPriority(EXTI0_1_IRQn, 3);
  NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void Trigger_Exec(trigger_setting *p)
{
  uint32_t tick = HAL_GetTick();
  if(!LED_GetOutputEnable(LED_MAIN)){
    LED_SetOutputEnable(LED_MAIN, 1);
  }
  Brigress_SetGradExpone(p->brigress_on, p->tau_turn_on);
  if((p->keep_time != UINT32_MAX) && (tick + p->keep_time > auto_off_tick)){
    auto_off_tick = tick + p->keep_time;
    auto_off_tau = p->tau_turn_off;
  }
}

void Trigger_cancal_auto_off()
{
  auto_off_tick = 0;
}

void Trigger_Tick_Callback()
{
  if((auto_off_tick != 0) && (HAL_GetTick() >= auto_off_tick)){
    auto_off_tick = 0;
    Brigress_SetGradExpone(0, auto_off_tau);
  }
}
