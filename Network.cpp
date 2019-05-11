#include "network.h"

sf::Packet& operator >>(sf::Packet& packet, Player& p){
	return packet >> p.score >> p.end_turn >> p.standing  >> p.hand[0] >> p.hand[1] >> p.hand[2] >> p.hand[3] >> p.ready;
}

sf::Packet& operator <<(sf::Packet& packet, Player& p){
	return packet << p.score << p.end_turn << p.standing  << p.hand[0] << p.hand[1] << p.hand[2] << p.hand[3] << p.ready;
}

int TCP::Receive(sf::TcpSocket& socket){
	
	socket.setBlocking(false);
	
	sf::Packet packet;
	
	//std::cout << "TRY RECEIVE SERVER" << std::endl;
	sf::Socket::Status status = socket.receive(packet);
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
		packet >> g.p2_draw_card_number >> g.p2_hand_card_number >> g.p2;
		std::cout << "RECEIVED FOLLOWING DATA: " << std::endl;
		std::cout << "HAND CARDS: " << std::endl;
		for(int i = 0; i < 4; ++i){
			std::cout << g.p2.hand[i] << std::endl;
		}
		std::cout << "P2 Hand card number: " << g.p2_hand_card_number << std::endl;
		std::cout << "END OF RECEIVE METHOD!" << std::endl;
		g.received = true;
	}
	return 0;
}

int TCP::Send(sf::TcpSocket& socket){
	
	sf::Packet packet;
	socket.setBlocking(false);
	
	packet << g.p1_draw_card_number << g.p1_hand_card_number << g.p1;
	
	if(socket.send(packet) != sf::Socket::Done){
		std::cout << "Couldn't send" << std::endl;
	} else {
		std::cout << "SENT!" << std::endl;
		std::cout << "SENT FOLLOWING DATA: " << std::endl;
		std::cout << "HAND CARDS: " << std::endl;
		for(int i = 0; i < 4; ++i){
			std::cout << g.p1.hand[i] << std::endl;
		}
		std::cout << "P1 Hand card number: " << g.p1_hand_card_number << std::endl;
		std::cout << "END OF SEND METHOD!" << std::endl;
		// Reset the data after sending
		g.p2_hand_card_number = 999;
		g.p2_draw_card_number = 999;
		g.p1_hand_card_number = 999;
		g.p1_draw_card_number = 999;
		g.p1.end_turn = false;
	}
	g.ready_to_send = false;
	return 0;
}

int TCP::Listen(){
	// Bind listner to a port
	if(listener.listen(25565) != sf::Socket::Done){
		std::cout  <<  "NO connections" << std::endl;
		return 0;
	}
	// accept connection
	if(listener.accept(client) != sf::Socket::Done){
		std::cout  << "Something went wrong \n";
		return 0;
	} else {
		std::cout << "Connected! >> " << client.getRemoteAddress() << std::endl;
		g.current_state = States::player_one_turn;
		client_connected = true;
		is_client = false;
		return 1;
	}
}

int TCP::Connect(){
	
	sf::Socket::Status status = socket.connect(server_adress, 25565);
	
	if(status != sf::Socket::Done){
		std::cout  << "Could not connect" << std::endl;
		return 0;
	} else {
		std::cout  << "Connected to " << server_adress << std::endl;
		client_connected = true;
		g.current_state = States::player_two_turn;
		g.is_client = true;
		is_client = true;
		return 1;
	}
}

void TCP::Net(){

	clock.restart();

	while(server_is_on){
		while(elapsed.asSeconds() > tick){
			
			if(!is_client && !client_connected){
				Listen();
			}
			if(is_client && !client_connected){
				Connect();
			}
			if(client_connected){
				if(g.ready_to_send){
					if(is_client){
						Send(socket);						
					} else if(!is_client){
						Send(client);
					}
					g.ready_to_send = false;
				}
				if(is_client){
					Receive(socket);						
				} else if(!is_client){
					Receive(client);
				}
			}
			elapsed -= sf::seconds(tick);
		}
		elapsed += clock.restart();
	}
	socket.disconnect();
	client.disconnect();
}

UDP::UDP(){
	// Setting up UDP socket to find servers and answer to clients
	//udp_socket.bind(25565);
	//udp_socket.setBlocking(false);
}

sf::IpAddress UDP::UDP_client(){
	udp_socket.bind(sf::Socket::AnyPort);
	udp_socket.setBlocking(false);
	while(!cancel){
		status = udp_socket.send(udp_packet, sf::IpAddress::Broadcast, 25565);
		if(status == sf::Socket::Error){
			std::cout << "ERROR" << std::endl;
			sf::sleep(sf::seconds(2.0f));
		}
		if(status != sf::Socket::Done)
		{
			std::cout << "Couldn't find server" << std::endl;
			continue;
		} else {
			if (udp_socket.receive(udp_packet, sender_adress, port) != sf::Socket::Done)
			{
				continue;
			} else {
				host_found = true;
				cancel = true;
				std::cout << sender_adress << std::endl;
				return sender_adress;
			}
		}
	}
}

sf::IpAddress UDP::UDP_server(){
	udp_socket.bind(25565);
	udp_socket.setBlocking(false);
	while(!cancel){
	
		if (udp_socket.receive(udp_packet, sender_adress, port) != sf::Socket::Done)
		{
			std::cout << "DID NOT FIND CLIENT" << std::endl;
			sf::sleep(sf::seconds(2.0f));
			continue;
		} else {
			if (udp_socket.send(udp_packet, sender_adress, port) != sf::Socket::Done)
			{
				continue;
			} else{
				std::cout << "FOUND CLIENT" << std::endl;
				host_found = true;
				cancel = true;
				return sender_adress;
			}
		}
	}
}

void UDP::Run()
{
	std::cout << "UDP active" << std::endl;
	cancel = false;
	if(client){
		UDP_client();
		return;
	} else {
		UDP_server();
	}
	udp_socket.unbind();
}