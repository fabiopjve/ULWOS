/*
 * LCD.C - RL78 LCD Library
 *
 * Author: Fábio Pereira
 */

// the following defines are used to define the LCD pins
#include "iodefine.h"
#ifndef PIN_LCD_EN
#define PIN_LCD_EN          P5_bit.no4  // LCD enable pin
#define PIN_LCD_EN_DIR	    PM5_bit.no4 // LCD enable pin direction
#define PIN_LCD_RS          P5_bit.no5  // LCD rs pin
#define PIN_LCD_RS_DIR      PM5_bit.no5 // LCD rs pin direction
#define PIN_LCD_D7          P7_bit.no3
#define PIN_LCD_D7_DIR      PM7_bit.no3
#define PIN_LCD_D6          P7_bit.no2
#define PIN_LCD_D6_DIR      PM7_bit.no2
#define PIN_LCD_D5          P7_bit.no1
#define PIN_LCD_D5_DIR      PM7_bit.no1
#define PIN_LCD_D4          P7_bit.no0
#define PIN_LCD_D4_DIR      PM7_bit.no0
//#define PIN_LCD_RW          P14_bit.no7
//#define PIN_LCD_RW_DIR      PM14_bit.no7
#endif

#define CPU_CLOCK           32000

// LCD second line RAM address
#define LCD_LIN2            0x40        

// the following defines are used for display configuration
#define CURSOR_ON           2
#define CURSOR_OFF          0
#define CURSOR_BLINK        1
#define CURSOR_FIXED        0
#define DISPLAY_ON          4
#define DISPLAY_OFF         0
#define DISPLAY_8x5         0
#define DISPLAY_10x5        4
#define _2LINES             8
#define _1LINE              0

void LCD_delay_ms(unsigned char time);

void LCD_send4(char data);

void LCD_send8(char addr, char data );

void LCD_init(char md1, char md2 );

void LCD_pos_xy(char x, char y);

void LCD_write_char(char data);

void LCD_write_string (char *data);

void LCD_print_frac_int(unsigned int data);

void LCD_print_char(unsigned char data);

