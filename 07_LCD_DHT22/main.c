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
#include "dht.c"

void lcd_begin()
{
	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Write_String("Eng. Comp. UFSM");
	Lcd4_Set_Cursor(2, 0);
	Lcd4_Write_String("  Hello World  ");
	_delay_ms(3000);
}

int main(void)
{
	//Configura os pinos como entrada ou saida
	//1 saida | 0 entrada
	//0b76543210
	DDRA = 0b1101;
	DDRC = 0b10001000;
	DDRD = 0b00011000;

	//Ativa pull-up para botoes
	PORTB |= ((1 << PORTB2));
	PORTD |= ((1 << PORTD0));

	//Inicialmente ligado
	set_bit(PORTB, user_led);

	//Inicia LCD com uma escrita
	lcd_begin();

	//init interrupt
	sei();

	char printbuff[100];
	float temperature = 0;
	float humidity = 0;

	while (1)
	{
		if (dht_gettemperaturehumidity(&temperature, &humidity) != -1)
		{

			Lcd4_Clear();
			Lcd4_Set_Cursor(1, 0);
			dtostrf(temperature, 2, 2, printbuff);
			Lcd4_Write_String("Temp.:");
			Lcd4_Write_String(printbuff);
			Lcd4_Write_String(" C");

			Lcd4_Set_Cursor(2, 0);
			dtostrf(humidity, 2, 2, printbuff);
			Lcd4_Write_String("Humid.:");
			Lcd4_Write_String(printbuff);
			Lcd4_Write_String("%");
		}
		else
		{
			Lcd4_Clear();
			Lcd4_Set_Cursor(1, 6);
			Lcd4_Write_String("Error");
			Lcd4_Set_Cursor(2, 2);
			Lcd4_Write_String("DHT Not found");
		}

		_delay_ms(1500);
	}
}
