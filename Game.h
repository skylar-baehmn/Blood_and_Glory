#pragma once
#include "Base.h"
#include "Enums.h"

int town_map(Gamewindow& window, Gamestate& current_game);
int shop_for_slaves(Gamewindow& window, Gamestate& current_game);
int fighting_pits(Gamewindow& window, Gamestate& current_game);
int ludus(Gamewindow& window, Gamestate& current_game);
int simple_shop(Gamewindow& window, Gamestate& current_game);
int map(Gamewindow& window, Gamestate& current_game);

void get_shop_preview_portrait(Gamewindow& window, int texture_package, int texture_id);

bool load_ui_game_textures(Gamewindow& window, Gamestate& current_game);
bool load_slave_shop_textures(Gamewindow& window);
bool load_game_textures_package_1(Gamewindow& window);
bool load_game_textures_package_2(Gamewindow& window);
bool load_temp_buttons(Gamewindow& window);
void free_ui_game_textures(Gamewindow& window);
void free_slave_shop_textures(Gamewindow& window);
void free_game_textures_package_1(Gamewindow& window);
void free_game_textures_package_2(Gamewindow& window);
void free_temp_button_textures(Gamewindow& window);

int play_game(Gamewindow& window, int start_mod) {
	Gamestate current_game;
	if (start_mod == mm_new_game) {
		current_game = Gamestate();
	}
	else {
		printf("We cannot load game yet!\n");
	}

	if (!load_ui_game_textures(window, current_game))
		printf("Unable to load ui textures!\n");
	if (!load_game_textures_package_1(window))
		printf("Unable to load game texture package 1!\n");
	if (!load_temp_buttons(window))
		printf("Unable to load temp buttons!\n");

	town_map(window, current_game);

	free_game_textures_package_1(window);
	free_ui_game_textures(window);
	free_temp_button_textures(window);
	free_game_textures_package_2(window);
	return 0;
}

