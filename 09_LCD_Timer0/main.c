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
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


//Biblioteca de dev
#include "cabecalho.h"
#include "lcd.h"
#include "I2C_Master_C_file.c"
#include "rtc_lib.h"

uint16_t count_update_lcd = 0;
int flag_update_screen = 0;
#define TIME_UPDATE_LCD 1000
uint16_t count_toggle_led = 0;

ISR(TIMER0_COMPA_vect)
{
	flag_update_screen = 1;
	//Action to be done every 1ms
	if (count_update_lcd >= TIME_UPDATE_LCD)
	{
		count_update_lcd = 0;
		flag_update_screen = 1;
	}
	else
		count_update_lcd++;
	
	
	if (count_toggle_led >= (TIME_UPDATE_LCD*2))
	{
		count_toggle_led = 0;
		clr_bit(PORTB, user_led);

	}
	else
		count_toggle_led++;
	
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Set_Cursor(1, 0);
}

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
	char buffer[20];
	RTC_Read_Clock(0); /* Read the clock with second address i.e location is 0 */
	sprintf(buffer, "HORA: %02x:%02x:%02x  ", hour, minute, second);

	Lcd4_Clear();
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Write_String(buffer);

	RTC_Read_Calendar(3); /* Read the calender with day address i.e location is 3 */
	sprintf(buffer, "DATA: %02x/%02x/%02x", date, month, year);
	Lcd4_Set_Cursor(2, 0);
	Lcd4_Write_String(buffer);

	flag_update_screen = 0;
}

int main(void)
{
	//Configura os pinos como entrada ou saida
	//1 saida | 0 entrada
	//0b76543210
	DDRA = 0b1101;
	DDRB = 0xFF;
	DDRC = 0b10001000;
	DDRD = 0b00011000;

	//Ativa pull-up para botoes
	PORTB |= ((1 << PORTB2));
	PORTD |= ((1 << PORTD0));

	//Inicialmente ligado
	set_bit(PORTB, user_led);

	//Inicia LCD com uma escrita
	lcd_begin();

	//SET TIMER0 FOR EXECUTION FLOW
	//Set the Timer Mode to CTC and prescaler
	TCCR0A |= (1 << CTC0) | (1 << CS00) | (1 << CS01);

	//Set the value that you want to count to
	OCR0A = 0x7C;

	//Set the ISR COMPA vect
	TIMSK0 |= (1 << OCIE0A);

	/* Initialize I2C */
	I2C_Init();


	//init interrupt
	sei();

	while (1)
	{
		if (flag_update_screen == 1)
			update_lcd();
	}
}
