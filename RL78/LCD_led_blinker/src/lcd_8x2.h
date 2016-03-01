/************************************************************************/
/*  LCD.C - Biblioteca de manipula��o de m�dulo LCD no RL78             */
/*                                                                      */
/*  Autor: F�bio Pereira                                                */
/*                                                                      */
/************************************************************************/

// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
#include "iodefine.h"
#ifndef PIN_LCD_EN
#define PIN_LCD_EN          P5_bit.no4  // pino enable do LCD
#define PIN_LCD_EN_DIR	    PM5_bit.no4 // dire��o do pino enable
#define PIN_LCD_RS          P5_bit.no5  // pino rs do LCD
#define PIN_LCD_RS_DIR      PM5_bit.no5 // dire��o do pino rs
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

// Endereço da segunda linha na RAM do LCD
#define LCD_LIN2            0x40        

// Defini��es utilizadas para configura��o do display
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

//**************************************************************************
//* Fun��o de envio de um nibble para o display
//**************************************************************************
//* Argumentos de chamada:
//* char dado : dado a ser enviado para o display (somente o nibble inferior)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send4(char data);

//**************************************************************************
//* Fun��o de escrita de 1 byte no display
//**************************************************************************
//* Argumentos de chamada:
//* char endereco : 0 se instru��o, 1 se dado
//* char dado : dado ou comando a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send8(char addr, char data );

//**************************************************************************
//* Fun��o de inicializa��o do display
//**************************************************************************
//* Argumentos de chamada:
//* char modo1 : modo do display (n�mero de linhas e tamanho dos caracteres
//* char modo2 : modo do display (estado do cursor e do display)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_init(char md1, char md2 );

//**************************************************************************
//* Fun��o de posicionamento do cursor do display
//**************************************************************************
//* Argumentos de chamada:
//* char x : coluna a ser posicionado o cursor (iniciando em 0)
//* char y : linha a ser posicionado o cursor (0 ou 1)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_pos_xy(char x, char y);

//**************************************************************************
//* Fun��o de escrita de um caractere no display
//**************************************************************************
//* Argumentos de chamada :
//* char c : caractere a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
//* Observa��es :
//*	\f apaga o conte�do do display
//* \n e \r retornam o cursor para a primeira coluna da segunda linha
//**************************************************************************
void LCD_write_char(char data);

//**************************************************************************
//* Fun��o de escrita de uma string no display
//**************************************************************************
//* Argumentos de chamada:
//* char *c : um ponteiro para um caractere
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_write_string (char *data);

/*
  Esta fun��o escreve um valor inteiro no LCD. Ela utiliza uma formata��o
  especial para representar um valor fracion�rio atrav�s de inteiros.
  Os d�gitos de dezenas de milhares, milhares e centenas s�o impressos,
  seguidos do ponto decimal e dos d�gitos de dezenas e unidades.
  Assim, o valor 1001 � apresentado como 10.01.
*/
void LCD_print_frac_int(unsigned int data);

void LCD_print_char(unsigned char data);

