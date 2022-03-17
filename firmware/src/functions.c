#include "functions.h"

void init(){
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(LED3_PIO_ID);
	
	pio_set_output(BUZZER_PIO, BUZZER_IDX_MASK, 0, 0, 0);
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(START_PIO_ID);
	pmc_enable_periph_clk(SELECAO_PIO_ID);
	
	pio_configure(START_PIO, PIO_INPUT, START_IDX_MASK, PIO_PULLUP);
	pio_configure(SELECAO_PIO, PIO_INPUT, SELECAO_IDX_MASK, PIO_PULLUP);
	
	pio_handler_set(START_PIO,
	START_PIO_ID,
	START_IDX_MASK,
	PIO_IT_FALL_EDGE,
	start_callback);
	
	pio_handler_set(SELECAO_PIO,
	SELECAO_PIO_ID,
	SELECAO_IDX_MASK,
	PIO_IT_EDGE,
	selecao_callback);
	
	pio_enable_interrupt(START_PIO, START_IDX_MASK);
	pio_get_interrupt_status(START_PIO);
	
	pio_enable_interrupt(SELECAO_PIO, SELECAO_IDX_MASK);
	pio_get_interrupt_status(SELECAO_PIO);
	
	NVIC_EnableIRQ(START_PIO_ID);
	NVIC_SetPriority(START_PIO_ID, 4);
	
	NVIC_EnableIRQ(SELECAO_PIO_ID);
	NVIC_SetPriority(SELECAO_PIO_ID, 4);
}

void start_callback(){
	start_flag = !start_flag;
}

void selecao_callback(){
	if (pio_get(SELECAO_PIO, PIO_INPUT, SELECAO_IDX_MASK)) {
		selecao_flag = 1;
		counter = counter + 1;
	}
	
	else {
		selecao_flag = 0;
	}
	
	if (counter > 3) {
		counter = 0;
	}
}

void set_buzzer(){
	pio_set(BUZZER_PIO, BUZZER_IDX_MASK);
}

void clear_buzzer(){
	pio_clear(BUZZER_PIO, BUZZER_IDX_MASK);
}

void set_leds(int n){
	if (n % 2 == 0) {
		pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
		pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
	}
	else if (n < 0) {
		pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
		pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
		pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
	}
	else {
		pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
	}
}

void clear_leds(){
	pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
	pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
	pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
}

int get_startstop(){
	return pio_get(START_PIO, PIO_INPUT, START_IDX_MASK);
}

int get_selecao(){
	return pio_get(SELECAO_PIO, PIO_INPUT, SELECAO_IDX_MASK);
}

void run_buzzer(double freq, double duracao){
	
	if (freq > 0) {
		double periodo = 1/freq;
	
		for(int i = 0; i<(freq*0.001*duracao); i++){
			set_buzzer();
			clear_leds();
			delay_us(periodo*1000000/2);
		
			clear_buzzer();
			set_leds(i);
			delay_us(periodo*1000000/2);
		} 
	}	
	
	else {
		delay_ms(duracao);
		clear_leds();
	}
}

void draw_filled_rect(int notes, int thisNote) {
	int i = (thisNote * BAR_COMPLETED) / notes;
	
	gfx_mono_generic_draw_rect(5, 5, 120, 10, 1);
	gfx_mono_generic_draw_filled_rect(5, 5, i, 10, 1);	
}

void play(int melody[], int duracao, int notes) {
	
	if (start_flag) {
	
		int divider = 0, noteDuration = 0;
		int wholenote = (60000 * 4) / duracao; // calcula a duração the uma nota inteira em ms

		for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
			
			while(start_flag) {}

			// calcula a duração de cada nota
			divider = melody[thisNote + 1];
			if (divider > 0) {
				noteDuration = (wholenote) / divider;
			}
			else if (divider < 0) {
				noteDuration = (wholenote) / abs(divider);
				noteDuration *= 1.5; // aumenta a duração
			}

			// toca a nota desejada 
			draw_filled_rect(notes, thisNote/2);
			
			run_buzzer(melody[thisNote], noteDuration * 0.9);
			delay_ms(noteDuration * 0.1);
		}
		
		gfx_mono_generic_draw_filled_rect(5, 5, BAR_COMPLETED, 10, 0);
		
	}
	else {
		clear_buzzer();
		set_leds(-1);
	}
}
