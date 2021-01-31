#include "st7735.h"
#include "stm32f0xx_hal.h"
#include "spi.h"

static void delay_ms(u32 ms)
{
  HAL_Delay(ms);
}

#define LCD_WIDTH 160

#define LCD_HIGH 128
#define LCD_DIR  1

#define MAX_SPI_BUF 100
typedef struct {

  uint8_t data[MAX_SPI_BUF];
}SPI_buf;

SPI_buf gBuf;

/*****************************************************************************
 ** ���ر���
 *****************************************************************************/
struct _LCD              // �ṹ�壺���ڱ�����Ϣ��״̬
{     
    u8  InitOK;
	u16 width;			 // LCD ���
	u16 height;			 // LCD �߶�
    
    u32 bColor;          // ����ɫ
}xLCD;



/*****************************************************************************
 ** ���غ���
 ****************************************************************************/
void sendOrder(u16 order);                                   // ��Һ�������ͣ��Ĵ�����ֵַ(ָ��), 8λ
void sendByte (u8  Data);                                    // ��Һ�������ͣ����ݣ�8λ
void sendShort(u16 Data);                                    // ��Һ�������ͣ����ݣ�16λ
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);  // ������ʾ����




/*****************************************************************************
 ** ��ʾ�����ƺ궨��, ��ҪΪ�˼��ļ��ڲ���
 *****************************************************************************/
// LCD_CS  
#define     LCD_CS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET)
#define     LCD_CS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET)
#define     LCD_RS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_RESET)
#define     LCD_RS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET)
#define     LCD_RES_LOW     HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET)
#define     LCD_RES_HIGH    HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET)
 	




// SPI���ʹ���1�ֽ�
void spiSendByte(u8 data)
{
    gBuf.data[0]= data;
    SPI_send_data(gBuf.data,1);
}



/*****************************************************************************
*��  ����LCD_Init
*��  �ܣ���ʼ��lcd
*��  ����Ϊ�˼���⡢������ֲ������(��Ļ���ء���ʾ����)��h�ļ�������޸�
*����ֵ���� 
*��  ע��
*****************************************************************************/
void LCD_Init(void)  
{ 	 
    xLCD.InitOK = 0; 
    xLCD.bColor  = BLACK;
	
	// ��Ļ��λ	
	LCD_RES_LOW ;	       // LCD_RST=0	  //SPI�ӿڸ�λ
	delay_ms(50);          // delay 20 ms 
    LCD_RES_HIGH  ;        // LCD_RST=1		
	delay_ms(50);

	sendOrder(0x11);       // �˳�˯��ģʽ
	delay_ms(120);         // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB1);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB2);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB3);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB4); // Dot inversion
	sendByte(0x03);
    
	sendOrder(0xC0); // ST7735R Power Sequence
	sendByte(0x28);
	sendByte(0x08);
	sendByte(0x04);
    
	sendOrder(0xC1);
	sendByte(0XC0);
    
	sendOrder(0xC2);
	sendByte(0x0D);
	sendByte(0x00);
	sendOrder(0xC3);
    
	sendByte(0x8D);
	sendByte(0x2A);
    
	sendOrder(0xC4);
	sendByte(0x8D);
	sendByte(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	sendOrder(0xC5); //VCOM
	sendByte(0x1A);
	sendOrder(0x36); //MX, MY, RGB mode
//	if(LCD_DIR == 1)  sendByte(0xC0);  // ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
//	if(LCD_DIR == 2)  sendByte(0x00);      
//    if(LCD_DIR == 3)  sendByte(0xA0);
//    if(LCD_DIR == 4)  sendByte(0x60);
    sendByte(0xc0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0xE0);
	sendByte(0x04);
	sendByte(0x22);
	sendByte(0x07);
	sendByte(0x0A);
	sendByte(0x2E);
	sendByte(0x30);
	sendByte(0x25);
	sendByte(0x2A);
	sendByte(0x28);
	sendByte(0x26);
	sendByte(0x2E);
	sendByte(0x3A);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x03);
	sendByte(0x13);
    
	sendOrder(0xE1);
	sendByte(0x04);
	sendByte(0x16);
	sendByte(0x06);
	sendByte(0x0D);
	sendByte(0x2D);
	sendByte(0x26);
	sendByte(0x23);
	sendByte(0x27);
	sendByte(0x27);
	sendByte(0x25);
	sendByte(0x2D);
	sendByte(0x3B);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x04);
	sendByte(0x13);
    
    // ���ڸ������ӵģ�������
    sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);
    // ���ڸ������ӵģ�������
	sendOrder(0x2b);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x9f);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0x3A); // 65k mode
	sendByte(0x05);
	
    sendOrder(0x29); // Display on

  LCD_Fill(0, 0, 128 , 160 , WHITE );
    xLCD.InitOK = 1; 	
}  
// ��LCD���ͼĴ�����ַ(ָ��)
void sendOrder(u16 order)
{
    while(1)
    {
      if(SPI_isReady())
      {
        HAL_Delay(1);
        break;
      }
    }
	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
    LCD_RS_LOW;            // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(order);    // ���� �Ĵ�����ֵַ
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 
}
// ��LCD������ֵ
void sendByte(u8 data) 
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 			 
}	
// ��LCD����2���ֽ�
void sendShort(u16 data)
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ���ݣ� RS��: ָ��
	spiSendByte(data>>8);  // ����1�ֽ�
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 	
}
				   
