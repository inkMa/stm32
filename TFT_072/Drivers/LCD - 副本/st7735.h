#ifndef __LCD_INIT_H
#define __LCD_INIT_H


#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif
	
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

//画笔颜色
#define WHITE           0xFFFF
#define BLACK           0x0000	  
#define BLUE            0x001F  
#define BRED            0XF81F
#define GRED            0XFFE0
#define GBLUE           0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN           0x07E0
#define CYAN            0x7FFF
#define YELLOW          0xFFE0
#define BROWN           0XBC40 //棕色
#define BRRED           0XFC07 //棕红色
#define GRAY            0X8430 //灰色
#define DARKBLUE        0X01CF	//深蓝色
#define LIGHTBLUE       0X7D7C	//浅蓝色  
#define GRAYBLUE        0X5458 //灰蓝色
#define LIGHTGREEN      0X841F //浅绿色
#define LGRAY           0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



void LCD_Init(void);//
void LCD_draw_point(u16 xsta,u16 ysta,u16 color);
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color);
#endif




