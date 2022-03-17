#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <asf.h>
#include <stdio.h>
#include "variables.h"

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

void init();
void start_callback();
void selecao_callback();

void set_buzzer();
void set_leds(int i);
void clear_buzzer();
void clear_leds();

int get_startstop();
int get_selecao();

void run_buzzer(double freq, double duracao);
void draw_filled_rect(int notes, int thisNote);
void play(int melody[], int tempo, int notes);


#endif /* FUNCTIONS_H */