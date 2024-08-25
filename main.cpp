#include "Base.h"
#include "Enums.h"
#include "Main_menu.h"
#include "Game.h"

int main(int argc, char* args[]) {
	Gamewindow main_window;
	main_window.init();

	int game_option = main_menu(main_window);
	if (game_option != mm_quit) {
		main_window.delete_all_textures();
		play_game(main_window, game_option);
	}

	return 0;
}