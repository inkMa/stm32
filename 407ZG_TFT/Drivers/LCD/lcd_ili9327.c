#include "lcd_ili9327.h"
#include "stm32f4xx_hal.h"

#define     LCD_CS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET)
#define     LCD_CS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET)
#define     LCD_RS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_RESET)
#define     LCD_RS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET)
#define     LCD_RES_LOW     HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET)
#define     LCD_RES_HIGH    HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET)
#define LCD_WIDTH 400
#define LCD_HIGH 240



#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
//#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
typedef struct
{
  uint16_t cmd;
  uint16_t data;
}LCD_type;

LCD_type *LCD_TFT = (LCD_type*)TFTLCD_BASE;



//—”≥Ÿ∫Ø ˝
static void LCD_delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}



//–¥√¸¡Ó
void LCD_writeCmd(uint16_t data)
{
  LCD_TFT->cmd = data;
}

//–¥ ˝æ›
void LCD_writeData(uint16_t data)
{
  LCD_TFT->data = data;
}

void LCD_writeData16Bit(uint16_t data)
{
  LCD_writeData( (data>>8)&0xFF);
  LCD_writeData( data&0xFF);
}

//–¥◊¯±Í
void LCD_setXY(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
{
  LCD_writeCmd(0x2A);
  LCD_writeData16Bit(x0);
  LCD_writeData16Bit(x1);
  LCD_writeCmd(0x2B);
  LCD_writeData16Bit(y0);
  LCD_writeData16Bit(y1);
  LCD_writeCmd(0x2C);
}

//ª≠µ„
void LCD_drawPoint(uint16_t x0,uint16_t y0,uint16_t color)
{
  LCD_setXY(x0,y0,x0,y0);
  LCD_writeData16Bit(color);
}

//ª≠œﬂ
void LCD_drawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color)
{
  
}

//ÃÓ≥‰æÿ–Œ
void LCD_fillRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color)
{
  LCD_setXY(x0,y0,x1,y1);
  
  uint32_t pixel = (x1-x0+1) * (y1-y0+1);

  while(pixel-->0)
  {
    LCD_writeData16Bit(color);
  }
}
//ª≠æÿ–Œ«¯”Ú
void LCD_drawRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t* pPixels)
{
  LCD_setXY(x0,y0,x1,y1);
  
  uint32_t pixel = (x1-x0+1) * (y1-y0+1);
  while(pixel-->0)
  {
    LCD_writeData16Bit(*pPixels++);
  }
}



//∆¡ƒª≥ı ºªØ
void LCD_Init(void)
{
  LCD_writeCmd(0xEF);
  uint16_t id = LCD_TFT->data;	//dummy read 
  id=LCD_TFT->data;  //02
  id=LCD_TFT->data;  //04
  id=LCD_TFT->data;  //93
  id=(id<<8)|LCD_TFT->data;  //27
  id=LCD_TFT->data;  //93
  
  
  LCD_writeCmd(0xE9); 
	LCD_writeData(0x20); 
	
	LCD_writeCmd(0x11); //Exit Sleep 
	LCD_delay_ms(100); 
	
	LCD_writeCmd(0x3A);
	LCD_writeData(0x55);
	
	LCD_writeCmd(0xD1); 
	LCD_writeData(0x00); 
	LCD_writeData(0x65); //
	LCD_writeData(0x1F); 
	
	LCD_writeCmd(0xD0); 
	LCD_writeData(0x07); 
	LCD_writeData(0x07); 
	LCD_writeData(0x80); 
	
	LCD_writeCmd(0x36); 
	LCD_writeData(0x48); 
	
	LCD_writeCmd(0xC1); 
	LCD_writeData(0x10); 
	LCD_writeData(0x10); 
	LCD_writeData(0x02); 
	LCD_writeData(0x02); 
	
	LCD_writeCmd(0xC0); //Set Default Gamma 
	LCD_writeData(0x00); 
	LCD_writeData(0x35); 
	LCD_writeData(0x00); 
	LCD_writeData(0x00); 
	LCD_writeData(0x01); 
	LCD_writeData(0x02); 
	
	LCD_writeCmd(0xC5); //Set frame rate 
	LCD_writeData(0x01); 
	
	LCD_writeCmd(0xD2); //power setting 
	LCD_writeData(0x01); 
	LCD_writeData(0x22); 
	
	LCD_writeCmd(0xC8); //Set Gamma 
	LCD_writeData(0x01); 
	LCD_writeData(0x52); 
	LCD_writeData(0x37); 
	LCD_writeData(0x10); 
	LCD_writeData(0x0d); 
	LCD_writeData(0x01); 
	LCD_writeData(0x04); 
	LCD_writeData(0x51); 
	LCD_writeData(0x77); 
	LCD_writeData(0x01); 
	LCD_writeData(0x01); 
	LCD_writeData(0x0d); 
	LCD_writeData(0x08); 
	LCD_writeData(0x80); 
	LCD_writeData(0x00); 
	
    LCD_writeCmd(0x36);
    LCD_writeData(0x28);//28 ∫·∆¡ 48 ˙∆¡ e8∫·∆¡µﬂµπ
    
	LCD_writeCmd(0x29); //display on
  
  //LCD_fillRect(0,0,239,399,BLACK);
}

