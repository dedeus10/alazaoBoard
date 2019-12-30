/*
--------------------------------------------------------------------------------
--                                                                            --
--                      Santa Maria - Rio Grande do Sul/BR                    --
--                                                                            --
--------------------------------------------------------------------------------
--                                                                            --
-- Design      : Alaz�o Board  v1.0	                                          --
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
--  Features: Hello World no LCD with button											  --
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


void update_lcd(bool i){
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	if(i == true)
		Lcd4_Write_String("  Good Morning  ");
	else
		Lcd4_Write_String(" Good Afternoon ");
	Lcd4_Set_Cursor(2,4);
	Lcd4_Write_String("Folks!!!");
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

	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Eng. Comp. UFSM");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("  Hello World  ");
	_delay_ms(3000);
	
	bool i = false;
	while (1)
	{

		/* ---------- BTN UP ------------ */
		if ((tst_bit(PINB, btn_up) == 0))
		{
			_delay_ms(200);
			while ((tst_bit(PINB, btn_up) == 0)) {}
			
			update_lcd(i);
			i =! i;
		}
		
		
	}
}

