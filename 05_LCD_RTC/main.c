/*
--------------------------------------------------------------------------------
--                                                                            --
--                      Santa Maria - Rio Grande do Sul/BR                    --
--                                                                            --
--------------------------------------------------------------------------------
--                                                                            --
-- Design      : Alazao Board  v1.0	                                          --
-- File		   : main.c					                                 	  --
-- Author      : Luis Felipe de Deus                                          --
--                                                                            --
--------------------------------------------------------------------------------
--                                                                            --
-- Created     : 16 Dez 2019                                                  --
-- Update      : 16 Dez 2019                                                  --
--------------------------------------------------------------------------------
--                              Overview                                      --
--                                                                            --
--  Microcontroller: ATtiny88 - Atmel/Microchip								  --
--  Features: LCD - Calendar with Real Time Clock(RTC)						--
--------------------------------------------------------------------------------
*/

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

//Display Ports
#define D4 eS_PORTD3
#define D5 eS_PORTD4
#define D6 eS_PORTA2
#define D7 eS_PORTA3
#define RS eS_PORTC7
#define EN eS_PORTA0


//Bibliotecas de sistema
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//Biblioteca de dev
#include "cabecalho.h"
#include "lcd.h"
#include "I2C_Master_C_file.c"
#include "rtc_lib.h"

void lcd_begin()
{
	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Eng. Comp. UFSM");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("  Hello World  ");
	_delay_ms(3000);
}

int main(void)
{
	//Configura os pinos como entrada ou saida
	//1 saida | 0 entrada
	//0b76543210
	DDRA = 0b1101;
	DDRC = 0b10000000;
	DDRD = 0b00011000;

	//Ativa pull-up para botoes
	PORTB |= ( (1 << PORTB2) );
	PORTD |= ( (1 << PORTD0) );
	
	//Inicialmente apagado
	clr_bit(PORTB, user_led);

	//Inicia LCD com uma escrita
	lcd_begin();
	
	char buffer[20];
	//char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	
	I2C_Init();										/* Initialize I2C */
	
	
	//Escreve data e hora no RTC
	//Hora/Min/Sec/Formato
	//DiaSemana/Dia/Mes/Ano
	//No primeiro uso é necessario gravar a data no RTC e logo apos comentar as duas linhas e gravar novamente o codigo
	//RTC_Clock_Write(0x20, 0x12, 0x00, hour_24);
	//RTC_Calendar_Write(0x02, 0x16, 0x12, 0x19);	
	

    while(1)
    {
		RTC_Read_Clock(0);							/* Read the clock with second address i.e location is 0 */
		sprintf(buffer, "HORA: %02x:%02x:%02x  ", hour, minute, second);
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String(buffer);
		
		RTC_Read_Calendar(3);						/* Read the calender with day address i.e location is 3 */
	    sprintf(buffer, "DATA: %02x/%02x/%02x", date, month, year);
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String(buffer);
		
		_delay_ms(1000);
    }
}

