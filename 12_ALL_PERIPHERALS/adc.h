void adc_init(void)
{
	// Configurando prescaler para 128, em 16Mhz,
	// resulta 125Khz (deve estar entre 50~200Khz)
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	// Desligando modo free-running
	//ADCSRA &= ~_BV(ADFR);
	// Habilitando ADC
	ADCSRA |= _BV(ADEN);
}

int analogRead(int porta)
{
	uint8_t low, high;
	uint8_t bToChannel = porta & 0x07;
	// Configurando o canal da porta a ser amostrado
	ADMUX = bToChannel;
	// Voltagem de referencia para AVCC
	ADMUX |= _BV(REFS0);
	// Desligando o ajuste de esquerda 
	// para ter 10 bits de resolu��o
	ADMUX &= ~_BV(ADLAR);
	// Ligando a amostragem e iniciando medi��o
	ADCSRA |= _BV(ADSC);
	// Bit ADIF � resetado qndo opera��o termina
	loop_until_bit_is_set(ADCSRA, ADIF);
	// lendo byte low primeiro para travar a leitura
	low  = ADCL;
	high = ADCH;
	// combinando bytes
	return (high << 8) | low;
}
