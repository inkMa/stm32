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
 ** 本地变量
 *****************************************************************************/
struct _LCD              // 结构体：用于保存信息、状态
{     
    u8  InitOK;
	u16 width;			 // LCD 宽度
	u16 height;			 // LCD 高度
    
    u32 bColor;          // 背景色
}xLCD;



/*****************************************************************************
 ** 本地函数
 ****************************************************************************/
void sendOrder(u16 order);                                   // 向液晶屏发送：寄存器地址值(指令), 8位
void sendByte (u8  Data);                                    // 向液晶屏发送：数据，8位
void sendShort(u16 Data);                                    // 向液晶屏发送：数据，16位
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);  // 设置显示区域




/*****************************************************************************
 ** 显示屏控制宏定义, 主要为了简化文件内操作
 *****************************************************************************/
// LCD_CS  
#define     LCD_CS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_RESET)
#define     LCD_CS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, GPIO_PIN_SET)
#define     LCD_RS_LOW      HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_RESET)
#define     LCD_RS_HIGH     HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET)
#define     LCD_RES_LOW     HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET)
#define     LCD_RES_HIGH    HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET)
 	




// SPI发送传送1字节
void spiSendByte(u8 data)
{
    gBuf.data[0]= data;
    SPI_send_data(gBuf.data,1);
}



/*****************************************************************************
*函  数：LCD_Init
*功  能：初始化lcd
*参  数：为了简化理解、便于移植，参数(屏幕像素、显示方向)在h文件里进行修改
*返回值：无 
*备  注：
*****************************************************************************/
void LCD_Init(void)  
{ 	 
    xLCD.InitOK = 0; 
    xLCD.bColor  = BLACK;
	
	// 屏幕复位	
	LCD_RES_LOW ;	       // LCD_RST=0	  //SPI接口复位
	delay_ms(50);          // delay 20 ms 
    LCD_RES_HIGH  ;        // LCD_RST=1		
	delay_ms(50);

	sendOrder(0x11);       // 退出睡眠模式
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
//	if(LCD_DIR == 1)  sendByte(0xC0);  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
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
    
    // 后期复制增加的，不明白
    sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);
    // 后期复制增加的，不明白
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
// 向LCD发送寄存器地址(指令)
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
	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_LOW;            // RS高: 数据值， RS低: 寄存器地址值
	spiSendByte(order);    // 发送 寄存器地址值
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 
}
// 向LCD发送数值
void sendByte(u8 data) 
{
 	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
	LCD_RS_HIGH;	       // RS高: 数据值， RS低: 寄存器地址值
	spiSendByte(data);     // 发送1字节
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 			 
}	
// 向LCD发送2个字节
void sendShort(u16 data)
{
 	LCD_CS_LOW;            // SPI设备片选拉低，开始通信
	LCD_RS_HIGH;	       // RS高: 数据， RS低: 指令
	spiSendByte(data>>8);  // 发送1字节
	spiSendByte(data);     // 发送1字节
	LCD_CS_HIGH;           // SPI设备片选拉高，结束通信 	
}
				   
//写寄存器: 寄存器地址，数据
void sendRegisterValue(u8 reg, u16 val)
{
	sendOrder(reg);
	sendShort(val);
}	 
	
/*****************************************************************
 * 函  数：LCD_Display
 * 功  能：显示屏开关
 * 参  数：0:熄，非0：开
 * 返回值：无
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void LCD_Display(u8 sw)
{
//	if(sw==0)
//	    LCD_BL_LOW ;
//	else
//		LCD_BL_HIGH ;	
}
	
/*****************************************************************
 * 函  数：setCursor
 * 功  能：设置显示区域，在此区域写点数据自动换行
 * 参  数：xStart横坐标起点, yStart纵坐标起点
 *         xEnd横坐标线束点，yEnd纵坐标线束点
 * 返回值：无
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
	sendByte (xStart); // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
	sendByte (0x00);   // 起始位置y高位
	sendByte (xEnd);   // 起始位置x位位

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // 发送写数据命令
}

/*****************************************************************
 * 函  数：drawPoint
 * 功  能：画一个点
 * 参  数：x坐标，y坐标, 16位颜色值
 * 返回值：无
 * 
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
******************************************************************/
void drawPoint(u16 x, u16 y, u16 color)
{
    setCursor(x, y, x, y);      //设置光标位置 
    sendShort(color);    
}

/******************************************************************
 * 函数名： LCD_Fill
 * 功  能： 在指定区域内填充单个颜色
 *          屏幕坐标从1开始；屏幕左右和下方实际上有多余行列
 * 参  数： u16 xStart, yStart  左上角起始坐标
 *          u16 xEnd,   yEnd    右下角起始坐标
 *          u32 Color           填充的颜色
 *
 * 备  注： 魔女开发板团队编写   淘宝 https://demoboard.taobao.com
 * 分  享： 不断更新于Q群文件夹：262901124  最后修改_2020年09月01日
 *****************************************************************/
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{     
	u32 pixel = (xEnd-xStart+1) * (yEnd-yStart+1);	// 填充的像素数量
								   
	setCursor(xStart, yStart, xEnd, yEnd);      	// 设定填充范围
    while(pixel-->0)                                // 发送颜色值
       sendShort(color);                            
}  

/******************************************************************
 * 函数名： LCD_Line
 * 功  能： 画线
 * 参  数： u16 x1,y1  起点坐标
 *          u16 x2,y2  终点坐标 
 *          u32 Color  颜色
 *****************************************************************/
void LCD_Line(u16 sX, u16 sY, u16 eX, u16 eY, u16 color) 
{
	
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx,incy,uRow,uCol; 
	delta_x = eX - sX; //计算坐标增量 
	delta_y = eY - sY; 
	uRow = sX; 
	uCol = sY; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		drawPoint(uRow, uCol, color);//画点 
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

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
// 原厂商提供的画圆函数，感觉有点失真，后面再慢慢搞它吧
void LCD_Circle(u16 x0,u16 y0,u8 r, u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                 //判断下个点位置的标志
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
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 
