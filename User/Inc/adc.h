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
#include "board_config.h"

#define LL_DMA_CHANNEL_ADC    LL_DMA_CHANNEL_3

#define ADC_BUFFSAMPS         (32)
#define ADC_BUFFSIZE          (2*32)

void ADC_Init();
void ADC_DMA_TC_Callback();
void ADC_DMA_HT_Callback();

#endif
