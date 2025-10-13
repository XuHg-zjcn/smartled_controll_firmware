#ifndef KEY_H
#define KEY_H

#define KEY_GPIO_PORT        GPIOF
#define KEY_LL_GPIO_PIN      LL_GPIO_PIN_4
#define KEY_LL_EXTI_CONFIG   LL_EXTI_CONFIG_LINE4
#define KEY_LL_EXTI_LINE     LL_EXTI_LINE_4

void Key_Init();
void Key_EXTI_Callback();

#endif
