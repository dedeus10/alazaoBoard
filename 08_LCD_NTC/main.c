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
	Lcd4_Clear();
	Lcd4_Set_Cursor(1, 0);
	Lcd4_Write_String("Temp: ");
	dtostrf(temperatura, 2, 2, printbuff);
	Lcd4_Write_String(printbuff);
	Lcd4_Write_String(" C");

	Lcd4_Set_Cursor(2, 0);
	Lcd4_Write_String("ADC: ");
	dtostrf(media_leituras, 2, 2, printbuff);
	Lcd4_Write_String(printbuff);

	flag_update_screen = 0;
}

bool le_temperatura()
{
	//Efetua a leitura do AD
	cli();
	leitura_ad = analogRead(0); //Efetua a Leitura do AD no PC0
	sei();

	if (leitura_ad <= 0 || leitura_ad > 1023)
	{
		//Descarta leitura
	}
	else
	{
		if (n_samples == (TAM_SAMPLES))
			return true;
		else
		{
			acc_leituras = acc_leituras + leitura_ad;
			n_samples++;	
		}
	}
	return false;
}

void stepTocelsius()
{

	float Thermistor_temperature;
	float analog_value;

	media_leituras = (float)(acc_leituras / TAM_SAMPLES);
	analog_value = media_leituras;
	//analog_value = leitura_ad;

	/* calculate the NTC resistance */
	NTC_Resistance = ((AD_PRECISION * Series_Resistance / analog_value) - Series_Resistance);
	Thermistor_temperature = log(NTC_Resistance); /* calculate natural log of resistance */

	/* Calculate Temperature using B parameter Equation */
	/* 1/T = 1/T0 + ((1/B_coefficient)*log(NTC_Resistance/Series_Resistance)) */
	Thermistor_temperature = ((1.0 / (Room_temperature + 273.15)) + (1.0 / B_coefficient) * log(NTC_Resistance / Series_Resistance));
	Thermistor_temperature = (1 / Thermistor_temperature) - 273.15; /* convert kelvin to ?C */

	cli(); //disable global interrupts
	temperatura = Thermistor_temperature;
	sei(); //Enable global interrupts
	
	acc_leituras = 0;
	n_samples = 0;
	flag_update_screen = 1;
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
		if(le_temperatura())
			stepTocelsius();
		
		if(flag_update_screen ==1)
			update_lcd();

		_delay_ms(200);
	}
}
