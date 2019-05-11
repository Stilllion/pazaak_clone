#ifndef Game_h
#define Game_h
#include "Board.h"
#include "Player.h"
#include <random>
#include <chrono>
#include <iostream>

using namespace sf;

#define mouse_position static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))
enum States {start_game, pause_state, game_over, player_one_turn, player_two_turn,
			 p1WinSet, p1LostSet, p1WinGame, p1LostGame, game_tie, set_tie, waiting_for_player, round_over};
struct Game
{
	
	States current_state; //= States::player_one_turn;
	sf::RenderWindow& window;
	// RNDN generator
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist;
	// Game methods
	void UpdateGame();
	void UpdateDeckBuilder();
	int UpdateStartScreen();
	
	void RenderGame();
	void RenderDeckBuilder();
	void RenderStartScreen();
	
	int DrawCard(std::array<sf::Vector2f, 9>& board_pos, std::vector<sf::Sprite>& card_sprites, std::array<sf::Vector2f, 9> numbers_pos, std::vector<sf::Text>& draw_card_text);
	void DrawPlayerHand();
	
	void SetPlayerOneHand();
	void SetHiddenHand();
	
	void HandleUserInput();
	void HandleAIinput();
	
	void PopUp();
	int Rules();
	void CheckWinCond();
	void EndRound();
	
	// for dbg
	void HandleInput();
	void On_receive();
	
	void ConvertBackToFace(sf::Sprite& card_to_convert, int card_number);
	void Reset();
	
	void FlipCard(int card_index);
	// SFML utils
	//sf::RenderWindow window;
	//sf::Event event;
	sf::Clock clock;
	sf::Time elapsed;
	float frametime = 1.0/15.0;
	
	// InDev tools
	sf::Text info;
	bool btn_was_released = true;
	bool card_was_drawn = false;
	
	
	// Packet data
	int p1_draw_card_number = 999;
	int p2_draw_card_number = 999;
	int p1_hand_card_number = 999;
	int p2_hand_card_number = 999;
	
	bool state_changed = false;

	sf::Sprite empty_spr; // For deleting cards
	sf::Text empty_txt;   // For deleting text
	
	// Card sprites are kept here
	std::vector<sf::Sprite> left_side_sprites;
	std::vector<sf::Sprite> right_side_sprites;
	std::vector<sf::Sprite> player_hand;
	std::vector<sf::Sprite> AI_hand;
	std::vector<sf::Sprite> available_cards;
	std::array<sf::CircleShape, 4> flipable_cards;
	//std::array<sf::Vector2f, 4> flip_icon_positions;
	
	int round_count = 0;
	
	Board board;
	Player p1;
	Player p2;

	
	Game(sf::RenderWindow& w);

	bool client_connected = false;
	bool is_server = false;
	bool is_client = false;
	
	bool ready_to_send = false;
	bool received = false;
	bool sent = false;
	
	bool player_init = true;
	
	// Pop-up messages
	sf::Texture youWinSet_texture;
	sf::Texture youWinSet_HL_texture;
	
	sf::Texture youLostSet_texture;
	sf::Texture youLostSet_HL_texture;
	
	sf::Texture youWinGame_texture;
	sf::Texture youWinGame_HL_texture;
	
	sf::Texture youLostGame_texture;
	sf::Texture youLostGame_HL_texture;
	
	sf::Texture waiting_texture;
	
	sf::Texture flip_icon_texture;
	
	sf::Sprite popup_spr;
	
	sf::Sprite flip_icon;
};

#endif