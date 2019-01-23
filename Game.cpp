#include "Game.h"

struct Button{
	sf::Vector2f position;
	sf::RectangleShape rect;
	bool was_released = true;
	Button(float x, float y){
		position.x = x;
		position.y = y;
		rect.setPosition(position);
		rect.setFillColor(sf::Color(0, 120, 255));
		rect.setSize(sf::Vector2f(105, 50));
	} 
	void onClick();
};

Button host_game(250.0, 200.0);
Button join_game(250.0, 260.0);

Game::Game() : dist(1, 10)
{
	p2.hand[0] = 5;
	p2.hand[1] = 5;
	p2.hand[2] = 5;
	p2.hand[3] = 5;
	
	
	window.create(sf::VideoMode(600, 500), "Pazaak");
	srand(time(0));
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(15);

	SetHiddenHand();
	SetPlayerOneHand();
	// INDEV TOOLS
	
	info.setFont(board.font);
	info.setCharacterSize(10);
	info.setPosition(100, 100);
	
	// Random number seed
	mt.seed(std::time(0));
	clock.restart();
}

void Game::On_receive(){
	int rndn = dist(mt);
	board.stand_btn.setPosition(rndn, rndn);
}

void Game::HandleAIinput()
{
	if(state_changed == true){
		btn_was_released = true;
		board.player_two_score.setString(std::to_string(p2.score));
		current_state = States::player_one_turn;
		return;
	}
	
	/*for(int i = 0; i < 4; ++i){
		if(AI_hand[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
			// if y == 388 then card is still in hand and not on board/
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && AI_hand[i].getPosition().y == 388){
				ConvertBackToFace(AI_hand[i], i);
				AI_hand[i] = empty_spr;
				p2.score += p2.hand[i];*/
				/*board.player_two_score.setString(std::to_string(p2.score));

				if(!p1.standing){
					current_state = States::player_one_turn;
					state_changed = true;
				}
				card_was_drawn = false;
				
				return;
			}
		}		
	}*/
	
	/*if(board.end_turn_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			if(!p1.standing){
				current_state = States::player_one_turn;					
			}
			card_was_drawn = false;
			btn_was_released = false;

			return;
		}
	}
	
	if(board.stand_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)  && btn_was_released){
			p2.standing = true;
			if(!p1.standing){
				current_state = States::player_one_turn;					
			}
			card_was_drawn = false;
			btn_was_released = false;

			return;
		}
	}*/
}

void Game::HandleUserInput()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			btn_was_released = true;
	}
	
	for(int i = 0; i < 4; ++i){
		if(player_hand[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
			// if y == 388 then card is still in hand and not on board/
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && player_hand[i].getPosition().y == 388){
				
				player_hand[i].setPosition(board.board_left[left_side_sprites.size()]);
				left_side_sprites.push_back(player_hand[i]);
				board.hand_card_text[i].setPosition(board.numbers_left[board.draw_card_text_left.size()]);
				board.draw_card_text_left.push_back(board.hand_card_text[i]);
				
				player_hand[i] = empty_spr; // "Delete" card from hand
				board.hand_card_text[i] = empty_txt; // "Delte" text from hand card txt arrs
				
				p1.score += p1.hand[i];
				p1_hand_card_number = p1.hand[i];
				board.player_one_score.setString(std::to_string(p1.score));
				
				if(!p2.standing){
					current_state = States::player_two_turn;
					state_changed = true;
				}
				ready_to_send = true;
				card_was_drawn = false;
				return;
			}
		}		
	}
	
	if(board.end_turn_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			p1.end_turn = true;
			if(!p2.standing){
				current_state = States::player_two_turn;	
				state_changed = true;			
			}
			btn_was_released = false;
			card_was_drawn = false;
			ready_to_send = true;
			return;
		}
	}
	if(board.stand_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			p1.standing = true;
			p1.end_turn = true;
			if(!p2.standing){
				current_state = States::player_two_turn;				
			}
			card_was_drawn = false;
			btn_was_released = false;
			ready_to_send = true;
			return;
		}
	}
}

void Game::UpdateGame()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
	}
	// Check if we have a winner, set the appropriate game state
	if(p1.standing && p2.standing){
	//	WhoIsWinner();
		Reset();
	}
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		std::cout << p2.standing << std::endl;
	}
	
	if(round_count < 3 && current_state != States::game_over){
		if(current_state == States::player_one_turn && !p1.standing){
			if(!card_was_drawn){
				p1.score += DrawCard(board.board_left, left_side_sprites, board.numbers_left, board.draw_card_text_left);
				board.player_one_score.setString(std::to_string(p1.score));
				card_was_drawn = true;
			}
			HandleUserInput();
		}
		// && sent - make sure that we sent all the data to player 2 and reset all the variables
		if(current_state == States::player_two_turn && received){
			std::cout << "CHE BLYAT ZA HUINYA&!" << std::endl;
			if(p2.standing){
				if(!p1.standing){
					current_state = States::player_one_turn;
					card_was_drawn = false;
				}
				received = false;
			}
			if(!card_was_drawn && p2_draw_card_number != 999){
				//p2.score += 
				DrawCard(board.board_right, right_side_sprites, board.numbers_right, board.draw_card_text_right);
				board.player_two_score.setString(std::to_string(p2.score));
				card_was_drawn = true;
				received = false;
			}
			if(p2_hand_card_number != 999){
				static int p2_hand_card_position = 0;
				ConvertBackToFace(AI_hand[p2_hand_card_position], p2_hand_card_number);
				AI_hand[p2_hand_card_position] = empty_spr;
				++p2_hand_card_position;
				board.player_two_score.setString(std::to_string(p2.score));
			
				if(!p1.standing){
					current_state = States::player_one_turn;
				}
				//ready_to_send = true;
				received = false;
				card_was_drawn = false;
			}
			if(p2.end_turn){
				p2.end_turn = false;
				if(!p1.standing){
					current_state = States::player_one_turn;
					card_was_drawn = false;
				}
				received = false;
			}

		}
		
	} else {
		// Announce results
	}
}

