#include "Player.h"
#include "Board.h"
Player::Player()
{
	for(int i = 0; i < 4; ++i){
		hand[i] = 3;
	}
}