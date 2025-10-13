#include "key.h"
#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_gpio.h"
#include "py32f0xx_ll_exti.h"
#include "led.h"


uint32_t prev_tick = 0;
int prev_stat = 0;
const uint32_t preset_pwm[] = {18, 40, 90, 202, 455, 1024}; /*round(1024/2.25**(5-i))*/
int preset_dire = 1;

void Key_Init()
{
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_GPIO_SetPinMode(KEY_GPIO_PORT, KEY_LL_GPIO_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(KEY_GPIO_PORT, KEY_LL_GPIO_PIN, LL_GPIO_PULL_DOWN);
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, KEY_LL_EXTI_CONFIG);
  LL_EXTI_EnableFallingTrig(KEY_LL_EXTI_LINE);
  LL_EXTI_EnableRisingTrig(KEY_LL_EXTI_LINE);
  LL_EXTI_EnableIT(KEY_LL_EXTI_LINE);
  NVIC_SetPriority(EXTI4_15_IRQn, 3);
  NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void Key_setPWM()
{
  uint32_t old_pwm = LED_GetOutputCompare(LED2);
  int preset_i = 0;
  int preset_i_new;
  while(preset_pwm[preset_i] < old_pwm){
    preset_i++;
  }
  if(preset_i == 0){
    preset_dire = 1;
    preset_i_new = 1;
  }
  if(preset_i >= sizeof(preset_pwm)/sizeof(preset_pwm[0])-1){
    preset_dire = -1;
    preset_i_new = sizeof(preset_pwm)/sizeof(preset_pwm[0])-2;
  }else{
    preset_i_new = preset_i + preset_dire;
  }
  uint32_t new_pwm = preset_pwm[preset_i_new];
  LED_SetOutputCompare(LED2, new_pwm);
}

void Key_EXTI_Callback()
{
  int stat = LL_GPIO_IsInputPinSet(KEY_GPIO_PORT, KEY_LL_GPIO_PIN);
  uint32_t now = HAL_GetTick();
  uint32_t delta = now - prev_tick;
  if(delta>20 && stat==0 && prev_stat!=0){
    //刚才按下,现在弹起了
    if(delta < 500){
      if(LED_GetOutputEnable(LED2) & LED2){
        Key_setPWM();
      }else{
        LED_SetOutputEnable(LED2, 1);
      }
    }else{
      LED_SetOutputEnable(LED2, 0);
    }
  }
  prev_stat = stat;
  prev_tick = now;
}
