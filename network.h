#ifndef network_h
#define network_h

	extern int Send();
	extern int Receive();

	sf::Thread send_thread(&Send);
	//sf::Thread receive_thread(&Receive);	

#endif