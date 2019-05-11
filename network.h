#pragma once
#include "Game.h"
#include <SFML/Network.hpp>

struct TCP
{
	Game& g;
	TCP(Game& game) : g(game) {}
	
	bool server_is_on = true;
	bool client_connected = false;
	bool is_client = false;
	int server_mode = -1;
	
	// Client
	sf::TcpSocket socket;
	
	// Server
	sf::TcpListener listener;
	sf::TcpSocket client;
	
	sf::Clock clock;
	sf::Time elapsed;
	float tick = 1.0/60.0;
	
	sf::IpAddress server_adress;
	
	int Connect();
	int Listen();
	int Send(sf::TcpSocket& socket);
	int Receive(sf::TcpSocket& socket);
	void Net();
};

struct UDP{
	void Run();
	sf::IpAddress UDP_client();
	sf::IpAddress UDP_server();
	sf::UdpSocket udp_socket;
	sf::Packet udp_packet;
	sf::IpAddress sender_adress;
	unsigned short port;
	
	bool host_found = false;
	bool cancel = false;
	bool client = false;
	
	sf::Socket::Status status;
	UDP();
};