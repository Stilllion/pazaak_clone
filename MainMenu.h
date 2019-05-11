#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "states.h"
#include <SFML/Audio.hpp>
#include "network.h"
#define mouse_position static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))

struct MainMenu{
	
	enum SubMenus {main_menu, host_game, join_game, options, deck_builder, quit};
	
	UDP& udp_net;
	TCP& tcp_net;
	sf::IpAddress server_adress;
	bool start_game = false;
	
	// To see if player has set up his deck. If not, he can't start the game
	bool deck_ready = false;
	
	struct Button{
		sf::RectangleShape btn_shape;
		SubMenus subm;
		SubMenus OnClick(){return subm;};
		
		Button(sf::Vector2f size, sf::Vector2f pos, SubMenus sm) {
			subm = sm;
			btn_shape.setSize(size);
			btn_shape.setPosition(pos);
		}
		Button() {};
	};
	

	SubMenus curr_subMenu;
	
	std::vector<Button> buttons_MM = {};
	std::vector<Button> buttons_HJG = {};
	//std::vector<Button> buttons_JG = {};	
	std::vector<Button> buttons_OPT = {};

	std::vector<Button> curr_subm_buttons = {};
	
	sf::Texture texture_bg;
	sf::Texture texture_hjgame_waiting;
	sf::Texture texture_hjgame_waiting_hl;
	
	sf::Texture  texture_bg_btn_JGhl;
	sf::Texture  texture_bg_btn_HGhl;
	sf::Texture  texture_bg_btn_OPThl;
	sf::Texture  texture_bg_btn_QThl;
	
	
	std::vector<sf::Texture > MM_textures;
	std::vector<sf::Texture > OPT_textures;
	std::vector<sf::Texture > HJG_textures;
	std::vector<sf::Texture > JG_textures;
	// and so on...
	std::vector<sf::Texture> curr_subm_textures;
	
	sf::Sprite spr_backGround;
	
	void HandleInput();
	void Update();
	void Render();
	bool Is_deck_ready();
	
	// Pop-up messages
	sf::Texture warning_texture;
	sf::Sprite popUp;
	bool b_popUp = false;
	
	bool btn_was_released = true;
	
	sf::SoundBuffer sound_buffer;
	sf::Sound scroll_sound;
	
	sf::Clock clock;
	sf::Socket::Status server_status;
	
	sf::RenderWindow& window;
	MainMenu(sf::RenderWindow& w, UDP& udp_net, TCP& tcp_net);
};

