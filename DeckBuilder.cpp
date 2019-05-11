#include "DeckBuilder.h"
#include <iostream>


DeckBuilder::DeckBuilder(sf::RenderWindow& w) : window(w) {
	deck_builder_texture.loadFromFile("textures/db.png");
	deck_builder_sprite.setTexture(deck_builder_texture);
	
	deck_builder_texture_saveHL.loadFromFile("textures/db_shl.png");
	deck_builder_texture_cancelHL.loadFromFile("textures/db_chl.png");
	
	deck_builder_sprite.setScale(600/deck_builder_sprite.getLocalBounds().width, 500/deck_builder_sprite.getLocalBounds().height);
	
	saved_texture.loadFromFile("textures/saved_pop.png");
	notcompl_texture.loadFromFile("textures/notcompl_pop.png");
	
	popUp_spr.setPosition(188, 218);
	
	// Text
	font.loadFromFile("textures/Old_R.ttf");
	card_number.setFont(font);
	card_number.setCharacterSize(10);
	
	card_texture.loadFromFile("textures/cardsFinal.tga");
	// Y + 69
	right_side = {sf::Vector2f(430, 75), sf::Vector2f(490, 75), 
				  sf::Vector2f(430, 145), sf::Vector2f(490, 145),
				  sf::Vector2f(430, 215), sf::Vector2f(490, 215),
				  sf::Vector2f(430, 285), sf::Vector2f(490, 285),
				  sf::Vector2f(430, 355), sf::Vector2f(490, 355)};
	// xt - xc = 445-430 = 15; yt - yc = 94 - 75 = 19;
	text_on_right_side = {sf::Vector2f(445, 97), sf::Vector2f(505, 97), 
						  sf::Vector2f(445, 167), sf::Vector2f(505, 167),
						  sf::Vector2f(445, 237), sf::Vector2f(505, 237),
						  sf::Vector2f(445, 307), sf::Vector2f(505, 307),
						  sf::Vector2f(445, 377), sf::Vector2f(505, 377)};
	// Put cards on screen
	
	// TO-DO Add text (numbers on cards)
	sf::Sprite card;
	sf::Vector2f pos (55, 45);
	card.setTexture(card_texture);
	
	//hand_card.setOrigin(46/2, 64/2);
	for(int i = 0; i < 18; ++i){
		if(i < 6){
			card.setTextureRect(sf::IntRect(97, 0, 46, 64));
			card.setPosition(pos);
			card_number.setString(str_card_numbers[i]);
			card_number.setPosition(pos.x + 16, pos.y + 21);
			text_on_cards[i] = card_number;
			pos.y += 68;
			available_cards.push_back(card);
			continue;
		}
		if(i >= 6 && i < 12){
			card.setTextureRect(sf::IntRect(50, 0, 46, 64));
			pos.x = 120;
			//card.setPosition(pos.x, pos.y - available_cards[i-1].getPosition().y);
			pos.y -= 68;
			card.setPosition(pos.x, pos.y);
			card_number.setString(str_card_numbers[i]);
			card_number.setPosition(pos.x + 13, pos.y + 21);
			text_on_cards[i] = card_number;
			available_cards.push_back(card);
			continue;
		}
		if(i >= 12){
			card.setTextureRect(sf::IntRect(146, 0, 46, 64));
			pos.x = 185;
			card.setPosition(pos);
			card_number.setString(str_card_numbers[i]);
			card_number.setPosition(pos.x + 5, pos.y + 21);
			text_on_cards[i] = card_number;
			pos.y += 68;
			available_cards.push_back(card);
			continue;
		}
	}
	
}



void DeckBuilder::SaveDeck()
{
	std::ofstream deck_file;
	deck_file.open("deck");
	
	for(size_t i = 0; i < 10; ++i){
		if(i == 9){
			deck_file << deck[i];
		} else {
			deck_file << deck[i] << " ";			
		}
	}
}

void DeckBuilder::AddToDeck(size_t card_index)
{
	// Move card to the right side of the screen, add it to deck array
	if(right_side_sprites.size() < 10){
		right_side_sprites.push_back(available_cards[card_index]);
		
		right_side_sprites.back().setPosition(right_side[right_side_sprites.size()-1]);
		text_on_cards_right_side[right_side_sprites.size()-1] = text_on_cards[card_index];
		text_on_cards_right_side[right_side_sprites.size()-1].setPosition(text_on_right_side[right_side_sprites.size()-1]);
		
		deck[right_side_sprites.size()-1] = card_numbers[card_index];		
	}
}

void DeckBuilder::Update()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		button_released = true;
	}
	
	sf::Vector2f mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	
	if(save_b.btn_shape.getGlobalBounds().contains(mp)){
		// HIGHLIGHT
		deck_builder_sprite.setTexture(deck_builder_texture_saveHL);
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && button_released){
			b_popUp = true;
			if(right_side_sprites.size() < 10){
				popUp_spr.setTexture(notcompl_texture);
				return;
			} else {
				popUp_spr.setTexture(saved_texture);
			}
			SaveDeck();
		}
	}
	if(cancel_b.btn_shape.getGlobalBounds().contains(mp)){
		// HIGHLIGHT
		deck_builder_sprite.setTexture(deck_builder_texture_cancelHL);
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && button_released){
			cancel = true;
		}
	}
	
	mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	
	for(size_t i = 0; i < available_cards.size(); ++i){
		if(available_cards[i].getGlobalBounds().contains(mp)){
			// TO-DO HIGHLIGHT THE CARD
			//
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && button_released){
				AddToDeck(i);
				button_released = false;
			}
		}
	}
	
	if(popUp_spr.getGlobalBounds().contains(mp)){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && button_released){
			// Means we are showing Saved! message
			if(right_side_sprites.size() == 10){
				saved = true;
			}
			b_popUp = false;
			button_released = false;
		}
	}
}

void DeckBuilder::Render()
{

	window.draw(deck_builder_sprite);
	for(unsigned int i = 0; i < available_cards.size(); ++i){
		window.draw(available_cards[i]);
	}
	for(size_t i = 0; i < right_side_sprites.size(); ++i){
		window.draw(right_side_sprites[i]);
	}
	for(size_t i = 0; i < text_on_cards.size(); ++i){
		window.draw(text_on_cards[i]);
	}
	for(size_t i = 0; i < right_side_sprites.size(); ++i){
		window.draw(text_on_cards_right_side[i]);
	}
	if(b_popUp){
		window.draw(popUp_spr);
	}
	window.display();
}

void DeckBuilder::Reset()
{
	saved = false;
	cancel = false;
	right_side_sprites.clear();

	deck = {0};
	
}