//д�Ĵ���: �Ĵ�����ַ������
void sendRegisterValue(u8 reg, u16 val)
{
	sendOrder(reg);
	sendShort(val);
}	 
	
/*****************************************************************
 * ��  ����LCD_Display
 * ��  �ܣ���ʾ������
 * ��  ����0:Ϩ����0����
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void LCD_Display(u8 sw)
{
//	if(sw==0)
//	    LCD_BL_LOW ;
//	else
//		LCD_BL_HIGH ;	
}
	
/*****************************************************************
 * ��  ����setCursor
 * ��  �ܣ�������ʾ�����ڴ�����д�������Զ�����
 * ��  ����xStart���������, yStart���������
 *         xEnd�����������㣬yEnd������������
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
	sendByte (xStart); // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
	sendByte (0x00);   // ��ʼλ��y��λ
	sendByte (xEnd);   // ��ʼλ��xλλ

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // ����д��������
}

/*****************************************************************
 * ��  ����drawPoint
 * ��  �ܣ���һ����
 * ��  ����x���꣬y����, 16λ��ɫֵ
 * ����ֵ����
 * 
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void drawPoint(u16 x, u16 y, u16 color)
{
    setCursor(x, y, x, y);      //���ù��λ�� 
    sendShort(color);    
}

/******************************************************************
 * �������� LCD_Fill
 * ��  �ܣ� ��ָ����������䵥����ɫ
 *          ��Ļ�����1��ʼ����Ļ���Һ��·�ʵ�����ж�������
 * ��  ���� u16 xStart, yStart  ���Ͻ���ʼ����
 *          u16 xEnd,   yEnd    ���½���ʼ����
 *          u32 Color           ������ɫ
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
 *****************************************************************/
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{     
	u32 pixel = (xEnd-xStart+1) * (yEnd-yStart+1);	// ������������
								   
	setCursor(xStart, yStart, xEnd, yEnd);      	// �趨��䷶Χ
    while(pixel-->0)                                // ������ɫֵ
       sendShort(color);                            
}  

/******************************************************************
 * �������� LCD_Line
 * ��  �ܣ� ����
 * ��  ���� u16 x1,y1  �������
 *          u16 x2,y2  �յ����� 
 *          u32 Color  ��ɫ
 *****************************************************************/
void LCD_Line(u16 sX, u16 sY, u16 eX, u16 eY, u16 color) 
{
	
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx,incy,uRow,uCol; 
	delta_x = eX - sX; //������������ 
	delta_y = eY - sY; 
	uRow = sX; 
	uCol = sY; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		drawPoint(uRow, uCol, color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
// ԭ�����ṩ�Ļ�Բ�������о��е�ʧ�棬����������������
void LCD_Circle(u16 x0,u16 y0,u8 r, u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                 //�ж��¸���λ�õı�־
	while(a<=b)
	{
		drawPoint(x0+a,y0-b, color);             //5
 		drawPoint(x0+b,y0-a, color);             //0           
		drawPoint(x0+b,y0+a, color);             //4               
		drawPoint(x0+a,y0+b, color);             //6 
		drawPoint(x0-a,y0+b, color);             //1       
 		drawPoint(x0-b,y0+a, color);             
		drawPoint(x0-a,y0-b, color);             //2             
  		drawPoint(x0-b,y0-a, color);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 
