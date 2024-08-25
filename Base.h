#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Enums.h"

struct anim_specs;
struct gladiator_stats;
struct gladiator_shop_stats;
struct base_specs;
struct player_stats;

class Ltexture;
class Gamewindow;
class Gladiator;
class Gamestate;
class Weapon;
class Armour;

bool on_texture(SDL_Point mouse_position, SDL_Rect texture_pos);
gladiator_stats create_random_gladiator_stats(int type);
std::string random_name(char gender);
int get_random(int min, int max);
int die_roll();

struct anim_specs {
	int current_row;
	int total_row;
	int current_col;
	int total_col;
	int current_sprite;
	int total_sprite;

	bool repeat;
	bool on_off;
};

struct gladiator_stats {
	// basic attr
	int strength;		// physical power - how hard someone can hit
	int dexterity;		// agility / coordination - how good someone is at hitting
	int intelligence;	// brainpower, intuition - ability to learn and remember skills
	int health;			// stamina / resistances - ability to keep fighting (not die)

	// str based stats
	int basic_lift;		// (str * str) / 5 - can be modified (how much a person can carry)
	int basic_strike;	// starts at 0 - can be modified (extra damage with all weapons, double for str based weapons)
	int hp;				// total health - needs to be balanced (@ 1/3 halves move and dodge)

	// dex based stats
	int basic_speed;	// (HT + DEX) / 4 - will affect combat speed
	int basic_combat;	// starts at 0 - can be modified (extra hit chance with all weapons, extra dmg for dex based weapons)
	int dodge;			// basic speed + 3 - can be modified

	// int based stats
	int willpower;		// == to int - ability to withstand stress (will stop a gladiator from giving up)
	int perception;		// == to int - ability to see changes during a fight
	int skills_total;	// 1/4 of int - how many skills can be learned

	// ht based stats
	int fatigue;		// == to ht - cost required to swing / move
};

struct gladiator_shop_stats {
	int strength_min;
	int strength_max;
	int dexterity_min;
	int dexterity_max;
	int intelligence_min;
	int intelligence_max;
	int health_min;
	int health_max;

	int basic_lift_min;
	int basic_lift_max;
	int basic_strike_min;
	int basic_strike_max;
	int hp_min;
	int hp_max;

	int basic_speed_min;
	int basic_speed_max;
	int basic_combat_min;
	int basic_combat_max;
	int dodge_min;
	int dodge_max;

	int willpower_min;
	int willpower_max;
	int perception_min;
	int perception_max;
	int skills_total_min;
	int skills_total_max;

	int fatigue_min;
	int fatigue_max;
};

struct base_specs {
	std::string base_name;
	std::string location;
	std::vector<int> base_textures;
};

struct player_stats {
	int intelligence;
	int ability_connoisseur;
	int weapon_connoisseur;
	int armour_connoisseur;
	int expert_skill_lanista;
	int current_affairs;
	int carousing;
	int accounting;
	int administration;
	int current_affairs_politics;
	int current_affairs_games;
	int diplomacy;
	int economics;
	int finance;
	int market_analysis;
	int politics;
	int merchant;
	int savoir_faire;
};

class Ltexture {
private:
	SDL_Texture* ptexture;
	int mimage_width;
	int mimage_height;

	SDL_Rect mtexture_render_pos;
	SDL_Rect mclip;
	SDL_Point mscroll_range;

	// animation specific variables
	int mtexture_type;
	bool mis_anim;
	anim_specs manim_var;

public:
	Ltexture() {
		ptexture = nullptr;
		mimage_width = 0;
		mimage_height = 0;
		mtexture_render_pos = { 0, 0, 0, 0 };
		mclip = { 0, 0, 0, 0 };
		mtexture_type = no_texture;
		mis_anim = false;
		manim_var = { 0, 0, 0, 0, 0, 0, false, false };
		mscroll_range = { 0, 1080 };
	}

	Ltexture(SDL_Texture* texture, int width, int height) {
		ptexture = texture;
		mimage_width = width;
		mimage_height = height;
		mtexture_type = no_texture;
		mis_anim = false;
		manim_var = { 0, 0, 0, 0, 0, 0, false, false };
		mscroll_range = { 0, 1080 };
	}

	~Ltexture() {
		free();
	}

	void free() {
		SDL_DestroyTexture(ptexture);
		ptexture = nullptr;
		mimage_width = 0;
		mimage_height = 0;
		mtexture_render_pos = { 0, 0, 0, 0 };
		mclip = { 0, 0, 0, 0 };
		mtexture_type = no_texture;
		mis_anim = false;
		manim_var = { 0, 0, 0, 0, 0, 0, false, false };
		mscroll_range = { 0, 0 };
	}

