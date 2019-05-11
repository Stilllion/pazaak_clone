#include "Game.h"
#include "MainMenu.h"
#include "states.h"
#include "DeckBuilder.h"
	
void ExitGame(Game& g)
{
	g.Reset();
	g.current_state = States::player_one_turn;
}	

void NetworkShutDown(UDP& udp_net, TCP& tcp_net){
	tcp_net.server_is_on = false;
	tcp_net.client_connected = false;
	tcp_net.server_mode = -1;
	tcp_net.is_client = false;
	
	udp_net.host_found = false;
	udp_net.cancel = true;
	udp_net.client = false;

}

int main()
{
	enum ProgramStates {main_menu, game, deck_builder};
	ProgramStates program_state;
	bool thread_launched = false;
	
	sf::RenderWindow window;
	sf::Event event;
	
	window.create(sf::VideoMode(600, 500), "Pazaak", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	
	Game g(window);
	DeckBuilder db(window);
	
	UDP udp_net;
	TCP tcp_net(g);
	
	MainMenu menu(window, udp_net, tcp_net);
	
	sf::Thread tcp_thread(&TCP::Net, &tcp_net);
	sf::Thread udp_thread(&UDP::Run, &udp_net);

	program_state = ProgramStates::main_menu;
	//program_state = ProgramStates::game;
	//program_state = ProgramStates::deck_builder;
	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if(event.type == sf::Event::Resized){
				sf::Vector2f new_size = static_cast<sf::Vector2f>(window.getSize());
				new_size.y = new_size.x / 1.2;
				
				window.setSize(static_cast<sf::Vector2u>(new_size));
			}	
		}
		switch(program_state){
			case ProgramStates::main_menu : {
				menu.Update();
				menu.Render();
				if(menu.start_game && !thread_launched){
					std::cout << "Launching thread Client: " << udp_net.client <<std::endl;
					udp_thread.launch();
					thread_launched = true;
				}
				if(!menu.start_game && thread_launched){
					std::cout << "SHUTTING DOWN THE SERVER" << std::endl;
					NetworkShutDown(udp_net, tcp_net);
					thread_launched = false;
				}
				if(udp_net.host_found){
					std::cout << "Supposte to tun TCP thread now" << std::endl;
					tcp_net.server_adress = udp_net.sender_adress;
					std::cout << "Server address just to make sure " << tcp_net.server_adress << std::endl;
					tcp_thread.launch();
					udp_net.host_found = false;
				}
				if(tcp_net.client_connected){
					program_state = ProgramStates::game;
				}
				if(menu.curr_subMenu == menu.SubMenus::deck_builder){
					program_state = ProgramStates::deck_builder;
					menu.curr_subMenu = menu.SubMenus::main_menu;
				}
			}
			break;
			case ProgramStates::game : {
				g.UpdateGame();
				g.RenderGame();
				if(g.current_state == States::game_over){
					ExitGame(g);
					NetworkShutDown(udp_net, tcp_net);
					program_state = ProgramStates::main_menu;
					menu.curr_subMenu = menu.SubMenus::main_menu;
				}
				g.elapsed += g.clock.restart();
			}
			break;
			case ProgramStates::deck_builder : {
				db.Update();
				db.Render();
				if(db.cancel || db.saved){
					program_state = ProgramStates::main_menu;
					db.Reset();
				}
			}
		}
	}
}






