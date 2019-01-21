#include "Game.h"

sf::Packet& operator >>(sf::Packet& packet, Player& p){
	return packet >> p.score >> p.standing >> p.score;
}

sf::Packet& operator <<(sf::Packet& packet, Player& p){
	return packet << p.score << p.standing << p.score;
}
	bool client_connected = false;
	bool is_client = false;
	int server_mode = -1;
	Game g;
	
	// Client
	sf::TcpSocket socket;
	
	// Server
	sf::TcpListener listener;
	sf::TcpSocket client;	

int Receive(){
	
	client.setBlocking(false);
	socket.setBlocking(false);
	
	static sf::Packet packet;
	
	if(!is_client){
		//std::cout << "TRY RECEIVE SERVER" << std::endl;
		sf::Socket::Status status = client.receive(packet);
		if(status == sf::Socket::NotReady){
		//	std::cout << "CLIENT NOT READY" << std::endl;
			return 0;
		}
		if(status == sf::Socket::Error){
			std::cout << "ERROR" << std::endl;
			return 0;
		}
		if(status == sf::Socket::Done){
			std::cout << "SERVER RECEIVED" << std::endl;
			packet >> g.draw_card_number >> g.hand_card_number >> g.p2;
		}
		// If we did not receive anything useful
		if(g.draw_card_number == 999 || g.hand_card_number == 999){
			return 0;
		}
		//g.received = true;
	}
	
	if(is_client){
		//std::cout << "TRY RECEIVE CLIENT" << std::endl;
		sf::Socket::Status status = socket.receive(packet);
		if(status == sf::Socket::NotReady){
		//	std::cout << "SERVER NOT READY" << std::endl;
			return 0;
		}
		if(status == sf::Socket::Error){
			std::cout << "RECEIVE ERROR" << std::endl;
			return 0;
		}
		if(status == sf::Socket::Done){
			std::cout << "CLIENT RECEIVED" << std::endl;
			packet >> g.draw_card_number >> g.hand_card_number >> g.p2;
			std::cout << g.draw_card_number << g.hand_card_number << g.state_changed << std::endl;
		}
		// If we did not receive anything useful
		if(g.draw_card_number == 999 || g.hand_card_number == 999){
			return 0;
		}
		//g.received = true;
	}
	return 0;
}

int Listen(){
	// Bind listner to a port
	//listener.setBlocking(false);
	if(listener.listen(25565) != sf::Socket::Done){
		std::cout  <<  "NO connections" << std::endl;
		return 0;
	}
	// accept connection
	if(listener.accept(client) != sf::Socket::Done){
		//std::cout  << "Something went wrong \n";
		return 0;
	} else {
		std::cout << "Connected! >> " << client.getRemoteAddress() << std::endl;
		g.current_state = States::player_one_turn;
		client_connected = true;
		is_client = false;
		return 1;
	}
}

int Connect(){
	
	//socket.setBlocking(false);
	sf::Socket::Status status = socket.connect("192.168.56.1", 25565);
	//sf::Socket::Status status = socket.connect("127.0.0.1", 25565);
	
	if(status != sf::Socket::Done){
		std::cout  << "Could not connect" << std::endl;
		return 0;
	} else {
		client_connected = true;
		g.current_state = States::player_two_turn;
		is_client = true;
		return 1;
	}
}

int Send(){
	/*static*/ sf::Packet packet;
	client.setBlocking(false);
	socket.setBlocking(false);
	packet << g.draw_card_number << g.hand_card_number << g.p1;
	
	if(is_client){
		if(socket.send(packet) != sf::Socket::Done){
			std::cout << "Couldn't send" << std::endl;
		} else {
			g.hand_card_number = 999;
			g.draw_card_number = 999;
			std::cout << "SENT!" << std::endl;
		}
	}
	if(!is_client){
		//std::cout << "About to send" << std::endl;
		if(client.send(packet) != sf::Socket::Done){
			std::cout << "Couldn't send" << std::endl;
		} else {
			std::cout << "SENT!" << std::endl;
			g.hand_card_number = 999;
			g.draw_card_number = 999;
		}
	}
	return 0;
}

	
int Server(){
	
	sf::Clock clock;
	sf::Time elapsed;
	float tick = 1.0/20.0;
	std::cout << "Thread started" << std::endl;
	std::cout << "server_mode " << server_mode << std::endl;
	clock.restart();
	while(1){
		while(elapsed.asSeconds() > tick){
			//TIMEOUTS += elapsed.asSeconds();
			if(server_mode == 1 && !client_connected){
				Listen();
			}
			if(server_mode == 0 && !client_connected){
				Connect();
			}
			if(client_connected){
				//std::cout << g.ready_to_send << std::endl;
				if(g.ready_to_send){
					Send();
					g.ready_to_send = false;
				}
				Receive();
			}
			elapsed -= sf::seconds(tick);
		}
		//std::cout << "TICK!" << std::endl;
		elapsed += g.clock.restart();
	}
}	

int main()
{
	g.current_state = States::start_game;
	
	sf::Thread server_thread(&Server);
	///sf::Thread client_thread(&Connect);
	
	/*sf::Thread send_thread(&Send);
	sf::Thread receive_thread(&Receive);*/
	
	while(g.window.isOpen()){
		while(g.window.pollEvent(g.event)){
			if(g.event.type == sf::Event::Closed){
				g.window.close();
			}
		}
		if(g.current_state == States::deck_builder){
			g.UpdateDeckBuilder();
			g.RenderDeckBuilder();
			continue;
		}
		if(g.current_state == States::start_game){
			server_mode = g.UpdateStartScreen();

			if(server_mode == 1 || server_mode == 0){
				server_thread.launch();
				g.current_state = States::cancel_game;
			}

			g.RenderStartScreen();
			continue;
		}
		if(g.current_state == States::cancel_game){
			// MOVE THIS IF TO THE SERVER THREAD!!!
			if(client_connected){
				g.current_state = States::player_one_turn;
			}
			g.RenderStartScreen();
			continue;

		}

		g.UpdateGame();
		g.RenderGame();
		//receive_thread.launch();
	
		g.elapsed += g.clock.restart();
	} 
}






