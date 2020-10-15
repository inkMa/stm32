#include <stdint.h>
#include "main.h"
#include "key.h"



//按键扫描信息
typedef struct {
    uint32_t key_state;//按键的状态
    uint32_t keep_time;//按键处于当前状态的时长
    uint32_t key_check;
    uint32_t key_value;

}key_scanInfo_t;


typedef struct 
{
    uint32_t head;
    uint32_t tail;
    key_scanInfo_t key_info[5];
}key_queue;


key_queue key_info_queue;





uint32_t get_key_value()
{
    uint32_t key_value = 0;
    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOE,KEY1_Pin))
    {
        key_value|=1;
    }
    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOE,KEY2_Pin))
    {
        key_value|=1<<1;
    }
    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOE,KEY3_Pin))
    {
        key_value|=1<<2;
    }
    if(GPIO_PIN_SET==HAL_GPIO_ReadPin(GPIOA,KEY_UP_Pin))
    {
        key_value|=1<<3;
    }
    return key_value;
}









//按键扫描，定时中断中
void key_scan(void)
{
//    uint32_t key_value = get_key_value();
    
    //消抖处理
    

}



uint32_t get_key()
{
    uint32_t key = 0;
    
    
    
    
    return key;
}