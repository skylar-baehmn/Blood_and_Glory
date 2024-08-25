#pragma once
#include "Base.h"
#include "Enums.h"

bool load_main_menu_media(Gamewindow& window);
void free_main_menu_media(Gamewindow& window);
int main_menu(Gamewindow& window);

int main_menu(Gamewindow& window) {
	if (!load_main_menu_media(window)) {
		printf("Something went wrong!\n");
		free_main_menu_media(window);
		return 0;
	}
	std::vector<int> on_screen_textures = {main_menu_background_id, main_menu_banner_id, main_menu_new_game_id, main_menu_load_game_id, main_menu_quit_id};

	bool quit = false;
	int main_menu_choice = 0;
	SDL_Event e;
	while (!quit) {
		window.render_textures(on_screen_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN){
				quit = true;
				if (window.clickable_texture(main_menu_new_game_id)) {
					main_menu_choice = mm_new_game;
				}
				else if (window.clickable_texture(main_menu_load_game_id)) {
					main_menu_choice = mm_load_game;
				}
				else if (window.clickable_texture(main_menu_quit_id)) {
					main_menu_choice = mm_quit;
				}
				else
					quit = false;
			}
		}
	}

	free_main_menu_media(window);
	return main_menu_choice;
}

bool load_main_menu_media(Gamewindow& window) {
	bool success = true;
	if (!window.load_texture(background, main_menu_background_id, "Assets/temp_images/temp_main_menu.png"))
		success = false;
	if (!window.load_texture(main_menu_banner, main_menu_banner_id, "Assets/temp_images/temp_mm_banner.png"))
		success = false;
	if (!window.load_texture(main_menu_new_game_button, main_menu_new_game_id, "Assets/temp_images/temp_mm_new_game_button.png"))
		success = false;
	if (!window.load_texture(main_menu_load_game_button, main_menu_load_game_id, "Assets/temp_images/temp_mm_load_game_button.png"))
		success = false;
	if (!window.load_texture(main_menu_quit_button, main_menu_quit_id, "Assets/temp_images/temp_mm_exit_button.png"))
		success = false;

	return success;
}

void free_main_menu_media(Gamewindow& window) {
	window.delete_texture({ main_menu_background_id, main_menu_banner_id, main_menu_new_game_id,main_menu_load_game_id, main_menu_quit_id });
}