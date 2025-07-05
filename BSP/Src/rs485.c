/************************************************************************
 * RS485驱动文件
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
#include "rs485.h"
#include "py32f0xx_hal.h"


#define TR_GPIO_PIN    GPIO_PIN_5
#define TR_GPIO_PORT   GPIOB

UART_HandleTypeDef huart;

void RS485_Init()
{
  //TXD和RXD脚已在HAL_UART_MspInit函数中初始化
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(TR_GPIO_PORT, TR_GPIO_PIN, GPIO_PIN_RESET); //先设置电平，防止毛刺
  GPIO_InitStruct.Pin       = TR_GPIO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TR_GPIO_PORT, &GPIO_InitStruct);

  huart.Instance = USART1;
  huart.Init.BaudRate     = 500000;
  huart.Init.WordLength   = UART_WORDLENGTH_8B;
  huart.Init.StopBits     = UART_STOPBITS_1;
  huart.Init.Parity       = UART_PARITY_NONE;
  huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart.Init.Mode         = UART_MODE_TX_RX;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart);
}

void RS485_Send(uint8_t *p, uint16_t size)
{
  HAL_GPIO_WritePin(TR_GPIO_PORT, TR_GPIO_PIN, GPIO_PIN_SET);
  //TODO: 消除因HAL库耗时在首尾多开启发送的一段时间，会造成电容耦合后明显基线漂移
  HAL_UART_Transmit_DMA(&huart, p, size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_GPIO_WritePin(TR_GPIO_PORT, TR_GPIO_PIN, GPIO_PIN_RESET);
}