	void set_default_rects(int texture_type) {
		int extra_pixels_total, top_extra, left_extra;
		mtexture_type = texture_type;
		switch (texture_type) {
		case background:
			mtexture_render_pos = { 0, 0, mimage_width, mimage_height };
			mclip = {0, 0, mimage_width, mimage_height};
			break;

		case main_menu_banner:
			mtexture_render_pos = { 0, 500, mimage_width, mimage_height };
			mclip = { 0, 0, mimage_width, mimage_height };
			break;

		case main_menu_new_game_button:
			mis_anim = true;
			manim_var = { 1, 1, 1, 2, 1, 2, false, true };
			mtexture_render_pos = { 400, 550, mimage_width / 2, mimage_height };
			mclip = { 0, 0, mimage_width / manim_var.total_col, mimage_height / manim_var.total_row };
			break;

		case main_menu_load_game_button:
			mis_anim = true;
			manim_var = { 1, 1, 1, 2, 1, 2, false, true };
			mtexture_render_pos = { 800, 550, mimage_width / 2, mimage_height };
			mclip = { 0, 0, mimage_width / manim_var.total_col, mimage_height / manim_var.total_row };
			break;

		case main_menu_quit_button:
			mis_anim = true;
			manim_var = { 1, 1, 1, 2, 1, 2, false, true };
			mtexture_render_pos = { 1200, 550, mimage_width / 2, mimage_height };
			mclip = { 0, 0, mimage_width / manim_var.total_col, mimage_height / manim_var.total_row };
			break;

		case shop_title:
			mtexture_render_pos = { (1920 / 2) - (mimage_width / 2), 20, mimage_width, mimage_height };
			mclip = { 0, 0, mimage_width, mimage_height };
			break;

		case shop_buy_character_window:
			mtexture_render_pos = { 1210, 310, mimage_width, mimage_height};
			mclip = { 0, 0, mimage_width, mimage_height };
			break;

		case ui_bottom_banner:
			mtexture_render_pos = { 0, 1080 - mimage_height, mimage_width, mimage_height };
			mclip = { 0, 0, mimage_width, mimage_height };
			break;

		case temp_home_buttons:
			mis_anim = true;
			manim_var = { 1, 1, 1, 2, 1, 2, false, true };
			mtexture_render_pos = { 0, 0, mimage_width / 2, mimage_height };
			mclip = { 0, 0, mimage_width / 2, mimage_height };
			break;

		case scroll_wheel_border:
			mtexture_render_pos = { 0, 0, mimage_width, mimage_height };
			mclip = { 0, 0, mimage_width, mimage_height };
			break;

		case scroll_wheel_box:
			mtexture_render_pos = { 0, 0, mimage_width, mimage_height };
			mclip = { 0, 0, mimage_width, mimage_height };
			break;
		}
	}

	void get_next_clip() {
		if (manim_var.current_sprite < manim_var.total_sprite) {
			if (manim_var.current_sprite % manim_var.total_col == 0) {
				manim_var.current_row++;
				mclip.x = 0;
				mclip.y += mclip.h;
			}
			else {
				manim_var.current_col++;
				mclip.x += mclip.w;
			}
			manim_var.current_sprite++;
		}
	}

	void get_previous_clip() {
		if (manim_var.current_sprite > 1) {
			if (manim_var.current_sprite % manim_var.total_col == 1) {
				manim_var.current_row--;
				mclip.x = mimage_width - mclip.x;
				mclip.y -= mclip.h;
			}
			else {
				manim_var.current_col--;
				mclip.x -= mclip.w;
			}
			manim_var.current_sprite--;
		}
	}

	void set_texture(SDL_Texture* t) { ptexture = t; }
	void set_image_width(int w) { mimage_width = w; }
	void set_image_height(int h) { mimage_height = h; }
	void set_render_pos(SDL_Rect pos) { mtexture_render_pos = pos; }
	void change_render_xy(SDL_Point xy) { mtexture_render_pos.x = xy.x; mtexture_render_pos.y = xy.y; }
	void set_clip(SDL_Rect clip) { mclip = clip; }
	void set_anim_specs(anim_specs specs) { manim_var = specs; }
	void set_scroll_range(int min, int max) { mscroll_range = { min, max }; }

	SDL_Texture* get_texture() { return ptexture; }
	int get_width() { return mimage_width; }
	int get_height() { return mimage_height; }
	int get_texture_type() { return mtexture_type; }
	SDL_Point get_xy_pos() { return { mtexture_render_pos.x, mtexture_render_pos.y }; }
	SDL_Point get_wh_pos() { return { mtexture_render_pos.w, mtexture_render_pos.h }; }
	SDL_Rect* get_pos() { return &mtexture_render_pos; }
	SDL_Rect copy_pos() { return mtexture_render_pos; }
	SDL_Rect* get_clip() { return &mclip; }
	SDL_Rect copy_clip() { return mclip; }
	bool is_anim() { return mis_anim; }
	anim_specs get_anim_specs() { return manim_var; }
	SDL_Point* get_scroll_range() { return &mscroll_range; }
};

class Weapon {
private:
	int mweapon_id;
	std::string mweapon_name;
	int mmin_damage;
	int mmax_damage;

	float mcrush_mod;
	float mcut_mod;
	float mimpale_mod;

public:
	Weapon() {
		mweapon_id = 999;
	}

	Weapon(int id, std::string weapon_name, int min_damage, int max_damage, float crush_mod, float cut_mod, float impale_mod) {
		mweapon_id = id;
		mweapon_name = weapon_name;
		mmin_damage = min_damage;
		mmax_damage = max_damage;
		mcrush_mod = crush_mod;
		mcut_mod = cut_mod;
		mimpale_mod = impale_mod;
	}
};

