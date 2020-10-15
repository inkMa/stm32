#include "task.h"
#include "key.h"


void task_init()
{
  //³õÊ¼»¯Òý½Å×´Ì¬
  HAL_GPIO_WritePin(GPIOF, BEEP_Pin|LED1_Pin|LED2_Pin, GPIO_PIN_SET);
}


void task_exec()
{
  uint32_t key = get_key_value();
  if(key & 0x01)
  {
    HAL_GPIO_WritePin(GPIOF, LED1_Pin, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOF, LED1_Pin, GPIO_PIN_SET);
  }
  
  
  if(key & 0x02)
  {
    HAL_GPIO_WritePin(GPIOF, LED2_Pin, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOF, LED2_Pin, GPIO_PIN_SET);
  }
  
  HAL_Delay(1);
}