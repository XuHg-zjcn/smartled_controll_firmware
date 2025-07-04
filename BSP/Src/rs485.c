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


#define RXD_GPIO_PIN   GPIO_PIN_7
#define RXD_GPIO_PORT  GPIOB
#define TXD_GPIO_PIN   GPIO_PIN_14
#define TXD_GPIO_PORT  GPIOA
#define TR_GPIO_PIN    GPIO_PIN_5
#define TR_GPIO_PORT   GPIOB
#define UART_TIMEOUT   (10)

UART_HandleTypeDef huart;

void RS485_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();

  HAL_GPIO_WritePin(TR_GPIO_PORT, TR_GPIO_PIN, GPIO_PIN_RESET); //先设置电平，防止毛刺
  GPIO_InitStruct.Pin       = TR_GPIO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TR_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = RXD_GPIO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
  HAL_GPIO_Init(RXD_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = TXD_GPIO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(TXD_GPIO_PORT, &GPIO_InitStruct);

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
  HAL_UART_Transmit(&huart, p, size, UART_TIMEOUT);
  HAL_GPIO_WritePin(TR_GPIO_PORT, TR_GPIO_PIN, GPIO_PIN_RESET);
}
