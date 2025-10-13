/************************************************************************
 * LED智能控制器BSP包LED驱动文件
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
#include "led.h"
#include "py32f0xx_ll_tim.h"
#include "py32f0xx.h"

//此处LEDx编号是PCB上印刷的粗体数字
#define LED1_GPIO_PORT	GPIOA
#define LED1_GPIO_PIN	GPIO_PIN_0
#define LED2_GPIO_PORT	GPIOA
#define LED2_GPIO_PIN   GPIO_PIN_13
#define LED3_GPIO_PORT	GPIOA
#define LED3_GPIO_PIN	GPIO_PIN_3
#define LED4_GPIO_PORT	GPIOA
#define LED4_GPIO_PIN	GPIO_PIN_1

#define LED1_TIM_CHANNEL  TIM_CHANNEL_3
#define LED2_TIM_CHANNEL  TIM_CHANNEL_2
#define LED3_TIM_CHANNEL  TIM_CHANNEL_1
#define LED4_TIM_CHANNEL  TIM_CHANNEL_4

#define TIM_CHANNEL_HAL_TO_LL(x)  (1<<(x))
#define LEDx_TO_LL_TIM_CHANNEL(x) (TIM_CHANNEL_HAL_TO_LL(map_led_to_channel[x]))
#define LEDx_TO_HAL_TIM_CHANNEL(x) (map_led_to_channel[x])

uint32_t map_led_to_channel[4] = {
  LED1_TIM_CHANNEL,
  LED2_TIM_CHANNEL,
  LED3_TIM_CHANNEL,
  LED4_TIM_CHANNEL
};

#define PERIOD          (1024)
#define PULSE_DEFAULT   (0)

TIM_HandleTypeDef   htim_led;
TIM_OC_InitTypeDef  sPWMConfig = {0};

//TODO: 此文件全部代码改为LL库
void LED_Init()
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;              /* 推免输出 */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                      /* 不上下拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;            /* GPIO速度 */
    GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;

    GPIO_InitStruct.Pin = LED1_GPIO_PIN;                     /* LED1: PA0  TIM1_CH3 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED2_GPIO_PIN;                     /* LED2: PA13 TIM1_CH2 */
    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED3_GPIO_PIN;                     /* LED3: PA3  TIM1_CH1 */
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED4_GPIO_PIN;                     /* LED4: PA1  TIM1_CH4 */
    HAL_GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStruct);

    htim_led.Instance = TIM1;
    htim_led.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;/* 时钟不分频 */
    htim_led.Init.Prescaler         = 1 - 1;                 /* 不预分频 */
    htim_led.Init.Period            = PERIOD - 1;            /* 计数器周期 */
    htim_led.Init.CounterMode       = TIM_COUNTERMODE_UP;    /* 计数器模式 */
    htim_led.Init.RepetitionCounter = 1 - 1;                 /* 不重复计数 */
    htim_led.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* 关闭预载 */
    HAL_TIM_PWM_Init(&htim_led);

    sPWMConfig.OCMode       = TIM_OCMODE_PWM1;               /* 配置为PWM1模式 */
    sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;           /* 高电平有效 */
    sPWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;          /* 高电平有效(此摄影不使用) */
    sPWMConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;         /* OCx空闲输出状态(此设计不使用) */
    sPWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;        /* OCxN空闲输出状态(此设计不使用) */
    sPWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;            /* 关闭快速模式 */
    sPWMConfig.Pulse = PULSE_DEFAULT;                        /* 默认占空比 */

    HAL_TIM_PWM_ConfigChannel(&htim_led, &sPWMConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim_led, &sPWMConfig, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim_led, &sPWMConfig, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim_led, &sPWMConfig, TIM_CHANNEL_4);
}

void LED_SetOutputEnable(int LEDx, int isEnable)
{
  if(LEDx >= LED_COUNT){
    return;
  }
  if(isEnable){
    LL_TIM_OC_SetMode(TIM1, LEDx_TO_LL_TIM_CHANNEL(LEDx), LL_TIM_OCMODE_PWM1);
    HAL_TIM_PWM_Start(&htim_led, LEDx_TO_HAL_TIM_CHANNEL(LEDx));
  }else{
    LL_TIM_OC_SetMode(TIM1, LEDx_TO_LL_TIM_CHANNEL(LEDx), LL_TIM_OCMODE_INACTIVE);
  }
}


int LED_GetOutputEnable(int LEDx)
{
  uint32_t mode = LL_TIM_OC_GetMode(TIM1, LEDx_TO_LL_TIM_CHANNEL(LEDx));
  if(mode == LL_TIM_OCMODE_PWM1){
    return 1;
  }else{
    return 0;
  }
}


void LED_SetOutputCompare(int LEDx, uint16_t compare)
{
  if(LEDx >= LED_COUNT){
    return;
  }
  __HAL_TIM_SET_COMPARE(&htim_led, LEDx_TO_HAL_TIM_CHANNEL(LEDx), compare);
}

uint32_t LED_GetOutputCompare(int LEDx)
{
  if(LEDx >= LED_COUNT){
    return 0;
  }
  return __HAL_TIM_GET_COMPARE(&htim_led, LEDx_TO_HAL_TIM_CHANNEL(LEDx));
}

void LED_StopAll()
{
  HAL_TIM_PWM_Stop(&htim_led, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim_led, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim_led, TIM_CHANNEL_3);
  HAL_TIM_PWM_Stop(&htim_led, TIM_CHANNEL_4);
}
