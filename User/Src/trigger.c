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
#include "trigger.h"
#include "brigress.h"

uint32_t auto_off_tick = 0;
int auto_off_i;
trigger_handler handlers[N_TRIGGER];
trigger_setting sett = {
  .keep_time = 30000,
  .brigress_on = 300,
  .tau_turn_on = 2000,
  .tau_turn_off = 30000,
};

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
  
  handlers[0].isEnabled = 1;
  handlers[1].isEnabled = 1;
}

void Trigger_Set_PowerOff(uint32_t ms)
{
  auto_off_tick = HAL_GetTick() + ms;
}

void Trigger_Callback(int index)
{
  trigger_handler *h = &handlers[index];
  if(h->isEnabled != 0){
    uint32_t tick = HAL_GetTick();
    Brigress_SetGradExpone(sett.brigress_on, sett.tau_turn_on);
    auto_off_tick = tick + sett.keep_time;
    h->trig_count++;
  }
}

void Trigger_Tick_Callback()
{
  if((auto_off_tick != 0) && (HAL_GetTick() >= auto_off_tick)){
    auto_off_tick = 0;
    Brigress_SetGradExpone(0, sett.tau_turn_off);
  }
}
