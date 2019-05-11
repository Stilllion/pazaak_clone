#include "Game.h"

Game::Game(sf::RenderWindow& w) : dist(1, 10), window(w)
{
	/*FULLLSCREEEEN MODE
	https://stackoverflow.com/questions/21363557/resizing-in-sfml-2-0*/
	srand(time(0));
	//window.setFramerateLimit(15);


	// INDEV TOOLS
	
	info.setFont(board.font);
	info.setCharacterSize(10);
	info.setPosition(100, 100);
	
	youWinSet_HL_texture;
	youLostSet_HL_texture;
	youWinGame_HL_texture;
	youLostGame_HL_texture;
	
	youWinSet_texture.loadFromFile("textures/you_WinSet.png");
	
	youLostSet_texture.loadFromFile("textures/you_LostSet.png");
	
	youWinGame_texture.loadFromFile("textures/you_WinGame.png");
	
	youLostGame_texture.loadFromFile("textures/you_LostGame.png");
	
	waiting_texture.loadFromFile("textures/waiting.png");
	
	flip_icon_texture.loadFromFile("textures/lbl_cardhilite.tga");
	
	popup_spr.setPosition(187, 225);
	
	flip_icon.setTexture(flip_icon_texture);

	//flip_icon_positions = {sf::Vector2f(90, 420), sf::Vector2f(140, 420), sf::Vector2f(190, 420), sf::Vector2f(240, 420)};
	
	// Random number seed
	mt.seed(std::time(0));
	clock.restart();
}

int Game::Rules()
{
    if (p1.score == p2.score){return 0;}
    if (p1.score == 20 && p2.score > 20){return 1;}
    if (p1.score == 20 && p2.score < 20){return 1;}
    if (p1.score > 20 && p1.score < p2.score){return 1;}
    if (p1.score < 20 && p1.score > p2.score){return 1;}
    else{return 2;}
}

void Game::CheckWinCond()
{
	int result = Rules();
	std::cout << "CHANGE STATE CALLED CHECKWINCOND" << std::endl; 
	switch(result){
		case 1: {++p1.wins; current_state = States::p1WinSet;}
		break;
		case 2: {++p2.wins; current_state = States::p1LostSet;}
		break;
		case 0: {current_state = States::set_tie;}		
	}
	++round_count;
	std::cout << round_count << std::endl;
}

void Game::HandleAIinput()
{
	if(state_changed == true){
		btn_was_released = true;
		board.player_two_score.setString(std::to_string(p2.score));
		current_state = States::player_one_turn;
		return;
	}
}

