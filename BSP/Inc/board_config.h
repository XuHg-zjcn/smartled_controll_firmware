/************************************************************************
 * PCB相关的配置文件
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
#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/************************************************************************
 * 说明:
 * 本文件含有GPIO引脚等配置，通过修改本文件来适配不同的PCB
 * 本文件中未列出可选的值，需要查看数据手册等资料确定
 * 部分配置修改可能需要同时修改一些相关代码才能正常运行
 ************************************************************************/

/******************/
/*     LED相关    */
/******************/
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

#define LED(x) (x-1)
#define LED1  LED(1)
#define LED2  LED(2)
#define LED3  LED(3)
#define LED4  LED(4)
#define LED_COUNT (4)
#define LED_MAIN  LED1


/******************/
/*    RS485相关   */
/******************/
#define TR_LL_GPIO_PIN    LL_GPIO_PIN_5
#define TR_GPIO_PORT      GPIOB

#define RXD_LL_GPIO_PIN   LL_GPIO_PIN_7
#define RXD_GPIO_PORT     GPIOB
#define RXD_LL_GPIO_AF    LL_GPIO_AF_0

#define TXD_LL_GPIO_PIN   LL_GPIO_PIN_14
#define TXD_GPIO_PORT     GPIOA
#define TXD_LL_GPIO_AF    LL_GPIO_AF_1

//编码后的RS485地址
#define RS485_ADDR   (0x01)
#define RS485_ADDR1  (0xA9)
#define RS485_ADDR2  (0xAA)


/******************/
/*     ADC相关    */
/******************/
#define GPIO_PORT_ADC_CURR    GPIOA
#define LL_GPIO_PIN_ADC_CURR  LL_GPIO_PIN_6
#define LL_ADC_CHANNEL_CURR   LL_ADC_CHANNEL_6
#define GPIO_PORT_ADC_VOLT    GPIOA
#define LL_GPIO_PIN_ADC_VOLT  LL_GPIO_PIN_7
#define LL_ADC_CHANNEL_VOLT   LL_ADC_CHANNEL_7


/******************/
/*    按键相关    */
/******************/
#define KEY_GPIO_PORT        GPIOF
#define KEY_LL_GPIO_PIN      LL_GPIO_PIN_4
#define KEY_LL_EXTI_CONFIG   LL_EXTI_CONFIG_LINE4
#define KEY_LL_EXTI_LINE     LL_EXTI_LINE_4


/******************/
/*    触发相关    */
/******************/
#define N_TRIGGER 2

#define TRIG1_GPIO_PORT       GPIOF
#define TRIG1_LL_GPIO_PIN     LL_GPIO_PIN_0
#define TRIG1_LL_EXTI_CONFIG  LL_EXTI_CONFIG_LINE0
#define TRIG1_LL_EXTI_LINE    LL_EXTI_LINE_0

#define TRIG2_GPIO_PORT       GPIOF
#define TRIG2_LL_GPIO_PIN     LL_GPIO_PIN_1
#define TRIG2_LL_EXTI_CONFIG  LL_EXTI_CONFIG_LINE1
#define TRIG2_LL_EXTI_LINE    LL_EXTI_LINE_1

#endif