int town_map(Gamewindow& window, Gamestate& current_game) {
	if (!load_game_textures_package_2(window))
		printf("Unable to load game textures package 2!\n");

	std::vector<int> town_map_textures = { town_map_background_id, temp_slave_shop_button_id, temp_fight_pits_button_id,
	temp_ludus_button_id, temp_shop_button_id, temp_map_button_id, ui_bottom_banner_id, ui_gold_id, ui_reputation_id };

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		window.render_textures(town_map_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (window.clickable_texture(temp_slave_shop_button_id)) {
					shop_for_slaves(window, current_game);
				}
				else if (window.clickable_texture(temp_fight_pits_button_id)) {
					fighting_pits(window, current_game);
				}
				else if (window.clickable_texture(temp_ludus_button_id)) {
					ludus(window, current_game);
				}
				else if (window.clickable_texture(temp_shop_button_id)) {
					simple_shop(window, current_game);
				}
				else if (window.clickable_texture(temp_map_button_id)) {
					map(window, current_game);
				}
			}
		}
	}

	return 0;
}
int shop_for_slaves(Gamewindow& window, Gamestate& current_game) {
	if (!load_slave_shop_textures(window))
		printf("Unable to load forum servus textures!\n");
	int temp_texture_index = texture_id_total;
	int scrollable_textures_height = 0;

	SDL_Rect character_window_pos = window.get_texture(slave_shop_preview_slot_id)->copy_pos();
	SDL_Rect character_preview_window_pos = { character_window_pos.x + 5, character_window_pos.y + 5, 150, 150 };

	std::vector<int> slave_shop_textures = { slave_shop_background_id, slave_shop_windows_id, scroll_wheel_border_id, scroll_wheel_box_id };
	std::vector<int> clickable_textures = {};
	window.get_texture(scroll_wheel_border_id)->set_render_pos({ 1890, 295, 25, 584 });
	window.get_texture(scroll_wheel_box_id)->set_render_pos({ 1894, 301, 17, 0 });

	for (int i = 0; i < current_game.get_market_size(); i++) {
		Gladiator* slave = current_game.get_gladiator_in_market(i);

		SDL_Rect character_slot_name_pos = { (character_window_pos.x + 200), character_window_pos.y, character_window_pos.w, character_window_pos.h };

		window.load_texture(shop_buy_character_window, temp_texture_index, "Assets/temp_images/temp_slave_shop_preview_slot.png");
		window.get_texture(temp_texture_index)->set_render_pos(character_window_pos);
		window.get_texture(temp_texture_index)->set_scroll_range(305, 870);
		clickable_textures.push_back(temp_texture_index);
		slave_shop_textures.push_back(temp_texture_index++);
		scrollable_textures_height += character_window_pos.h;

		get_shop_preview_portrait(window, slave->get_texture_package(), temp_texture_index);
		window.get_texture(temp_texture_index)->set_render_pos(character_preview_window_pos);
		window.get_texture(temp_texture_index)->set_scroll_range(305, 870);
		slave_shop_textures.push_back(temp_texture_index++);

		window.create_single_line_text(slave->get_name().c_str(), character_slot_name_pos, temp_texture_index, 50);
		window.get_texture(temp_texture_index)->set_scroll_range(305, 870);
		slave_shop_textures.push_back(temp_texture_index++);

		character_window_pos.y += character_window_pos.h + 10;
		character_preview_window_pos.y += character_window_pos.h + 10;
		scrollable_textures_height += 10;
	}

	std::vector<int> cleanup(slave_shop_textures.begin() + 4, slave_shop_textures.end());
	slave_shop_textures.push_back(ui_bottom_banner_id);
	slave_shop_textures.push_back(ui_gold_id);
	slave_shop_textures.push_back(ui_reputation_id);
	float box_scale = 565.0 / (float)scrollable_textures_height;
	int border_padding = (window.get_texture(scroll_wheel_box_id)->get_pos()->y - window.get_texture(scroll_wheel_border_id)->get_pos()->y);
	if (box_scale < 1)
		window.get_texture(scroll_wheel_box_id)->get_pos()->h = (window.get_texture(scroll_wheel_border_id)->get_pos()->h * box_scale);
	else 
		window.get_texture(scroll_wheel_box_id)->get_pos()->h = (window.get_texture(scroll_wheel_border_id)->get_pos()->h - (2 * border_padding));
	window.create_single_line_text("You clicked " + current_game.get_gladiator_in_market(0)->get_name(), {150, 350, 0, 0}, slave_shop_main_window_id, 50);
	slave_shop_textures.push_back(slave_shop_main_window_id);
	window.create_single_line_text("Price : " + std::to_string(current_game.get_gladiator_in_market(0)->get_value()), { 150, 500, 0, 0 }, slave_shop_main_window_price_id, 50);
	slave_shop_textures.push_back(slave_shop_main_window_price_id);

	bool quit = false;
	bool mouse_down = false;
	bool scroll_wheel_click = false;
	SDL_Point click_pos;
	SDL_Event e;
	while (!quit) {
		window.render_textures(slave_shop_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEWHEEL) {
				window.scroll_textures(e.wheel.y, box_scale, border_padding, cleanup);
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				mouse_down = true;
				click_pos = {0, 0};
				window.get_mouse_pos(&click_pos.x, &click_pos.y);
				if (on_texture(click_pos, window.get_texture(scroll_wheel_border_id)->copy_pos())) {
					scroll_wheel_click = true;
				}
				else {
					for (int i = 0; i < clickable_textures.size(); i++) {
						if (window.clickable_texture(clickable_textures[i])) {
							window.create_single_line_text("You clicked " + current_game.get_gladiator_in_market(i)->get_name(), { 150, 350, 0, 0 }, slave_shop_main_window_id, 50);
							window.create_single_line_text("Price : " + std::to_string(current_game.get_gladiator_in_market(i)->get_value()), { 150, 500, 0, 0 }, slave_shop_main_window_price_id, 50);
						}
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				mouse_down = false;
				scroll_wheel_click = false;
			}
		}
		if (mouse_down) {
			SDL_Point mpos = { 0, 0 };
			window.get_mouse_pos(&mpos.x, &mpos.y);
			int difference = click_pos.y - mpos.y;
			if (scroll_wheel_click)
				window.scroll_textures(difference, box_scale, border_padding, cleanup, false);
			click_pos = mpos;
		}
	}

	window.delete_texture(cleanup);
	free_slave_shop_textures(window);
	return 0;
}
int fighting_pits(Gamewindow& window, Gamestate& current_game) {
	std::vector<int> fighting_pits_texture = { fight_pits_background_id, ui_bottom_banner_id, ui_gold_id, ui_reputation_id };
	
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		window.render_textures(fighting_pits_texture);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
		}
	}

	return 0;
}
int ludus(Gamewindow& window, Gamestate& current_game) {
	std::vector<int> ludus_textures = { ludus_background_id, ui_bottom_banner_id, ui_gold_id, ui_reputation_id };

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		window.render_textures(ludus_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
		}
	}

	return 0;
}
int simple_shop(Gamewindow& window, Gamestate& current_game) {
	std::vector<int> shop_textures = { shop_background_id, ui_bottom_banner_id, ui_gold_id, ui_reputation_id };

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		window.render_textures(shop_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
		}
	}

	return 0;
}
int map(Gamewindow& window, Gamestate& current_game) {
	std::vector<int> map_textures = { map_id, ui_bottom_banner_id, ui_gold_id, ui_reputation_id };

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		window.render_textures(map_textures);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
				}
			}
		}
	}

	return 0;
}

