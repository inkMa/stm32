#include "task.h"
#include "st7735.h"
#include "stdint.h"


uint16_t array[500] = {0};

void task_init()
{
  LCD_Init();
  LCD_fillRect(0,0,20,20,RED);
  for(int i = 0;i<500;i++)
  {
    array[i] = GREEN;
  }
}


void task_exec()
{
  LCD_drawRect(10,50,29,70,array);
  //static int i = 0;
  //i++;
  //LCD_drawPoint(i,i,RED);
}