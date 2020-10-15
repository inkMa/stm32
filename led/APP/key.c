#include <stdint.h>
#include "main.h"
#include "key.h"



//����ɨ����Ϣ
typedef struct {
    uint32_t key_state;//������״̬
    uint32_t keep_time;//�������ڵ�ǰ״̬��ʱ��
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









//����ɨ�裬��ʱ�ж���
void key_scan(void)
{
//    uint32_t key_value = get_key_value();
    
    //��������
    

}



uint32_t get_key()
{
    uint32_t key = 0;
    
    
    
    
    return key;
}