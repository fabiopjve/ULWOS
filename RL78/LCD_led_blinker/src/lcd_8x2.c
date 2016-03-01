/************************************************************************/
/*  LCD.C - Biblioteca de manipula��o de m�dulo LCD no RL78             */
/*                                                                      */
/*  Autor: F�bio Pereira                                                */
/*                                                                      */
/************************************************************************/

// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
#include "iodefine.h"
#include "lcd_8x2.h"

void LCD_delay_ms(unsigned char time)
{
  unsigned int temp;
  for (;time;time--) for (temp=CPU_CLOCK/30;temp;temp--) asm("NOP");
}

//**************************************************************************
//* Fun��o de envio de um nibble para o display
//**************************************************************************
//* Argumentos de chamada:
//* char dado : dado a ser enviado para o display (somente o nibble inferior)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send4(char data)
{
  // coloca os quatro bits nas saidas
#ifdef PIN_LCD_RW
	PIN_LCD_RW = 0;
#endif
  //PORT_LCD_DATA4 = data & 0x0F;
  if (data & 0x08) PIN_LCD_D7=1; else PIN_LCD_D7=0 ;
  if (data & 0x04) PIN_LCD_D6=1; else PIN_LCD_D6=0 ;
  if (data & 0x02) PIN_LCD_D5=1; else PIN_LCD_D5=0 ;
  if (data & 0x01) PIN_LCD_D4=1; else PIN_LCD_D4=0 ;
  // pulsa a linha enable
  PIN_LCD_EN = 1;
  PIN_LCD_EN = 0;
}
//**************************************************************************
//* Fun��o de escrita de 1 byte no display
//**************************************************************************
//* Argumentos de chamada:
//* char endereco : 0 se instru��o, 1 se dado
//* char dado : dado ou comando a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send8(char addr, char data )
{
  // configura a linha rs dependendo do modo selecionado
  PIN_LCD_RS = addr;
  LCD_delay_ms(1);
  // envia a primeira parte do byte
  LCD_send4(data >> 4);
  // envia a segunda parte do byte
  LCD_send4(data & 0x0f);
}

//**************************************************************************
//* Fun��o de inicializa��o do display
//**************************************************************************
//* Argumentos de chamada:
//* char modo1 : modo do display (n�mero de linhas e tamanho dos caracteres
//* char modo2 : modo do display (estado do cursor e do display)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_init(char md1, char md2 )
{
  unsigned char temp;
  // configura os pinos como sa�das
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
  // coloca os pinos em n�vel l�gico 0
  //PORT_LCD_DATA4 = 0;
  PIN_LCD_D7 = 0;
  PIN_LCD_D6 = 0;
  PIN_LCD_D5 = 0;
  PIN_LCD_D4 = 0;
  PIN_LCD_RS = 0;
  PIN_LCD_EN = 0;
  LCD_delay_ms(15);
  // envia uma seq��ncia de 3 vezes 0x03 e depois 0x02 para configurar o m�dulo
  // no modo de 4 bits
  for(temp=3;temp;temp--)
  {
    LCD_send4(3);
    LCD_delay_ms(5);
  }
  LCD_send4(2);
  // envia c�digos de inicializa��o do display
  LCD_send8(0,0x20 | md1);
  LCD_send8(0,0x08 | md2);
  LCD_send8(0,1);
  LCD_send8(0,6);
}

//**************************************************************************
//* Fun��o de posicionamento do cursor do display
//**************************************************************************
//* Argumentos de chamada:
//* char x : coluna a ser posicionado o cursor (iniciando em 0)
//* char y : linha a ser posicionado o cursor (0 ou 1)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  unsigned char addr;
  addr = y * 64 + x;
  LCD_send8(0,0x80|addr);
}

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
void LCD_write_char(char data)
// envia um caractere para o display
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

//**************************************************************************
//* Fun��o de escrita de uma string no display
//**************************************************************************
//* Argumentos de chamada:
//* char *c : um ponteiro para um caractere
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_write_string (char *data)
{
  while (*data)
  {
    LCD_write_char(*data);
    data++;
  }
}

/*
  Esta fun��o escreve um valor inteiro no LCD. Ela utiliza uma formata��o
  especial para representar um valor fracion�rio atrav�s de inteiros.
  Os d�gitos de dezenas de milhares, milhares e centenas s�o impressos,
  seguidos do ponto decimal e dos d�gitos de dezenas e unidades.
  Assim, o valor 1001 � apresentado como 10.01.
*/
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
