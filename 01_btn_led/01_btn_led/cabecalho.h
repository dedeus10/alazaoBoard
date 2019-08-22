/*
--------------------------------------------------------------------------------
--                                                                            --
--                      Santa Maria - Rio Grande do Sul/BR                    --
--                                                                            --
--------------------------------------------------------------------------------
--                                                                            --
-- Design      : Alaz�o Board  v1.0	                                          --
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
--  Features: botao up liga o led botao down desliga						  --
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
#define btn_down		PIND0

//##############    MACROS  #####################
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))  //ativa o bit x da vari�vel Y (coloca em 1)
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x)) //limpa o bit x da vari�vel Y (coloca em 0)
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))   //testa o bit x da vari�vel Y (retorna 0 ou 1)
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))  //troca o estado do bit x da vari�vel Y (complementa)
