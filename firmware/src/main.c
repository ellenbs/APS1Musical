#include <asf.h>
#include <stdio.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

#include "variables.h"
#include "musics.h"
#include "functions.h"

int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	init();
	
	int i = 1;
	
	// Init OLED
	gfx_mono_ssd1306_init();
	gfx_mono_draw_string("What song?", 0, 16, &sysfont);
	
	Musica selected_music = {};
  
	/* Insert application code here, after the board has been initialized. */
	while(1) {
		
		if (selecao_flag) {
			
			char blank[] = "             ";
			gfx_mono_draw_string(blank, 0, 16, &sysfont);
			
			if (counter == supermariobros.id) {
			    gfx_mono_draw_string(supermariobros.nome, 0, 16, &sysfont);
				selected_music = supermariobros;
			}
			
			else if (counter == harrypotter.id) {
			    gfx_mono_draw_string(harrypotter.nome, 0, 16, &sysfont);
				selected_music = harrypotter;
			}
			
			else if (counter == gameofthrones.id) {
			    gfx_mono_draw_string(gameofthrones.nome, 0, 16, &sysfont);
				selected_music = gameofthrones;
			}				
			
			play(selected_music.melodia, selected_music.duracao, selected_music.tamanho);
		}
	}
}
