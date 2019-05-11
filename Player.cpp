#include "Player.h"
#include "Board.h"
#include <fstream>
#include <string>  
#include <random>
#include <chrono>
#include <iostream>

void Player::Init()
{
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist(0, 9);
	mt.seed(std::time(0));
	
	std::string card = "";
	int deck[10] = {0};
	
	std::fstream deck_file;
	deck_file.open("deck");
	if(deck_file.fail()){
		return;
	}
	for(int i = 0; i < 10; ++i){
		deck_file >> card;
		deck[i] = std::stoi(card);
		
	}
	
	for(int i = 0; i < 4; ++i){
		hand[i] = deck[dist(mt)];
		std::cout << hand[i] << std::endl;
	}
	
}