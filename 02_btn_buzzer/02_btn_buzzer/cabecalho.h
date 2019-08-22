/*
--------------------------------------------------------------------------------
--                                                                            --
--                      Santa Maria - Rio Grande do Sul/BR                    --
--                                                                            --
--------------------------------------------------------------------------------
--                                                                            --
-- Design      : Alazão Board  v1.0	                                          --
-- File		   : cabecalhoh.h			                                   	  --
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
--  Features: botao up apita o buzzer por X seg								  --
--------------------------------------------------------------------------------
*/

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/*############## I/0 ###################*/
#define user_led		PINB6
#define btn_up			PINB2
#define buzzer			PINA1

/*############## CONSTANTES ###################*/
#define TIME_DELAY 300

//##############    MACROS  #####################
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))  //ativa o bit x da variável Y (coloca em 1)
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x)) //limpa o bit x da variável Y (coloca em 0)
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))   //testa o bit x da variável Y (retorna 0 ou 1)
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))  //troca o estado do bit x da variável Y (complementa)
