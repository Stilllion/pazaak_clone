#ifndef Board_h
#define Board_h
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <array>

struct Board
{
	Board();
	// Fonts, textures etc.
	sf::Font font;
	sf::Texture backGround_texture;
	sf::Texture deck_builder_texture;
	sf::Texture card_texture;
	sf::Text card_number;
	sf::Text player_name;
	sf::Text player_one_score;
	sf::Text player_two_score;
	sf::Sprite backGround;
	sf::Sprite deck_builder_sprite;
	//sf::Sprite card; Created at Init();
	// !!!!!!!!!! sprite.setTextureRect(sf::IntRect(10, 10, 32, 32)); !!!!!!!!!!!!!
	// USE THIS TO LOAD A NEEDED CARD, there is no need to have 100500 textures for that
	
	

	std::vector<sf::Text> draw_card_text_left;	
	std::vector<sf::Text> draw_card_text_right;
	
	std::vector<sf::Text> hand_card_text;
	
	// Slots positions will stored here
	std::array<sf::Vector2f, 9> board_left;
	std::array<sf::Vector2f, 9> board_right;
	// Card Number positions stored here
	std::array<sf::Vector2f, 9> numbers_left;
	std::array<sf::Vector2f, 9> numbers_right;
	// Turn light positions
		//sf::Vector2f turn_light_left(x, y);
		//sf::Vector2f turn_light_right(x, y);
	// Player hand card positions
	std::array<sf::Vector2f, 4> hand_left;
	std::array<sf::Vector2f, 4> hand_right;
	// Hand card number positions
	std::array<sf::Vector2f, 4> numbers_hand_left;
	//std::array<sf::Vector2f, 4> numbers_hand_right;
	
	
	// Buttons
	sf::RectangleShape end_turn_btn;
	sf::RectangleShape stand_btn;
	sf::Text end_turn_text;
	sf::Text stand_text;
	
};

#endif /* Board_h */