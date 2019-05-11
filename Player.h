#ifndef Player_h
#define Player_h
#include <array>
struct Player
{
	std::array<int, 4> hand = {0};
	
	std::string name = "";
	bool standing = false;
	bool end_turn = false;
	// Maybe those two belong in Game class
	int score = 0;
	int wins = 0;
	bool ready = false;
	void Init();
};

#endif