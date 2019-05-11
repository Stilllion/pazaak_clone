#pragma once
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>

#define mouse_position static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))

// Buttons
struct cancel_button{
	sf::RectangleShape btn_shape;
	
	cancel_button(){
		btn_shape.setSize(sf::Vector2f(125, 20));
		btn_shape.setPosition(sf::Vector2f(97, 470));
	}
};

struct save_button{
	sf::RectangleShape btn_shape;
	
	save_button(){
		btn_shape.setSize(sf::Vector2f(125, 20));
		btn_shape.setPosition(sf::Vector2f(287, 470));
	}
};

struct DeckBuilder{
	
	sf::RenderWindow& window;
	
	sf::Texture deck_builder_texture;
	sf::Sprite deck_builder_sprite;
	
	sf::Texture card_texture;
	sf::Texture deck_builder_texture_saveHL;
	sf::Texture deck_builder_texture_cancelHL;
	
	// Pop-up messages
	sf::Texture saved_texture;
	sf::Texture notcompl_texture;
	sf::Sprite popUp_spr;
	bool b_popUp = false;
	
	sf::Font font;
	
	sf::Text card_number;
	
	std::vector<sf::Sprite> available_cards;
	std::array<int, 10> deck;
	std::vector<sf::Sprite> right_side_sprites;
	std::array<sf::Vector2f, 10> right_side;
	std::array<int, 18> card_numbers = {1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6, 11, 22, 33, 44, 55, 66};
	std::array<std::string, 18> str_card_numbers = {"1", "2", "3", "4", "5", "6", "-1", "-2", "-3", "-4", "-5", "-6", "+-1", "+-2", "+-3", "+-4", "+-5", "+-6"};
	
	std::array<sf::Text, 18> text_on_cards;
	std::array<sf::Vector2f, 10> text_on_right_side;
	std::array<sf::Text, 10> text_on_cards_right_side;
	
	void HandleInput();
	void Update();
	void Render();
	
	void AddToDeck(size_t card_index);
	void SaveDeck();
	void Reset();
	bool button_released = false;
	// Player exits
	bool cancel = false;
	bool saved = false;
	
	DeckBuilder(sf::RenderWindow& w);
	
	save_button save_b;
	cancel_button cancel_b;
};