class Armour {
private:
	int marmour_id;
	std::string marmour_name;
	int mlocation;
	int marmour_val;

	std::vector<int> armour_textures;

public:
	Armour() {
		marmour_id = 999;
	}

	Armour(int id, std::string name, int location, int armour_value) {
		marmour_id = id;
		marmour_name = name;
		mlocation = location;
		marmour_val = armour_value;
	}

	int get_armour_id() { return marmour_id; }
	int get_location() { return mlocation; }
	int get_armour_val() { return marmour_val; }
};

class Gladiator {
private:
	int mgladiator_type;
	int mgladiator_class;
	gladiator_stats mstats;
	gladiator_shop_stats mshop_stats;
	std::string mname;
	std::string mnickname;
	int mgold_value;

	int mtexture_package;

	// weapons will be tracked by id so that a million copies dont exist
	int mleft_hand_weapon;
	int mright_hand_weapon;

	// armour tracked by id
	int marmour_slots[armour_slots_total];

	int mhead_armour_val;
	int mbody_armour_val;
	int mleft_bicept_armour_val;
	int mright_bicept_armour_val;
	int mleft_forearm_armour_val;
	int mright_forearm_armour_val;
	int mupper_legs_armour_val;
	int mleft_shin_armour_val;
	int mright_shin_armour_val;

	std::vector<int> mgladiator_textures;

public:
	Gladiator(int type, int glad_class = no_glad_class, char gender = 'M') {
		mgladiator_type = type;
		mgladiator_class = glad_class;
		mstats = create_random_gladiator_stats(type);
		mname = random_name(gender);
		set_gladiator_gold_value();
		mtexture_package = get_random(0, 3);
		for (int i = 0; i < sizeof(marmour_slots); i++) {
			marmour_slots[i] = no_armour;
		}
	}

	void equip_armour(Armour& armour, bool left = true) {
		switch (armour.get_location()) {
		case head:
			marmour_slots[head_slot] = armour.get_armour_id();
			mhead_armour_val = armour.get_armour_val();
			break;
		case body:
			marmour_slots[body_slot] = armour.get_armour_id();
			mbody_armour_val = armour.get_armour_val();
			break;
		case bicept:
			if (left) {
				marmour_slots[left_bicept_slot] = armour.get_armour_id();
				mleft_bicept_armour_val = armour.get_armour_val();
			}
			else {
				marmour_slots[right_bicept_slot] = armour.get_armour_id();
				mright_bicept_armour_val = armour.get_armour_val();
			}
			break;
		case forearm:
			if (left) {
				marmour_slots[left_forearm_slot] = armour.get_armour_id();
				mleft_forearm_armour_val = armour.get_armour_val();
			}
			else {
				marmour_slots[right_bicept_slot] = armour.get_armour_id();
				mright_forearm_armour_val = armour.get_armour_val();
			}
			break;
		case upper_legs:
			marmour_slots[upper_legs_slot] = armour.get_armour_id();
			mupper_legs_armour_val = armour.get_armour_val();
			break;
		case shin:
			if (left) {
				marmour_slots[left_shin_slot] = armour.get_armour_id();
				mleft_shin_armour_val = armour.get_armour_val();
			}
			else {
				marmour_slots[right_shin_slot] = armour.get_armour_id();
				mright_shin_armour_val = armour.get_armour_val();
			}
			break;
		}
	}

	void set_gladiator_gold_value() {
		// the gold value will be based on current stats, and a random amount will be added
		int value = die_roll();
		value += get_random(mstats.strength - 5, mstats.strength + 5);
		value += get_random(mstats.dexterity - 5, mstats.dexterity + 5);
		value += get_random(mstats.intelligence - 5, mstats.intelligence + 5);
		value += get_random(mstats.health - 5, mstats.health + 5);

		int hp_gold_value = mstats.hp / 10;
		value += get_random(hp_gold_value - 3, hp_gold_value + 3);
		value += get_random(mstats.basic_speed - 3, mstats.basic_speed + 3);
		value += get_random(mstats.dodge - 3, mstats.dodge + 3);
		value += get_random(mstats.skills_total - 3, mstats.skills_total + 3);

		mgold_value = value;
	}

	int get_type() { return mgladiator_type; }
	gladiator_stats stats() { return mstats; }
	gladiator_shop_stats get_shop_stats() { return mshop_stats; }
	void set_shop_stats(gladiator_shop_stats s) { mshop_stats = s; }
	std::string get_name() { return mname; }
	std::string get_nickname() { return mnickname; }
	void set_value(int val) { mgold_value = val; }
	int get_value() { return mgold_value; }
	int get_texture_package() { return mtexture_package; }
	std::vector<int> get_textures() { return mgladiator_textures; }
};

class Gamestate {
private:
	// resources
	int mgold;
	float mreputation;
	player_stats mplayer_stat;

	// people
	std::vector<Gladiator> mroster;
	std::vector<Gladiator> mslave_market;

	// place
	base_specs mbase;

public:
	Gamestate() {
		mgold = 1000;
		mreputation = 100;
		mplayer_stat = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
		mroster = {};
		for (int i = 0; i < 3; i++) {
			mslave_market.push_back(Gladiator(slave));
			get_shop_discount(&mslave_market[mslave_market.size() - 1]);
		}
	}

