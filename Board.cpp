#include "Board.h"

Board::Board()
{
	board_left = {sf::Vector2f(137, 155), sf::Vector2f(187, 155), sf::Vector2f(237, 155),
				  sf::Vector2f(137, 226), sf::Vector2f(187, 226), sf::Vector2f(237, 226),
				  sf::Vector2f(137, 297), sf::Vector2f(187, 297), sf::Vector2f(237, 297)
	};
	
	board_right = {sf::Vector2f(362, 155), sf::Vector2f(412, 155), sf::Vector2f(462, 155),
				   sf::Vector2f(362, 226), sf::Vector2f(412, 226), sf::Vector2f(462, 226),
				   sf::Vector2f(362, 297), sf::Vector2f(412, 297), sf::Vector2f(462, 297)
	};
	
	numbers_left = {sf::Vector2f(131, 146), sf::Vector2f(181, 146), sf::Vector2f(231, 146),
					sf::Vector2f(131, 217), sf::Vector2f(181, 217), sf::Vector2f(231, 217),
					sf::Vector2f(131, 288), sf::Vector2f(181, 288), sf::Vector2f(231, 288)
	};
	numbers_right = {sf::Vector2f(358, 146), sf::Vector2f(408, 146), sf::Vector2f(458, 146),
					sf::Vector2f(358, 217), sf::Vector2f(408, 217), sf::Vector2f(458, 217),
					sf::Vector2f(358, 288), sf::Vector2f(408, 288), sf::Vector2f(458, 288)
	};
	
	hand_left = {sf::Vector2f(99, 388), sf::Vector2f(150, 388), sf::Vector2f(201, 388),
						 sf::Vector2f(252, 388)
	};
	
	hand_right = {sf::Vector2f(345, 388), sf::Vector2f(396, 388), sf::Vector2f(447, 388),
						  sf::Vector2f(498, 388)
	};
	
	numbers_hand_left = {sf::Vector2f(94, 379), sf::Vector2f(145, 379), sf::Vector2f(196, 379),
						 sf::Vector2f(247, 379)
	};
	
	backGround_texture.loadFromFile("textures/lbl_pazaak.tga");
	texture_turn_light.loadFromFile("textures/turn_light.tga");
	//backGround_texture.loadFromFile("textures/800x600pazaak.tga");
	backGround.setTexture(backGround_texture);
	backGround.setScale(600/backGround.getLocalBounds().width, 500/backGround.getLocalBounds().height);
	
	texture_turn_light.loadFromFile("textures/turn_light.tga");
	spr_turn_light.setTexture(texture_turn_light);
	
	turn_ligths_pos[0] = sf::Vector2f(70, 57);
	turn_ligths_pos[1] = sf::Vector2f(500, 57);
	spr_turn_light.setPosition(turn_ligths_pos[0]);
	
	card_texture.loadFromFile("textures/cardsFinal.tga");
	
	font.loadFromFile("textures/Old_R.ttf");
	card_number.setFont(font);
	player_name.setFont(font);
	player_one_score.setFont(font);
	player_two_score.setFont(font);
	
	card_number.setCharacterSize(10);
	player_name.setCharacterSize(10);
	
	player_one_score.setCharacterSize(10);
	player_two_score.setCharacterSize(10);
	
	player_one_score.setPosition(250, 95);
	player_two_score.setPosition(340, 95);
	
	// We set the default string for player two score bcs he goes second, hence his score is updated later
	player_two_score.setString("0");
	
	// Buttons
	end_turn_btn.setFillColor(sf::Color::Black);
	end_turn_btn.setSize(sf::Vector2f(105, 24));
	end_turn_btn.setPosition(299, 431);
	
	end_turn_text.setFont(font);
	end_turn_text.setColor(sf::Color::Cyan);
	end_turn_text.setString("End turn");
	end_turn_text.setCharacterSize(10);
	end_turn_text.setOrigin(end_turn_text.getLocalBounds().width/2, end_turn_text.getLocalBounds().height/2);
	end_turn_text.setPosition(sf::Vector2f(351.5, 443));
	
	
	stand_btn.setFillColor(sf::Color::Black);
	stand_btn.setSize(sf::Vector2f(105, 24));
	stand_btn.setPosition(412, 431);
	
	stand_text.setFont(font);
	stand_text.setColor(sf::Color::Cyan);
	stand_text.setCharacterSize(10);
	stand_text.setString("Stand");
	stand_text.setOrigin(stand_text.getLocalBounds().width/2, stand_text.getLocalBounds().height/2);
	stand_text.setPosition(sf::Vector2f(464.5, 443));
	
}