void get_shop_preview_portrait(Gamewindow& window, int texture_package, int texture_id) {
	switch (texture_package) {
	case 0:
		window.load_texture(shop_buy_character_window, texture_id, "Assets/temp_images/temp_slave_preview_0.png");
		break;
	case 1:
		window.load_texture(shop_buy_character_window, texture_id, "Assets/temp_images/temp_slave_preview_1.png");
		break;
	case 2:
		window.load_texture(shop_buy_character_window, texture_id, "Assets/temp_images/temp_slave_preview_2.png");
		break;
	case 3:
		window.load_texture(shop_buy_character_window, texture_id, "Assets/temp_images/temp_slave_preview_3.png");
		break;
	}
}

bool load_ui_game_textures(Gamewindow& window, Gamestate& current_game) {
	bool success = true;
	if (!window.load_texture(ui_bottom_banner, ui_bottom_banner_id, "Assets/temp_images/temp_ui.png"))
		success = false;
	window.create_ui_text(current_game);
	if (!window.load_texture(scroll_wheel_border, scroll_wheel_border_id, "Assets/temp_images/scroll_wheel_border.png"))
		success = false;
	if (!window.load_texture(scroll_wheel_box, scroll_wheel_box_id, "Assets/temp_images/scroll_wheel_box.png"))
		success = false;

	return success;
}
bool load_slave_shop_textures(Gamewindow& window) {
	bool success = true;
	if (!window.load_texture(background, slave_shop_background_id, "Assets/temp_images/temp_slave_shop_background.png"))
		success = false;
	if (!window.load_texture(shop_buy_character_window, slave_shop_preview_slot_id, "Assets/temp_images/temp_slave_shop_preview_slot.png"))
		success = false;
	if (!window.load_texture(background, slave_shop_windows_id, "Assets/temp_images/temp_slave_shop_windows.png"))
		success = false;

	return success;
}
bool load_game_textures_package_1(Gamewindow& window) {
	bool success = true;
	if (!window.load_texture(background, town_map_background_id, "Assets/temp_images/temp_town_map.png"))
		success = false;

	return success;
}
bool load_game_textures_package_2(Gamewindow& window) {
	bool success = true;
	if (!window.load_texture(background, fight_pits_background_id, "Assets/temp_images/temp_fight_pits_background.png"))
		success = false;
	if (!window.load_texture(background, ludus_background_id, "Assets/temp_images/temp_ludus_background.png"))
		success = false;
	if (!window.load_texture(background, shop_background_id, "Assets/temp_images/temp_shop_background.png"))
		success = false;
	if (!window.load_texture(background, map_id, "Assets/temp_images/temp_map.png"))
		success = false;

	return success;
}
bool load_temp_buttons(Gamewindow& window) {
	bool success = true;
	if (!window.load_texture(temp_home_buttons, temp_slave_shop_button_id, "Assets/temp_images/temp_slave_shop_button.png"))
		success = false;
	if (!window.load_texture(temp_home_buttons, temp_fight_pits_button_id, "Assets/temp_images/temp_fight_pits_button.png"))
		success = false;
	if (!window.load_texture(temp_home_buttons, temp_ludus_button_id, "Assets/temp_images/temp_ludus_button.png"))
		success = false;
	if (!window.load_texture(temp_home_buttons, temp_shop_button_id, "Assets/temp_images/temp_shop_button.png"))
		success = false;
	if (!window.load_texture(temp_home_buttons, temp_map_button_id, "Assets/temp_images/temp_map_button.png"))
		success = false;

	window.get_texture(temp_slave_shop_button_id)->change_render_xy({400, 300});
	window.get_texture(temp_fight_pits_button_id)->change_render_xy({ 800, 300 });
	window.get_texture(temp_ludus_button_id)->change_render_xy({ 1200, 300 });
	window.get_texture(temp_shop_button_id)->change_render_xy({ 600, 600 });
	window.get_texture(temp_map_button_id)->change_render_xy({ 1000, 600 });

	return success;
}

void free_ui_game_textures(Gamewindow& window) {
	window.delete_texture({ ui_bottom_banner_id, ui_gold_id, ui_reputation_id });
}
void free_slave_shop_textures(Gamewindow& window) {
	window.delete_texture({ slave_shop_background_id, slave_shop_preview_slot_id, slave_shop_windows_id });
}
void free_game_textures_package_1(Gamewindow& window) {
	window.delete_texture({  town_map_background_id });
}
void free_game_textures_package_2(Gamewindow& window) {
	window.delete_texture({ fight_pits_background_id, ludus_background_id, shop_background_id, map_id });
}
void free_temp_button_textures(Gamewindow& window) {
	window.delete_texture({ temp_slave_shop_button_id, temp_fight_pits_button_id, temp_ludus_button_id, temp_shop_button_id, temp_map_button_id });
}