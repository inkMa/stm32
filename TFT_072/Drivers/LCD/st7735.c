#include "st7735.h"
#include "stm32f0xx_hal.h"
#include "spi.h"

#define     LCD_CS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET)
#define     LCD_CS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET)
#define     LCD_RS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_RESET)
#define     LCD_RS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET)
#define     LCD_RES_LOW     HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET)
#define     LCD_RES_HIGH    HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET)
#define LCD_WIDTH 160
#define LCD_HIGH 128

//���ݻ��������壬����ram�Ĵ�С���������е���
#define MAX_SPI_BUF 100
typedef struct {
  uint8_t data[MAX_SPI_BUF];
}SPI_buf;

SPI_buf gBuf;



//�ӳٺ���
static void LCD_delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}


//ͨ��SPI��tft��������
void SPI_writeByte(uint8_t data)
{
  uint8_t sendByte = data;
  SPI_send_data(&sendByte,1);
}

//д����
void LCD_writeCmd(uint8_t data)
{
  LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
  LCD_RS_LOW;            // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
  SPI_writeByte(data);   // ���� �Ĵ�����ֵַ
  LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 
}

//д����
void LCD_writeData(uint8_t data)
{
  LCD_CS_LOW;           // SPI�豸Ƭѡ���ͣ���ʼͨ��
  LCD_RS_HIGH;          // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
  SPI_writeByte(data);  // ����1�ֽ�
  LCD_CS_HIGH;          // SPI�豸Ƭѡ���ߣ�����ͨ��
}

//д����16bit
void LCD_writeData16Bit(uint16_t data)
{
  LCD_CS_LOW;                       // SPI�豸Ƭѡ���ͣ���ʼͨ��
  LCD_RS_HIGH;                      // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
  SPI_writeByte((data>>8)&0xFF);    // ����1�ֽ�
  SPI_writeByte(data&0xFF);         // ����1�ֽ�
  LCD_CS_HIGH;                      // SPI�豸Ƭѡ���ߣ�����ͨ��
}

//д����
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

//����
void LCD_drawPoint(uint16_t x0,uint16_t y0,uint16_t color)
{
  LCD_setXY(x0,y0,x0,y0);
  LCD_writeData16Bit(color);
}

//����
void LCD_drawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color)
{
  
}

//������
void LCD_fillRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color)
{
  LCD_setXY(x0,y0,x1,y1);
  
  uint16_t pixel = (x1-x0+1) * (y1-y0+1);

  while(pixel-->0)
  {
    LCD_writeData16Bit(color);
  }
}
//����������
void LCD_drawRect(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t* pPixels)
{
  LCD_setXY(x0,y0,x1,y1);
  
  uint16_t pixel = (x1-x0+1) * (y1-y0+1);
  while(pixel-->0)
  {
    LCD_writeData16Bit(*pPixels++);
  }
}



//��Ļ��ʼ��
void LCD_Init(void)
{
  LCD_RES_LOW ;             // LCD_RST=0	  //SPI�ӿڸ�λ
  LCD_delay_ms(50);
  LCD_RES_HIGH  ;
  LCD_delay_ms(50);

  LCD_writeCmd(0x11);       // �˳�˯��ģʽ
  LCD_delay_ms(120);      

  LCD_writeCmd(0xB1);
  LCD_writeData (0x05);
  LCD_writeData (0x3C);
  LCD_writeData (0x3C);
  
  LCD_writeCmd(0xB2);
  LCD_writeData (0x05);
  LCD_writeData (0x3C);
  LCD_writeData (0x3C);
  
  LCD_writeCmd(0xB3);
  LCD_writeData (0x05);
  LCD_writeData (0x3C);
  LCD_writeData (0x3C);
  LCD_writeData (0x05);
  LCD_writeData (0x3C);
  LCD_writeData (0x3C);

  LCD_writeCmd(0xB4);
  LCD_writeData(0x03);
  
  LCD_writeCmd(0xC0);
  LCD_writeData(0x28);
  LCD_writeData(0x08);
  LCD_writeData(0x04);
  
  LCD_writeCmd(0xC1);
  LCD_writeData(0XC0);
  
  LCD_writeCmd(0xC2);
  LCD_writeData(0x0D);
  LCD_writeData(0x00);
  LCD_writeCmd(0xC3);
  
  LCD_writeData(0x8D);
  LCD_writeData(0x2A);
  
  LCD_writeCmd(0xC4);
  LCD_writeData(0x8D);
  LCD_writeData(0xEE);

  LCD_writeCmd(0xC5);
  LCD_writeData(0x1A);
  
  LCD_writeCmd(0x36);
  LCD_writeData(0xc0);// ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
  
  LCD_writeCmd(0xE0);
  LCD_writeData(0x04);
  LCD_writeData(0x22);
  LCD_writeData(0x07);
  LCD_writeData(0x0A);
  LCD_writeData(0x2E);
  LCD_writeData(0x30);
  LCD_writeData(0x25);
  LCD_writeData(0x2A);
  LCD_writeData(0x28);
  LCD_writeData(0x26);
  LCD_writeData(0x2E);
  LCD_writeData(0x3A);
  LCD_writeData(0x00);
  LCD_writeData(0x01);
  LCD_writeData(0x03);
  LCD_writeData(0x13);
    
  LCD_writeCmd(0xE1);
  LCD_writeData(0x04);
  LCD_writeData(0x16);
  LCD_writeData(0x06);
  LCD_writeData(0x0D);
  LCD_writeData(0x2D);
  LCD_writeData(0x26);
  LCD_writeData(0x23);
  LCD_writeData(0x27);
  LCD_writeData(0x27);
  LCD_writeData(0x25);
  LCD_writeData(0x2D);
  LCD_writeData(0x3B);
  LCD_writeData(0x00);
  LCD_writeData(0x01);
  LCD_writeData(0x04);
  LCD_writeData(0x13);
    

  LCD_writeCmd(0x2a);
  LCD_writeData(0x00);
  LCD_writeData(0x00);
  LCD_writeData(0x00);
  LCD_writeData(0x7f);

  LCD_writeCmd(0x2b);
  LCD_writeData(0x00);
  LCD_writeData(0x00);
  LCD_writeData(0x00);
  LCD_writeData(0x9f);

  LCD_writeCmd(0x3A); // 65k mode
  LCD_writeData(0x05);
  
  LCD_writeCmd(0x29); // Display on
  
  LCD_fillRect(0,0,128,160,RED);
}

