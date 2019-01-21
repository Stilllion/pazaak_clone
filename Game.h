#ifndef Game_h
#define Game_h
#include "Board.h"
#include "Player.h"
#include <random>
#include <chrono>
#include "States.h"
#include <iostream>

#include <SFML/Network.hpp>



using namespace sf;

struct Game
{
	States current_state; //= States::player_one_turn;
	
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
	
	// for dbg
	void HandleInput();
	void On_receive();
	
	void ConvertBackToFace(sf::Sprite& card_to_convert, int card_number);
	void Reset();
	// SFML utils
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed;
	float frametime = 1.0/15.0;
	
	// InDev tools
	sf::Text info;
	bool btn_was_released = true;
	bool card_was_drawn = false;
	
	
	// Packet data
	int draw_card_number = 999;
	int hand_card_number = 999;
	bool state_changed = false;

	sf::Sprite empty_spr; // For deleting cards
	sf::Text empty_txt;   // For deleting text
	
	// Card sprites are kept here
	std::vector<sf::Sprite> left_side_sprites;
	std::vector<sf::Sprite> right_side_sprites;
	std::vector<sf::Sprite> player_hand;
	std::vector<sf::Sprite> AI_hand;
	std::vector<sf::Sprite> available_cards;
	
	int round_count = 1;
	Board board;
	Player p1;
	Player p2;

	
	Game();

// NETWORKING
// Network stuff
	sf::TcpListener listener;
	sf::TcpSocket client;
	sf::Packet packet;
	sf::TcpSocket socket;
	
	int Server();
	int Client();
	bool client_connected = false;
	bool is_server = false;
	bool is_client = false;
	
	bool ready_to_send = false;
	bool received = false;
	

};

#endif