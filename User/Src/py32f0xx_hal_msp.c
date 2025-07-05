/**
  ******************************************************************************
  * @file    py32f0xx_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RXD_GPIO_PIN   GPIO_PIN_7
#define RXD_GPIO_PORT  GPIOB
#define TXD_GPIO_PIN   GPIO_PIN_14
#define TXD_GPIO_PORT  GPIOA
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief Initialize global MSP
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  //参考代码:
  // STM32CubeF1/Projects/STM32F103RB-Nucleo/Examples/UART/UART_TwoBoards_ComDMA/Src/stm32f1xx_hal_msp.c
  // PY32F0xx_Firmware/Projects/PY32F003-STK/Example/USART/USART_HyperTerminal_DMA/Src/py32f0xx_hal_msp.c
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  //开启时钟
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_DMA_CLK_ENABLE();

  //初始化RXD和TXD GPIO
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

  //初始化UART发送的DMA通道
  hdma_tx.Instance                 = DMA1_Channel1;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_MEDIUM;
  HAL_DMA_Init(&hdma_tx);
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  //初始化UART接收的DMA通道
  hdma_rx.Instance                 = DMA1_Channel2;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_rx);
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);

  //配置DMA请求映射
  /*UART1_TX DMA_CH1-0x5  ; UART1_RX DMA_CH2-0x6*/
  HAL_SYSCFG_DMA_Req(0x0605);

  //开启中断
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);   //DMA UART1 TX
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0); //DMA UART1 RX
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