void Game::HandleUserInput()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			btn_was_released = true;
	}
	sf::Vector2f mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	// Click on Flip icon
	for(size_t i = 0; i < flipable_cards.size(); ++i){
		if(flipable_cards[i].getGlobalBounds().contains(mp)){
			// Highlight maybe?
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
				FlipCard(i);
				std::cout << "FLIP!" << std::endl;
				btn_was_released = false;
			}
		}
	}
	
	// Click on card/end turn button/ stand button
	mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	
	for(int i = 0; i < 4; ++i){
		if(player_hand[i].getGlobalBounds().contains(mp)){
			// if y == 388 then card is still in hand and not on board/
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && player_hand[i].getPosition().y == 388){
				
				player_hand[i].setPosition(board.board_left[left_side_sprites.size()]);
				left_side_sprites.push_back(player_hand[i]);
				board.hand_card_text[i].setPosition(board.numbers_left[board.draw_card_text_left.size()]);
				board.draw_card_text_left.push_back(board.hand_card_text[i]);
				
				// TO-DO redo this trough vec.erase()
				player_hand[i] = empty_spr; // "Delete" card from hand
				board.hand_card_text[i] = empty_txt; // "Delte" text from hand card txt arrs
				
				p1.score += p1.hand[i];
				p1_hand_card_number = i;
				board.player_one_score.setString(std::to_string(p1.score));
				
				if(!p2.standing){
					current_state = States::player_two_turn;
				}
				
				ready_to_send = true;
				card_was_drawn = false;
				return;
			}
		}		
	}
	
	mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	
	if(board.end_turn_btn.getGlobalBounds().contains(mp)){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			p1.end_turn = true;
			if(!p2.standing){
				current_state = States::player_two_turn;				
			}
			btn_was_released = false;
			card_was_drawn = false;
			ready_to_send = true;
			return;
		}
	}
	
	mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	
	if(board.stand_btn.getGlobalBounds().contains(mp)){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			p1.standing = true;
			p1.end_turn = true;
			p1.ready = false;
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

void Game::EndRound()
{
	if(current_state == States::player_one_turn || current_state == States::player_two_turn){
		CheckWinCond();
	}
	
	std::cout << p1.ready << " " << p2.ready << std::endl;
	
	PopUp();

	
	if(p1.ready && current_state != States::waiting_for_player){		
		current_state = States::waiting_for_player;
		ready_to_send = true;
		// If we are the client we want to reset the game right away so 
		// when host sends us data for a new round we wouldn't accidentally delete it
		if(is_client){
			Reset();
		}
	}
	
	if(p2.ready && p1.ready){
		if(!is_client){
			Reset();
			current_state = States::player_one_turn;			
		}
		if(is_client){current_state = States::player_two_turn;}
		ready_to_send = true;
	}
}

void Game::UpdateGame()
{
	if(player_init){
		p1.Init();
		SetHiddenHand();
		SetPlayerOneHand();
		player_init = false;
		
	}
	// Check if we have a winner, set the appropriate game state
	if(p1.standing && p2.standing || current_state == States::waiting_for_player || current_state == States::p1WinSet
	|| current_state == States::p1LostSet || current_state == States::set_tie){
		EndRound();
	}
	
	if(round_count <= 2 && (current_state != States::game_over || current_state != States::waiting_for_player)){
		
		if(p1.score == 20 && current_state == States::player_one_turn){
			p1.standing = true;
			ready_to_send = true;
			card_was_drawn = false;
			if(!p2.standing){
				current_state = States::player_two_turn;
			}
		}

		
		if(current_state == States::player_one_turn && !p1.standing){
			// switch the turn light
			board.spr_turn_light.setPosition(board.turn_ligths_pos[0]);
			if(!card_was_drawn){
				p1.score += DrawCard(board.board_left, left_side_sprites, board.numbers_left, board.draw_card_text_left);
				board.player_one_score.setString(std::to_string(p1.score));
				card_was_drawn = true;
			}
			HandleUserInput();
		}
		// && sent - make sure that we sent all the data to player 2 and reset all the variables
		if(current_state == States::player_two_turn && received){
			// switch the turn light
			board.spr_turn_light.setPosition(board.turn_ligths_pos[1]);
			if(p2.standing){
				p2.ready = false;
				if(!p1.standing){
					current_state = States::player_one_turn;
					card_was_drawn = false;
				}
				received = false;
			}
			
			if(!card_was_drawn && p2_draw_card_number != 999){
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
		if(p1.wins > p2.wins){
			current_state = States::p1WinGame; 
			PopUp();
		} else {
			current_state = States::p1LostGame; 
			PopUp();
		}
	}
}

void Game::FlipCard(int card_index)
{
	p1.hand[card_index] *= -1;
	// length > 1 means there is minus appended to card number 
	
	std::string card_number_txt = board.hand_card_text[card_index].getString();
	
	if(card_number_txt.length() > 1){
		board.hand_card_text[card_index].setString(card_number_txt.substr(1, 1));
	} else {
		board.hand_card_text[card_index].setString("-" + card_number_txt);
	}
	/*if(board.hand_card_text[card_index].getString.length() > 1){
		board.hand_card_text[card_index].setString(board.hand_card_text[card_index].getString.substr(1, 2));
	} else {
		board.hand_card_text[card_index].setString("-" + board.hand_card_text[card_index].getString);
	}*/
	
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
	
	// Draw flip icons
	if(!flipable_cards.empty()){
		for(size_t i = 0; i < 4; ++i){
	//		std::cout << flipable_cards[i].getPosition().x << " " << flipable_cards[i].getPosition().y << std::endl;
			window.draw(flipable_cards[i]);
		}
	}
	
	window.draw(board.end_turn_btn);
	window.draw(board.end_turn_text);
	window.draw(board.stand_btn);
	window.draw(board.stand_text);
	window.draw(board.player_one_score);
	window.draw(board.player_two_score);
	window.draw(board.spr_turn_light);
	
	if(current_state != States::player_one_turn && current_state != States::player_two_turn){
		window.draw(popup_spr);
	}
	
	window.display();
}


void Game::PopUp()
{
	if(popup_spr.getGlobalBounds().contains(mouse_position)){
		std::cout << "CLICK ME" << std::endl;
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			// if this true? time to end game and return to MainMenu
			if(current_state == States::p1LostGame || current_state == States::p1WinGame){
				current_state = States::game_over;
			} else {
				// Player is ready to contnue the game
				p1.ready = true;
			}
		}
	}
	
	switch (current_state){
		case States::p1WinSet:{popup_spr.setTexture(youWinSet_texture);}
		break;
		case States::p1LostSet:{popup_spr.setTexture(youLostSet_texture);}
		break;
		case States::p1WinGame:{popup_spr.setTexture(youWinGame_texture);}
		break;
		case States::p1LostGame:{popup_spr.setTexture( youLostGame_texture);}
		break;
		case States::waiting_for_player:{popup_spr.setTexture(waiting_texture);}
	}
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
	
	if(card_sprites.size() < 8){
		new_card.setPosition(board_pos[card_sprites.size()]);		
		
		board.card_number.setString(std::to_string(rndn));
		board.card_number.setPosition(numbers_pos[card_sprites.size()]);
	}
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
	if(p2.hand[card_number] % 11 == 0){
		card_to_convert.setTextureRect(sf::IntRect(146, 0, 46, 64));
		p2.hand[card_number] = p2.hand[card_number] / 11;
	}
	card_to_convert.setPosition(board.board_right[right_side_sprites.size()]);
	std::cout << p2.hand[card_number] << std::endl;
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

	std::array<sf::Vector2f, 4> flip_icon_positions = {sf::Vector2f(90, 420), sf::Vector2f(140, 420), sf::Vector2f(190, 420), sf::Vector2f(240, 420)};
	
	for(int i = 0; i < 4; ++i){
		std::cout << p1.hand[i] << std::endl;
		if(p1.hand[i] > 0){
			hand_card.setTextureRect(sf::IntRect(97, 0, 46, 64));
		}
		if(p1.hand[i] < 0){
			hand_card.setTextureRect(sf::IntRect(50, 0, 46, 64));
		}
		// 11, 22, 33 ... 66
		if(p1.hand[i] > 0 && p1.hand[i] % 11 == 0){
			hand_card.setTextureRect(sf::IntRect(146, 0, 46, 64));
		
			board.card_number.setString(std::to_string(p1.hand[i]/11));
			// Get rid of the "flag" and convert card value to a valid one
			p1.hand[i] = p1.hand[i] / 11;
			
			// Take Flip Icon, and put it in flipable_cards array at the position corresponding to the position of a card in player's hand
			
			sf::CircleShape icon;
			icon.setRadius(9);
			icon.setFillColor(sf::Color::Red);
			
			flipable_cards[i] = icon;
			
			flipable_cards[i].setPosition(flip_icon_positions[i].x, flip_icon_positions[i].y);
			
		} else {
			board.card_number.setString(std::to_string(p1.hand[i]));			
		}
		
		hand_card.setPosition(board.hand_left[i]);
		player_hand.push_back(hand_card);

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

	p1.standing = false;
	p2.standing = false;
	p1.score = 0;
	p2.score = 0;

	received = false;
	card_was_drawn = false;
	
	/*if(is_client){current_state = States::player_two_turn;}
	if(!is_client){current_state = States::player_one_turn;}*/

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
