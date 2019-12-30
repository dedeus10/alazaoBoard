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
--  Features: Hello World no LCD											  --
--------------------------------------------------------------------------------
*/

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*############## I/0 ###################*/
#define user_led PINB6
#define btn_up PINB2
#define buzzer PINA1

/*############## CONSTANTES ###################*/
#define SCREEN_INIT 0
#define SCREEN_CALENDAR 1
#define SCREEN_DHT 2
#define SCREEN_ADC 3
#define TAM_SCREEN 3

#define TIME_DELAY 300
#define TIME_LCD 1000
#define TAM_SAMPLES 30.0      //Tamanho de amostras do NTC
#define AD_PRECISION 1023     //Precisão do canal AD-1 8 bits (255) 10 bits (1023)
#define B_coefficient 3600.00 /* B coefficient of NTC Thermistor 3950 - 3100*/
#define Room_temperature 25.00
#define Series_Resistance 10000.00

//##############    MACROS  #####################
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))  //ativa o bit x da vari�vel Y (coloca em 1)
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x)) //limpa o bit x da vari�vel Y (coloca em 0)
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))   //testa o bit x da vari�vel Y (retorna 0 ou 1)
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))  //troca o estado do bit x da vari�vel Y (complementa)

/*############## VARIAVEIS ###################*/
int flag_update_screen = 0; //Flag para atualizar o LCD apenas quando necessario
float temperatura = 0.0;    //Variavel que contem o valor real da temperatura
int16_t acc_leituras = 0;   //Acumulador de leituras (10x)
float media_leituras = 0;   //Media aritmetica das leituras
int n_samples = 0;          //Numero de amostras de leitura
int16_t leitura_ad = 0;     //Variavel utilizada para efetuar a leitura do NTC (canal AD)
long NTC_Resistance;        //Usado para determinar a resistencia do NTC
int16_t luminosidade = 0;
float temperature = 0;
float humidity = 0;
int flag_atualizar_lcd  = 0;
int16_t cont_atualizar_lcd = 0;