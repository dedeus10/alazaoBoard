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
-- Created     : 20 Aug 2019                                                  --
-- Update      : 21 Aug 2019                                                  --
--------------------------------------------------------------------------------
--                              Overview                                      --
--                                                                            --
--  Microcontroller: ATtiny88 - Atmel/Microchip								  --
--  Features: botao up liga o led botao down desliga						  --
--------------------------------------------------------------------------------
*/

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

//Bibliotecas de sistema
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Biblioteca de dev
#include "cabecalho.h"


int main(void)
{
	//Configura os pinos como entrada ou saida
	//1 saida | 0 entrada
	//0b76543210
	DDRB = 0b01000000;
	DDRD = 0b00000000;
	
	//Ativa pull-up para botoes
	PORTB |= ( (1 << PORTB2) );
	PORTD |= ( (1 << PORTD0) );
	
	//Inicialmente apagado
	clr_bit(PORTB, user_led);
	
	while (1)
	{
		/* ---------- BTN UP ------------ */
		if ((tst_bit(PINB, btn_up) == 0))
		{
			_delay_ms(200);
			while ((tst_bit(PINB, btn_up) == 0)) {}
			
			//Coloca em nivel alto o led
			set_bit(PORTB, user_led);
		}
		
		/* ---------- BTN DOWN ------------ */
		if ((tst_bit(PIND, btn_down) == 0))
		{
			_delay_ms(200);
			while ((tst_bit(PIND, btn_down) == 0)) {}
			
			//Coloca em nivel baixo o led
			clr_bit(PORTB, user_led);
		}
	}
}

