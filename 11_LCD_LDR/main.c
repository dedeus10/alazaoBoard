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
#include "adc.h"

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

void update_lcd()
{
	char printbuff[100];
	//Lcd4_Clear();
	Lcd4_Set_Cursor(1, 0);
	if(leitura_ad > 700)
		Lcd4_Write_String("State: On        ");
	else
		Lcd4_Write_String("State: Off       ");

	Lcd4_Set_Cursor(2, 0);
	Lcd4_Write_String("ADC:    ");
	dtostrf(leitura_ad, 2, 2, printbuff);
	Lcd4_Write_String(printbuff);
	Lcd4_Write_String("        ");

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

	//Inicialmente apagado
	clr_bit(PORTB, user_led);

	//Inicia LCD com uma escrita
	lcd_begin();

	// Iniciando ADC
	adc_init();

	//init interrupt
	sei();

	while (1)
	{
		//Efetua a leitura do AD
		cli();
		leitura_ad = analogRead(1); //Efetua a Leitura do AD no PC1
		sei();

		update_lcd();

		_delay_ms(500);
	}
}