	// these work in conjunction with creating gladiators, in order to set all the stats properly
	void get_shop_discount(Gladiator* gladiator) {
		// do a dice roll for each gladiator and compare to skill (merchant). Each success / fail is -/+ 5%
		int roll = die_roll();
		float discount;
		if (roll < 5 || (mplayer_stat.merchant - roll > 9))
			discount = 0.5;
		else if (roll > 16 || (mplayer_stat.merchant - roll < -9))
			discount = -0.5;
		else
			discount = (roll - mplayer_stat.merchant) * 0.05;
		gladiator->set_value(gladiator->get_value() + (gladiator->get_value() * discount));
	}
	void set_gladiator_shop_stats(Gladiator* gladiator) {
		int roll = die_roll();
		gladiator_shop_stats shop_stats = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		gladiator_stats real = gladiator->stats();

		if (roll < 5 || (mplayer_stat.ability_connoisseur - roll > 9)) {
			//for a crit success all values will be represented as a single number accurately
			shop_stats.strength_max = shop_stats.strength_min = real.strength;
			shop_stats.dexterity_max = shop_stats.dexterity_min = real.dexterity;
			shop_stats.intelligence_max = shop_stats.intelligence_min = real.intelligence;
			shop_stats.health_max = shop_stats.health_min = real.health;
			shop_stats.hp_max = shop_stats.hp_min = real.hp;
			shop_stats.dodge_max = shop_stats.dodge_min = real.dodge;
			shop_stats.skills_total_max = shop_stats.skills_total_min = real.skills_total;
			shop_stats.fatigue_max = shop_stats.fatigue_min = real.fatigue;
			shop_stats.basic_speed_max = shop_stats.basic_speed_min = real.basic_speed;
			shop_stats.basic_lift_max = shop_stats.basic_lift_min = real.basic_lift;
			shop_stats.willpower_max = shop_stats.willpower_min = real.willpower;
			shop_stats.perception_max = shop_stats.perception_min = real.perception;
			shop_stats.basic_strike_max = shop_stats.basic_strike_min = real.basic_strike;
			shop_stats.basic_combat_max = shop_stats.basic_combat_min = real.basic_combat;
		}
		else if (roll > 16 || (mplayer_stat.ability_connoisseur - roll < -9)) {
			// for a crit failure all stats will be represented by a random single number
			shop_stats.strength_max = shop_stats.strength_min = get_random(3, real.strength + 5);
			shop_stats.dexterity_max = shop_stats.dexterity_min = get_random(3, real.dexterity + 5);
			shop_stats.intelligence_max = shop_stats.intelligence_min = get_random(3, real.intelligence + 5);
			shop_stats.health_max = shop_stats.health_min = get_random(3, real.health + 5);
			shop_stats.hp_max = shop_stats.hp_min = (real.strength * 7) + (real.health * 4);
			shop_stats.fatigue_max = shop_stats.fatigue_min = shop_stats.health_max;
			shop_stats.basic_speed_max = shop_stats.basic_speed_min = (shop_stats.health_max + shop_stats.dexterity_max) / 4;
			shop_stats.dodge_max = shop_stats.dodge_min = shop_stats.basic_speed_max + 3;
			shop_stats.basic_lift_max = shop_stats.basic_lift_min = (shop_stats.strength_max * shop_stats.strength_max) / 5;
			shop_stats.willpower_max = shop_stats.willpower_min = shop_stats.intelligence_max;
			shop_stats.perception_max = shop_stats.perception_min = shop_stats.intelligence_max;
			shop_stats.skills_total_max = shop_stats.skills_total_min = shop_stats.intelligence_max / 4;
			shop_stats.basic_strike_max = shop_stats.basic_strike_min = get_random(0, 4);
			shop_stats.basic_combat_max = shop_stats.basic_combat_min = get_random(0, 4);
		}
		// now for non-crit rolls, we will reveal some information, but not neccessarily all
		else if (mplayer_stat.ability_connoisseur - roll < 0) {
			// this represents a failure, most stats will not be represented, the primary stats will be represented, but
			// not very accurate

			// start by getting a random number (real - 5) - (real + 5)
			shop_stats.strength_min = get_random(real.strength - 5, real.strength + 5);
			shop_stats.strength_max = shop_stats.strength_min + 10;
			// if the min is too low (< 3) then we shift all the numbers up
			if (shop_stats.strength_min < 3) {
				int shift = 3 - shop_stats.strength_min;
				shop_stats.strength_min + shift;
				shop_stats.strength_max + shift;
			}
			// if max is too high (> 18) then shift down
			if (shop_stats.strength_max > 18) {
				int shift = shop_stats.strength_max - 18;
				shop_stats.strength_max - shift;
				shop_stats.strength_min - shift;
			}

			// repeat for each primary attribute
			shop_stats.dexterity_min = get_random(real.dexterity - 5, real.dexterity + 5);
			shop_stats.dexterity_max = shop_stats.dexterity_min + 10;
			if (shop_stats.dexterity_min < 3) {
				int shift = 3 - shop_stats.dexterity_min;
				shop_stats.dexterity_min + shift;
				shop_stats.dexterity_max + shift;
			}
			if (shop_stats.dexterity_max > 18) {
				int shift = shop_stats.dexterity_max - 18;
				shop_stats.dexterity_max - shift;
				shop_stats.dexterity_min - shift;
			}


			shop_stats.intelligence_min = get_random(real.intelligence - 5, real.intelligence + 5);
			shop_stats.intelligence_max = shop_stats.intelligence_min + 10;
			if (shop_stats.intelligence_min < 3) {
				int shift = 3 - shop_stats.intelligence_min;
				shop_stats.intelligence_min + shift;
				shop_stats.intelligence_max + shift;
			}
			if (shop_stats.intelligence_max > 18) {
				int shift = shop_stats.intelligence_max - 18;
				shop_stats.intelligence_max - shift;
				shop_stats.intelligence_min - shift;
			}

			shop_stats.health_min = get_random(real.health - 5, real.health + 5);
			shop_stats.health_max = shop_stats.health_min + 10;
			if (shop_stats.health_min < 3) {
				int shift = 3 - shop_stats.health_min;
				shop_stats.health_min + shift;
				shop_stats.health_max + shift;
			}
			if (shop_stats.health_max > 18) {
				int shift = shop_stats.health_max - 18;
				shop_stats.health_max - shift;
				shop_stats.health_min - shift;
			}

			shop_stats.hp_max = shop_stats.hp_min = 0;
			shop_stats.dodge_max = shop_stats.dodge_min = 0;
			shop_stats.skills_total_max = shop_stats.skills_total_min = 0;
			shop_stats.fatigue_max = shop_stats.fatigue_min = 0;
			shop_stats.basic_speed_max = shop_stats.basic_speed_min = 0;
			shop_stats.basic_lift_max = shop_stats.basic_lift_min = 0;
			shop_stats.willpower_max = shop_stats.willpower_min = 0;
			shop_stats.perception_max = shop_stats.perception_min = 0;
			shop_stats.basic_strike_max = shop_stats.basic_strike_min = 0;
			shop_stats.basic_combat_max = shop_stats.basic_combat_min = 0;
		}
	}