void Game::RenderGame()
{
	window.clear();
	window.draw(board.backGround);
	// Draw left/right side yellow cards
	if(left_side_sprites.size() > 0){
		for(int i = 0; i < left_side_sprites.size(); ++i){
			window.draw(left_side_sprites[i]);
		}		
	}
	
	if(right_side_sprites.size() > 0){
		for(int i = 0; i < right_side_sprites.size(); ++i){
			window.draw(right_side_sprites[i]);
		}		
	}
	
	// Draw hands
	for(int i = 0; i < 4; ++i){
		window.draw(player_hand[i]);
		window.draw(AI_hand[i]);
	}
	
	// Draw all the numbers
	for(int i = 0; i < board.draw_card_text_left.size(); ++i){
		window.draw(board.draw_card_text_left[i]);
	}
	for(int i = 0; i < board.draw_card_text_right.size(); ++i){
		window.draw(board.draw_card_text_right[i]);
	}
	for(int i = 0; i < board.hand_card_text.size(); ++i){
		window.draw(board.hand_card_text[i]);
	}

	window.draw(board.end_turn_btn);
	window.draw(board.end_turn_text);
	window.draw(board.stand_btn);
	window.draw(board.stand_text);
	window.draw(board.player_one_score);
	window.draw(board.player_two_score);
	
	window.display();
}

// Handels input on deck builde screen
void Game::UpdateDeckBuilder()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
	}
}

void Game::RenderDeckBuilder()
{
	sf::Sprite card;
	sf::Vector2f pos (50, 45);
	card.setTexture(board.card_texture);
	//hand_card.setOrigin(46/2, 64/2);
	for(int i = 0; i < 18; ++i){
		if(i < 6){
			card.setTextureRect(sf::IntRect(97, 0, 46, 64));
			card.setPosition(pos);
			pos.y += 68;
			available_cards.push_back(card);
			continue;
		}
		if(i > 6 && i < 12){
			card.setTextureRect(sf::IntRect(50, 0, 46, 64));
			pos.x = 100;
			card.setPosition(pos.x, pos.y - available_cards[i-1].getPosition().y);

			available_cards.push_back(card);
			continue;
		}
		if(i > 12){
			card.setTextureRect(sf::IntRect(192, 0, 46, 64));
			pos.x = 150;
			card.setPosition(pos);
			pos.y += 68;
			available_cards.push_back(card);
			continue;
		}
	}
	window.draw(board.deck_builder_sprite);
	for(unsigned int i = 0; i < available_cards.size(); ++i){
		window.draw(available_cards[i]);
	}
	window.display();
}

int Game::UpdateStartScreen()
{

	// Listen or connect
	/*if(client_connected){
		current_state = States::player_one_turn;
	}*/
	/*if(is_server){
		if(Server() == 1){
			current_state = States::player_one_turn;
		}
	}
	if(is_client){
		if(Client() == 1){
			current_state = States::player_one_turn;
		}
	}*/
	
	/*if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			host_game.was_released = true;
			join_game.was_released = true;
	}*/
	if(host_game.rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && host_game.was_released){
			host_game.was_released = false;
			std::cout << "Host game button pressed" << std::endl;
			return 1;
		}
	}
	if(join_game.rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && join_game.was_released){
			join_game.was_released = false;
			std::cout << "Join game button pressed" << std::endl;
			return 0;
			//std::thread client_thread(Client);
		}
	}
	return -1;
}

void Game::RenderStartScreen()
{
	window.draw(host_game.rect);
	window.draw(join_game.rect);
	
	window.display();
}

int Game::DrawCard(std::array<sf::Vector2f, 9>& board_pos, std::vector<sf::Sprite>& card_sprites, std::array<sf::Vector2f, 9> numbers_pos, std::vector<sf::Text>& draw_card_text)
{
	int rndn = 0;
	
	if(current_state == States::player_two_turn){
		rndn = p2_draw_card_number;
	} else {
		rndn = dist(mt);
		p1_draw_card_number = rndn;		
	}
	
	sf::Sprite new_card;
	
	new_card.setTexture(board.card_texture);
	new_card.setTextureRect(sf::IntRect(194, 0, 46, 64));
	new_card.setOrigin(46/2, 64/2);
	
	new_card.setPosition(board_pos[card_sprites.size()]);

	board.card_number.setString(std::to_string(rndn));
	

	board.card_number.setPosition(numbers_pos[card_sprites.size()]);

	draw_card_text.push_back(board.card_number);
	card_sprites.push_back(new_card);
	
	// Send draw card number if its not player's two turn. Coz otherwise we shouldn't send anything
	// (it will confuse the client's update cycle!)
	if(current_state != States::player_two_turn){
		ready_to_send = true;		
	}
	return rndn;
}

