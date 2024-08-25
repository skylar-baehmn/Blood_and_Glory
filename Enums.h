#pragma once
// convinient place to store all of the enums we may need
enum main_menu_enums {
	mm_quit,
	mm_new_game,
	mm_load_game,
	mme_total
};

enum texture_ids {
	main_menu_background_id,
	main_menu_banner_id,
	main_menu_new_game_id,
	main_menu_load_game_id,
	main_menu_quit_id,
	ui_bottom_banner_id,
	ui_gold_id,
	ui_reputation_id,
	scroll_wheel_border_id,
	scroll_wheel_box_id,
	slave_shop_background_id,
	slave_shop_windows_id,
	slave_shop_preview_slot_id,
	slave_shop_preview_character_window_id,
	slave_shop_main_window_id,
	slave_shop_main_window_price_id,
	temp_text_id,
	town_map_background_id,
	fight_pits_background_id,
	ludus_background_id,
	shop_background_id,
	map_id,

	temp_slave_shop_button_id,
	temp_fight_pits_button_id,
	temp_ludus_button_id,
	temp_shop_button_id,
	temp_map_button_id,

	texture_id_total
};

enum texture_types {
	no_texture,
	background,
	main_menu_banner,
	main_menu_new_game_button,
	main_menu_load_game_button,
	main_menu_quit_button,
	ui_bottom_banner,
	shop_title,
	shop_buy_character_window,
	temp_home_buttons,
	scroll_wheel_border,
	scroll_wheel_box,
	texture_types_total
};

enum font_sizes {
	shop_text = 30,
	total_font_sizes
};

enum character_types {
	slave,
	criminal,
	freeman,
	aristocrat,
	gladiator,
	character_types_total
};

enum gladiator_classes {
	no_glad_class,
	trax,
	hoplomachus,
	murmillo,
	retiarius,
	scissor,
	secutor,
	gladiator_classes_total
};

enum weapon_ids {
	no_weapon,
	gladius,				// simple short sword
	retes,					// weighted net
	fascina,				// trident
	spear,
	siccae,					// short scimitar
	pugio,					// short dagger
	gladius_hispaniensis,	// leaf-shaped sword
	old_gladius,			// gladius with a prominant triangular tip
	spatha,					// straight longsword
	weapon_types_total
};

enum shield_types {

};

enum armour_ids {
	no_armour,
	galea,					// standard roman helmet
	corium_ocrea,			// simple leather greaves
	lorica_corium,			// simple leather tunic
	pteruges_corium,		// simple leather skirt
	corium_manica,			// simple leather vambrace
	corium_scapula,			// simple leather pauldron
};

enum armour_locations {
	any_location,
	head,
	body,
	bicept,
	forearm,
	upper_legs,
	shin,
	armour_locations_total
};

enum armour_slots {
	head_slot,
	body_slot,
	left_bicept_slot,
	right_bicept_slot,
	left_forearm_slot,
	right_forearm_slot,
	upper_legs_slot,
	left_shin_slot,
	right_shin_slot,
	armour_slots_total
};