	int get_gold() { return mgold; }
	int get_reputation() { return mreputation; }
	player_stats get_player_stats() { return mplayer_stat; }
	std::vector<Gladiator>& get_slaves_in_market() { return mslave_market; }
	int get_market_size() { return mslave_market.size(); }
	Gladiator* get_gladiator_in_market(int index) { return &mslave_market[index]; }
};

class Gamewindow {
private:
	SDL_Window* pwindow;
	SDL_Renderer* prenderer;
	TTF_Font* pfont;
	float mtexture_scale;

	std::unordered_map<int, Ltexture*> loaded_textures;
	std::unordered_map<int, Weapon> weapon_list;
	std::unordered_map<int, Armour> armour_list;

public:
	Gamewindow() {
		pwindow = nullptr;
		prenderer = nullptr;
		mtexture_scale = 0;
		loaded_textures = {};
	}

	~Gamewindow() {
		delete_all_textures();
		TTF_CloseFont(pfont);
		pfont = nullptr;
		SDL_DestroyWindow(pwindow);
		pwindow = nullptr;
		SDL_DestroyRenderer(prenderer);
		prenderer = nullptr;
		mtexture_scale = 0;
		loaded_textures.clear();

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	bool init() {
		bool success = true;
		srand(time(NULL));
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("Unable to initialize SDL! SDL Error: %s\n", SDL_GetError());
		}
		else {
			pwindow = SDL_CreateWindow("Blood and Glory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
			if (pwindow == nullptr) {
				printf("Unable to create window. SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (prenderer == nullptr) {
					printf("Unable to create renderer. SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else {
					SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 0xFF);
					Uint8 img_flags = IMG_INIT_PNG;
					if ((IMG_Init(img_flags) & img_flags) != img_flags) {
						printf("Unable to initialize SDL_image. SDL Error: %s\n", SDL_GetError());
						success = false;
					}
					else {
						SDL_DisplayMode desktop_resolution;
						if (SDL_GetDesktopDisplayMode(0, &desktop_resolution) < 0) {
							printf("Unable to get desktop resolution! SDL Error: %s\n", SDL_GetError());
							success = false;
						}
						else {
							SDL_RenderSetLogicalSize(prenderer, 1920, 1080);
						}
					}
					if (TTF_Init() == -1) {
						printf("TTF could not initialize! SDL Error: %s\n", TTF_GetError());
						success = false;
					}
					else {
						pfont = TTF_OpenFont("Assets/font/OpenSans.ttf", 18);
						if (pfont == nullptr) {
							printf("Unable to load Baskervvile-Regular font!\n");
							success = false;
						}
					}
				}
			}
		}
		return success;
	}

	bool load_texture(int texture_type, int texture_id, std::string path) {
		if (loaded_textures.find(texture_id) == loaded_textures.end()) {
			SDL_Texture* new_texture = nullptr;
			SDL_Surface* loaded_surface = IMG_Load(path.c_str());
			if (loaded_surface == nullptr) {
				printf("Unable to create surface from %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
			}
			else {
				new_texture = SDL_CreateTextureFromSurface(prenderer, loaded_surface);
				if (new_texture == nullptr) {
					printf("Unable to create texture from surface %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
				}
				else {
					Ltexture* new_Ltexture = new Ltexture(new_texture, loaded_surface->w, loaded_surface->h);
					new_Ltexture->set_default_rects(texture_type);
					loaded_textures[texture_id] = new_Ltexture;
				}
				SDL_FreeSurface(loaded_surface);
			}
		}
		
		return loaded_textures.count(texture_id);
	}

	void create_temp_texture(int texture_id, Ltexture* temp_texture) {
		loaded_textures[texture_id] = temp_texture;
	}

	bool create_single_line_text(std::string text, SDL_Rect render_pos, int texture_id, int font_size, SDL_Color text_color = { 0, 0, 0 }) {
		TTF_SetFontSize(pfont, font_size);

		SDL_Texture* new_texture = nullptr;
		SDL_Surface* temp_text_surface = TTF_RenderUTF8_Solid(pfont, text.c_str(), text_color);
		new_texture = SDL_CreateTextureFromSurface(prenderer, temp_text_surface);
		Ltexture* new_text_Ltexture = new Ltexture(new_texture, temp_text_surface->w, temp_text_surface->h);


		new_text_Ltexture->set_render_pos({render_pos.x, render_pos.y, temp_text_surface->w, temp_text_surface->h});
		new_text_Ltexture->set_clip({ 0, 0, temp_text_surface->w, temp_text_surface->h });
		loaded_textures[texture_id] = new_text_Ltexture;

		SDL_FreeSurface(temp_text_surface);

		return new_texture != nullptr;
	}

	void create_ui_text(Gamestate &current_game) {
		create_single_line_text("Gold : " + std::to_string(current_game.get_gold()), { 200, 920, 0, 0 }, ui_gold_id, 100);
		create_single_line_text("Reputation : " + std::to_string(current_game.get_reputation()), { 1000, 920, 0, 0 }, ui_reputation_id, 100);
	}

	bool clickable_texture(int texture_id) {
		SDL_Point mouse_pos = { 0, 0 };
		get_mouse_pos(&mouse_pos.x, &mouse_pos.y);
		return on_texture(mouse_pos, loaded_textures[texture_id]->copy_pos());
	}

	void render_textures(std::vector<int> render_target) {
		// get mouse position because we need it for hover textures
		SDL_Point mouse_pos = { 0, 0 };
		get_mouse_pos(&mouse_pos.x, &mouse_pos.y);
		// textures render in order sent
		SDL_RenderClear(prenderer);
		for (int i = 0; i < render_target.size(); i++) {
			// rendering animations
			if (loaded_textures[render_target[i]]->is_anim()) {
				// rendering on/off textures
				if (loaded_textures[render_target[i]]->get_anim_specs().on_off) {
					if (on_texture(mouse_pos, loaded_textures[render_target[i]]->copy_pos())) {
						loaded_textures[render_target[i]]->get_next_clip();
					}
					else {
						loaded_textures[render_target[i]]->get_previous_clip();
					}
					SDL_RenderCopy(prenderer, loaded_textures[render_target[i]]->get_texture(), loaded_textures[render_target[i]]->get_clip(), loaded_textures[render_target[i]]->get_pos());
				}
			}
			// if we scroll too far up with the texture and no longer want to render part of it
			else if (loaded_textures[render_target[i]]->get_scroll_range()->x > loaded_textures[render_target[i]]->get_pos()->y) {
				int scroll_difference = loaded_textures[render_target[i]]->get_scroll_range()->x - loaded_textures[render_target[i]]->get_pos()->y;
				SDL_Rect current_clip = loaded_textures[render_target[i]]->copy_clip();
				SDL_Rect current_pos = loaded_textures[render_target[i]]->copy_pos();
				
				// the clip y and h need to be changed (y influences the top of the render and h represents the height)
				current_clip.y += scroll_difference;
				current_clip.h -= scroll_difference;

				// the render_pos needs to be capped at the scroll_min and height needs adjusted based on it
				current_pos.y = loaded_textures[render_target[i]]->get_scroll_range()->x;
				current_pos.h -= scroll_difference;

				SDL_RenderCopy(prenderer, loaded_textures[render_target[i]]->get_texture(), &current_clip, &current_pos);
			}
			// if we scroll too far down
			else if (loaded_textures[render_target[i]]->get_scroll_range()->y < (loaded_textures[render_target[i]]->get_pos()->y + loaded_textures[render_target[i]]->get_pos()->h)) {
				int scroll_difference = (loaded_textures[render_target[i]]->get_pos()->y + loaded_textures[render_target[i]]->get_pos()->h) - loaded_textures[render_target[i]]->get_scroll_range()->y;
				SDL_Rect current_clip = loaded_textures[render_target[i]]->copy_clip();
				SDL_Rect current_pos = loaded_textures[render_target[i]]->copy_pos();

				current_clip.h -= scroll_difference;
				current_pos.h -= scroll_difference;

				SDL_RenderCopy(prenderer, loaded_textures[render_target[i]]->get_texture(), &current_clip, &current_pos);
			}
			else
				SDL_RenderCopy(prenderer, loaded_textures[render_target[i]]->get_texture(), loaded_textures[render_target[i]]->get_clip(), loaded_textures[render_target[i]]->get_pos());
		}
		SDL_RenderPresent(prenderer);
	}

	void delete_texture(std::vector<int> del_textures) {
		for (int i = 0; i < del_textures.size(); i++) {
			if (loaded_textures.count(del_textures[i])) {
				loaded_textures[del_textures[i]]->~Ltexture();
				loaded_textures.erase(del_textures[i]);
			}
		}
	}

	void delete_all_textures() {
		for (int i = 0; i < texture_id_total; i++) {
			if (loaded_textures.count(i)) {
				loaded_textures[i]->~Ltexture();
				loaded_textures.erase(i);
			}
		}
	}

	void load_all_weapons() {
		weapon_list[no_weapon] = Weapon(no_weapon, "Fists", 0, 3, 1.0, 0.0, 0.0);
		weapon_list[gladius] = Weapon(gladius, "Gladius", 3, 7, 0.5, 1.5, 2.0);
		weapon_list[old_gladius] = Weapon(old_gladius, "Old Gladius", 2, 5, 0.5, 1.25, 1.75);
		weapon_list[fascina] = Weapon(fascina, "Fascina", 2, 10, 0.75, 0.5, 2.5);
	}

	void load_all_armour() {
		armour_list[no_armour] = Armour(no_armour, "Bare", any_location, 0);
		armour_list[galea] = Armour(galea, "Galea", head, 5);
		armour_list[corium_ocrea] = Armour(corium_ocrea, "Corium Ocrea", shin, 2);
		armour_list[lorica_corium] = Armour(lorica_corium, "Lorica Corium", body, 3);
		armour_list[pteruges_corium] = Armour(pteruges_corium, "Pteruges Corium", upper_legs, 2);
		armour_list[corium_manica] = Armour(corium_manica, "Corium Manica", forearm, 2);
		armour_list[corium_scapula] = Armour(corium_scapula, "Corium Scapula", bicept, 2);
	}

	void get_mouse_pos(int* logicalMouseX, int* logicalMouseY) {
		int realMouseX, realMouseY;
		SDL_GetMouseState(&realMouseX, &realMouseY);
		int wWidth, wHeight;
		int rLogicalWidth, rLogicalHeight;
		int rRealWidth, rRealHeight;
		float rScaleX, rScaleY;
		int rMidpointY, wMidpointY;
		int rMidpointX, wMidpointX;
		int rY, rX;

		SDL_GetWindowSize(pwindow, &wWidth, &wHeight);
		wMidpointY = wHeight / 2;
		wMidpointX = wWidth / 2;

		SDL_RenderGetLogicalSize(prenderer, &rLogicalWidth, &rLogicalHeight);
		SDL_RenderGetScale(prenderer, &rScaleX, &rScaleY);
		rRealWidth = (float)rLogicalWidth * (float)rScaleX;
		rRealHeight = (float)rLogicalHeight * (float)rScaleY;
		rMidpointY = rRealHeight / 2;
		rMidpointX = rRealWidth / 2;
		rY = wMidpointY - rMidpointY;
		rX = wMidpointX - rMidpointX;

		int adjustedMouseY = realMouseY - rY;
		int adjustedMouseX = realMouseX - rX;
		*logicalMouseX = (float)adjustedMouseX / (float)rRealWidth * (float)rLogicalWidth;
		*logicalMouseY = (float)adjustedMouseY / (float)rRealHeight * (float)rLogicalHeight;

	}

	void scroll_textures(int distance, float box_scale, int border_padding, std::vector<int> textures, bool scroll_wheel = true) {
		int scroll_speed, box_scroll_speed, texture_pos;
		int box_top_current = loaded_textures[scroll_wheel_box_id]->get_pos()->y;
		int box_bottom_current = (loaded_textures[scroll_wheel_box_id]->get_pos()->y + loaded_textures[scroll_wheel_box_id]->get_pos()->h);
		int box_top_max = loaded_textures[scroll_wheel_border_id]->get_pos()->y + border_padding;
		int box_bottom_max = (loaded_textures[scroll_wheel_border_id]->get_pos()->y + loaded_textures[scroll_wheel_border_id]->get_pos()->h) - border_padding;
		
		int textures_top = loaded_textures[textures[0]]->get_pos()->y;
		int textures_bottom = (loaded_textures[textures[textures.size() - 1]]->get_pos()->y + loaded_textures[textures[textures.size() - 1]]->get_pos()->h);
		int textures_size = textures_bottom - textures_top;

		// first get the box scroll distance
		if (scroll_wheel) {
			box_scroll_speed = distance * 15;
		}
		else {
			box_scroll_speed = distance;
		}

		// scroll the box depending on how much space / how far the scroll is
		if ((distance < 0 && box_bottom_current - box_scroll_speed <= box_bottom_max) ||
			(distance > 0 && box_top_current - box_scroll_speed >= box_top_max)) {
			if ((distance < 0 && box_bottom_current < box_bottom_max) || (distance > 0 && box_top_current > box_top_max)) {
				loaded_textures[scroll_wheel_box_id]->get_pos()->y -= box_scroll_speed;
			}
		}
		else if (box_bottom_current < box_bottom_max && box_bottom_current - box_scroll_speed > box_bottom_max) {
			int box_space_left = box_bottom_max - box_bottom_current;
			loaded_textures[scroll_wheel_box_id]->get_pos()->y += box_space_left;
		}
		else if (box_top_current > box_top_max && box_top_current - box_scroll_speed < box_top_max) {
			int box_space_left = box_top_current - box_top_max;
			loaded_textures[scroll_wheel_box_id]->get_pos()->y -= box_space_left;
		}

		// need to figure out where the box is now
		int box_new_pos = loaded_textures[scroll_wheel_box_id]->get_pos()->y;

		// figure out how far down we have scrolled
		texture_pos = (box_new_pos - box_top_current) / box_scale;

		// scroll the textures to where they need to be
		for (int i = 0; i < textures.size(); i++) {
			loaded_textures[textures[i]]->get_pos()->y -= texture_pos;
		}
	}

	Ltexture* get_texture(int texture_id) { return loaded_textures[texture_id]; }
	Weapon* get_weapon(int weapon_id) { return &weapon_list[weapon_id]; }
	Armour* get_armour(int armour_id) { return &armour_list[armour_id]; }
};

bool on_texture(SDL_Point mouse_position, SDL_Rect texture_pos) {
	bool is_in = true;
	if (mouse_position.x < texture_pos.x)
		is_in = false;
	else if (mouse_position.x > texture_pos.x + texture_pos.w)
		is_in = false;
	else if (mouse_position.y < texture_pos.y)
		is_in = false;
	else if (mouse_position.y > texture_pos.y + texture_pos.h)
		is_in = false;

	return is_in;
}
gladiator_stats create_random_gladiator_stats(int type) {
	int str, dex, intel, ht, blift, hp, bspeed, dodge, will, per, skills, fatigue;
	int bstrike = 0, bcombat = 0;

	if (get_random(1, 20) < 5) {
		bstrike++;
		bcombat++;
	}
	if (get_random(1, 20) < 3) {
		bstrike++;
	}
	if (get_random(1, 20) < 3) {
		bcombat++;
	}
	switch (type) {
	case slave:
		str = 3 + get_random(0, 8);
		dex = 3 + get_random(0, 8);
		intel = 3 + get_random(0, 8);
		ht = 3 + get_random(0, 8);
		bcombat += get_random(0, 1);

	case criminal:
		str = 5 + get_random(0, 7);
		dex = 4 + get_random(0, 7);
		intel = 3 + get_random(0, 6);
		ht = 4 + get_random(0, 8);
		bcombat += get_random(0, 1);

	case freeman:
		str = 7 + get_random(0, 6);
		dex = 5 + get_random(0, 7);
		intel = 4 + get_random(0, 7);
		ht = 6 + get_random(0, 9);
		bstrike += get_random(0, 1);
		bcombat += get_random(0, 1) + get_random(0, 1);

	case aristocrat:
		str = 6 + get_random(0, 6);
		dex = 6 + get_random(0, 6);
		intel = 7 + get_random(0, 8);
		ht = 8 + get_random(0, 8);
		bstrike += get_random(0, 1);
		bcombat += get_random(0, 1) + get_random(0, 1);

	case gladiator:
		str = 7 + get_random(0, 8);
		dex = 7 + get_random(0, 8);
		intel = 4 + get_random(0, 8);
		ht = 10 + get_random(0, 5);
		bstrike += get_random(0, 1) + get_random(0, 1);
		bcombat += get_random(0, 1) + get_random(0, 1) + get_random(0, 1);
	}

	blift = ((str * str) / 5);
	hp = (str * 7) * (ht * 4);
	bspeed = (ht + dex) / 4;
	dodge = bspeed + 3;
	will = intel;
	per = intel;
	skills = intel / 4;
	fatigue = ht;

	return { str, dex, intel, ht, blift, bcombat, hp, bspeed, bcombat, dodge, will, per, skills, fatigue };
}
std::string random_name(char gender) {
	std::vector<std::string> names;
	if (gender == 'M') {
		names = { "Marcipor", "Scipio", "Virgil", "Hector", "Marcus", "Scipor", "Quintus", "Vopiscus",
			"Vopispor", "Caelus", "Caepor", "Manius", "Opiter", "Decius", "Decipor", "Vibious", "Vibipor", "Gallio",
			"Gallipor", "Publius", "Tullus", "Tullipor", "Tiberious", "Amulius", "Amulipor" };
	}
	else if (gender == 'F') {
		names = { "Aurelia", "Galeria", "Tertinia", "Attia", "Vibidia", "Mucia", "Favonia", "Canidia",
			"Uulia", "Aemilia" };
	}

	return names[get_random(0, names.size() - 1)];
}
int get_random(int min, int max) {
	return (rand() % (max - min + 1) + min);
}
int die_roll() {
	int roll = 0;
	roll += rand() % (6 - 1 + 1) + 1;
	roll += rand() % (6 - 1 + 1) + 1;
	roll += rand() % (6 - 1 + 1) + 1;
	return roll;
}