void Game::ConvertBackToFace(sf::Sprite& card_to_convert, int card_number)
{
	if(p2.hand[card_number] > 0){
		card_to_convert.setTextureRect(sf::IntRect(97, 0, 46, 64));
	}
	if(p2.hand[card_number] < 0){
		card_to_convert.setTextureRect(sf::IntRect(50, 0, 46, 64));
	}
	if(p2.hand[card_number] > 0 && p2.hand[card_number] % 2 == 0){
		card_to_convert.setTextureRect(sf::IntRect(192, 0, 46, 64));
	}
	card_to_convert.setPosition(board.board_right[right_side_sprites.size()]);
		
	board.card_number.setString(std::to_string(p2.hand[card_number]));
	board.card_number.setPosition(board.numbers_right[right_side_sprites.size()]);
	
	board.draw_card_text_right.push_back(board.card_number);
	right_side_sprites.push_back(card_to_convert);
}

void Game::SetPlayerOneHand()
{
	sf::Sprite hand_card;
	hand_card.setTexture(board.card_texture);
	hand_card.setOrigin(46/2, 64/2);

	for(int i = 0; i < 4; ++i){
		if(p1.hand[i] > 0){
			hand_card.setTextureRect(sf::IntRect(97, 0, 46, 64));
		}
		if(p1.hand[i] < 0){
			hand_card.setTextureRect(sf::IntRect(50, 0, 46, 64));
		}
		if(p1.hand[i] > 0 && p1.hand[i] % 2 == 0){
			hand_card.setTextureRect(sf::IntRect(192, 0, 46, 64));
		}
		
		hand_card.setPosition(board.hand_left[i]);
		player_hand.push_back(hand_card);
		
		board.card_number.setString(std::to_string(p1.hand[i]));

		board.card_number.setPosition(board.numbers_hand_left[board.hand_card_text.size()]);

		board.hand_card_text.push_back(board.card_number);
	}
}

void Game::SetHiddenHand()
{	
	sf::Sprite hand_card;
	hand_card.setOrigin(46/2, 64/2);
	hand_card.setTexture(board.card_texture);
	hand_card.setTextureRect(sf::IntRect(0, 0, 46, 64));
	
	for(int i = 0; i < 4; ++i){
		hand_card.setPosition(board.hand_right[i]);
		AI_hand.push_back(hand_card);
	}
}

void Game::Reset()
{
	left_side_sprites.clear();
	right_side_sprites.clear();
	
	board.draw_card_text_left.clear();	
	board.draw_card_text_right.clear();
	
	board.player_one_score.setString("0");
	board.player_two_score.setString("0");
	
	current_state = States::player_one_turn;
	p1.standing = false;
	p2.standing = false;
	p1.score = 0;
	p2.score = 0;
	
}

void Game::HandleInput()
{
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		board.stand_btn.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	}
	if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		board.stand_text.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	}
	if(Keyboard::isKeyPressed(Keyboard::B)){
		info.setString("X: " + std::to_string(board.stand_btn.getPosition().x) + "  Y: " +   std::to_string(board.stand_btn.getPosition().y));
	}
	if(Keyboard::isKeyPressed(Keyboard::T)){
		info.setString("X: " + std::to_string(board.stand_text.getPosition().x) + "  Y: " +   std::to_string(board.stand_text.getPosition().y));
	}
	if(Keyboard::isKeyPressed(sf::Keyboard::Up)){
		//DrawCard();
	}
}
















/*////////////////////////////////////////////////////
BOARD COODRINATS/

TURN LIGHT - X:  Y: 
LEFT SIDE FIRST ROW FIRST SLOT CENTER X: 137 Y: 155
LEFT SIDE FIRST ROW SECOND SLOT CENTER X: 187 Y: 155
RIGHT SIDE FIRST ROW FIRST SLOT CENTER X:362 Y: 155


NUMBER POS X: 133 Y: 146 !!!!!!!!!!!!!!

NUMBER FOR HAND CARDS X: 98 Y: 379 

LEFT SIDE SECOND ROW FIRST SLOT CENTER X:137 Y:226
LEFT SIDE SECOND ROW SECOND SLOT X:187 Y: 225 (226 maybe??)
RIGHT SIDE SECOND ROW FIRST SLOT CENTER X:362 Y: 226
RIGHT SIDE SECOND ROW SECOND SLOT X: 412 Y: 226
HAND LEFT FIRST CARD X: 102 Y: 388 SECOND: X: 153 Y: 388 
HAND RIGHT FIRST CARD X:348 Y:388 SECOND X: 397









															*/
