/************************************************************************
 * ADC驱动文件
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
*************************************************************************/
#ifndef ADC_H
#define ADC_H

#define GPIO_PORT_ADC_CURR    GPIOA
#define LL_GPIO_PIN_ADC_CURR  LL_GPIO_PIN_6
#define LL_ADC_CHANNEL_CURR   LL_ADC_CHANNEL_6
#define GPIO_PORT_ADC_VOLT    GPIOA
#define LL_GPIO_PIN_ADC_VOLT  LL_GPIO_PIN_7
#define LL_ADC_CHANNEL_VOLT   LL_ADC_CHANNEL_7

#define LL_DMA_CHANNEL_ADC    LL_DMA_CHANNEL_3

#define ADC_BUFFSAMPS         (32)
#define ADC_BUFFSIZE          (2*32)

void ADC_Init();
void ADC_DMA_TC_Callback();
void ADC_DMA_HT_Callback();

#endif
