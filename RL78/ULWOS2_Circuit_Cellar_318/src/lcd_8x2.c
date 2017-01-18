/*
 * LCD.C - RL78 LCD library
 *
 * Author: Fábio Pereira
 */

#include "iodefine.h"
#include "lcd_8x2.h"

void LCD_delay_ms(unsigned char time)
{
  unsigned int temp;
  for (;time;time--) for (temp=CPU_CLOCK/30;temp;temp--) asm("NOP");
}

void LCD_send4(char data)
{
#ifdef PIN_LCD_RW
	PIN_LCD_RW = 0;
#endif
  if (data & 0x08) PIN_LCD_D7=1; else PIN_LCD_D7=0 ;
  if (data & 0x04) PIN_LCD_D6=1; else PIN_LCD_D6=0 ;
  if (data & 0x02) PIN_LCD_D5=1; else PIN_LCD_D5=0 ;
  if (data & 0x01) PIN_LCD_D4=1; else PIN_LCD_D4=0 ;
  // pulse enable line
  PIN_LCD_EN = 1;
  PIN_LCD_EN = 0;
}

void LCD_send8(char addr, char data )
{
  // configure rs line according to the selected address
  PIN_LCD_RS = addr;
  LCD_delay_ms(1);
  // send first nibble
  LCD_send4(data >> 4);
  // send second nibble
  LCD_send4(data & 0x0f);
}

void LCD_init(char md1, char md2 )
{
  unsigned char temp;
  // place pins in low logic level
  //PORT_LCD_DATA4 = 0;
  PIN_LCD_D7 = 0;
  PIN_LCD_D6 = 0;
  PIN_LCD_D5 = 0;
  PIN_LCD_D4 = 0;
  PIN_LCD_RS = 0;
  PIN_LCD_EN = 0;
  // configure them as outputs
  PIN_LCD_EN_DIR = 0;
  PIN_LCD_RS_DIR = 0;
#ifdef PIN_LCD_RW_DIR
  PIN_LCD_RW_DIR = 0;
  PIN_LCD_RW = 0;
#endif
  //PORT_LCD_DATA4_DIR = 0xF0;
  PIN_LCD_D7_DIR = 0;
  PIN_LCD_D6_DIR = 0;
  PIN_LCD_D5_DIR = 0;
  PIN_LCD_D4_DIR = 0;
  LCD_delay_ms(15);
  // send LCD init sequence (3 times 0x03 and then 0x02 to place the LCD in 4-bit mode
  for(temp=3;temp;temp--)
  {
    LCD_send4(3);
    LCD_delay_ms(5);
  }
  LCD_send4(2);
  // configure the display
  LCD_send8(0,0x20 | md1);
  LCD_send8(0,0x08 | md2);
  LCD_send8(0,1);
  LCD_send8(0,6);
}

void LCD_pos_xy(char x, char y)
{
  unsigned char addr;
  addr = y * 64 + x;
  LCD_send8(0,0x80|addr);
}

void LCD_write_char(char data)
// send a character to the display
{
   switch (data)
  {
    case '\f' 	:
      LCD_send8(0,1);
      LCD_delay_ms(5);
      break;
    case '\n'	:
    case '\r' 	:	
      LCD_pos_xy(0,1);
      break;
    default	:	
      LCD_send8(1,data);
  }
}

void LCD_write_string (char *data)
{
  while (*data)
  {
    LCD_write_char(*data);
    data++;
  }
}

void LCD_print_frac_int(unsigned int data)
{
  unsigned char aux, space;
  unsigned int sub = 10000;
  aux = 0;
  space = 1;
  while (sub)
  {
    aux = 0;
    while (data>=sub)
    {
      data -= sub;
      aux++;
      space = 0;
    }
    if (!space) LCD_write_char(aux+'0');
    sub /= 10;
    if (sub==10)
    {
      if (space) LCD_write_char('0');
      LCD_write_char('.');
      space = 0;
    }
  }
  if (space) LCD_write_char('0');
}

void LCD_print_char(unsigned char data)
{
  unsigned char aux, space;
  unsigned char sub = 100;
  aux = 0;
  space = 1;
  while (sub)
  {
    aux = 0;
    while (data>=sub)
    {
      data -= sub;
      aux++;
      space = 0;
    }
    if (!space) LCD_write_char(aux+'0');
    sub /= 10;
  }
  if (space) LCD_write_char('0');
}
