#include "MainMenu.h"
#include <fstream>

MainMenu::MainMenu(sf::RenderWindow& w, UDP& udp, TCP& tcp) : window(w), tcp_net(tcp), udp_net(udp){

	Button btn_join_game(sf::Vector2f(181, 29), sf::Vector2f(209, 187), SubMenus::join_game);
	Button btn_host_game(sf::Vector2f(181, 29), sf::Vector2f(209, 225), SubMenus::host_game);
	Button btn_opt(sf::Vector2f(181, 29), sf::Vector2f(209, 255), SubMenus::deck_builder);
	Button btn_quit(sf::Vector2f(181, 29), sf::Vector2f(209, 293), SubMenus::host_game);
	Button btn_cancel_game(sf::Vector2f(110, 25), sf::Vector2f(246, 270), SubMenus::main_menu);
	// Empty button to fill up the space
	Button btn_empty;
	
	texture_bg.loadFromFile("textures/MainMenu.png");
	texture_hjgame_waiting.loadFromFile("textures/MainMenu_StartGame_waiting.png");
	texture_hjgame_waiting_hl.loadFromFile("textures/MainMenu_StartGame_waiting_hl.png");
	
	texture_bg_btn_JGhl.loadFromFile("textures/MainMenu_JG.png");
	texture_bg_btn_HGhl.loadFromFile("textures/MainMenu_HG.png");;
	texture_bg_btn_OPThl.loadFromFile("textures/MainMenu_OPT.png");;
	texture_bg_btn_QThl.loadFromFile("textures/MainMenu_QT.png");;
	
	
	spr_backGround.setTexture(texture_bg);
	//spr_backGround.setScale(600/spr_backGround.getLocalBounds().width, 
	//500/spr_backGround.getLocalBounds().height);
	
	buttons_MM = {btn_join_game, btn_host_game, btn_opt, btn_quit};
	buttons_HJG = {btn_empty, btn_cancel_game};
	
	MM_textures = {texture_bg_btn_JGhl, texture_bg_btn_HGhl, texture_bg_btn_OPThl, texture_bg_btn_QThl};
	
	HJG_textures = {texture_hjgame_waiting, texture_hjgame_waiting_hl};
	
	warning_texture.loadFromFile("textures/warning.png");
	popUp.setTexture(warning_texture);
	popUp.setPosition(187, 225);
	curr_subMenu = main_menu;
}

bool MainMenu::Is_deck_ready()
{
	// We can't start the game until player sets up his deck 
	std::fstream deck_file;
	deck_file.open("deck");
		
	if(deck_file.fail()){
		return false;
	}
	if(deck_file.good()){
		return true;
	}
}

void MainMenu::Render()
{
	window.clear(sf::Color::Red);
	window.draw(spr_backGround);
	for(size_t i = 0; i < curr_subm_buttons.size(); ++i){
		//window.draw(curr_subm_buttons[i]);
	}
	if(b_popUp){
		window.draw(popUp);
	}
	
	window.display();
}

void MainMenu::Update(){
	switch(curr_subMenu){
		case SubMenus::main_menu :
		{
			// Cancel everything out in case we switched from another state (host_game or join_game)
			start_game = false;
			//
			curr_subm_buttons = buttons_MM;
			curr_subm_textures = MM_textures;
		}
		break;
		case SubMenus::deck_builder :
		{
			
		}
		break;
		case SubMenus::host_game :
		{
			if(!Is_deck_ready()){
				// Show message
				curr_subMenu = SubMenus::main_menu;
				b_popUp = true;
				break;
			}
			start_game = true;
			curr_subm_textures = HJG_textures;
			curr_subm_buttons = buttons_HJG;
			udp_net.client = false;
			tcp_net.server_mode = 1;
			tcp_net.is_client = false;
		}
		break;
		case SubMenus::join_game : 
		{
			if(!Is_deck_ready()){
				// Show message
				curr_subMenu = SubMenus::main_menu;
				b_popUp = true;
				break;
			}
			start_game = true;
			curr_subm_textures = HJG_textures;
			curr_subm_buttons = buttons_HJG;
			udp_net.client = true;
			tcp_net.server_mode = 0;
			tcp_net.is_client = true;
		}
	}
	HandleInput();
}

void MainMenu::HandleInput()
{
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		btn_was_released = true;
	}
	
	sf::Vector2f mp = window.mapPixelToCoords(static_cast<sf::Vector2i>(mouse_position));
	for(size_t i = 0; i < curr_subm_buttons.size(); ++i){
		if(curr_subm_buttons[i].btn_shape.getGlobalBounds().contains(mp)){
	
			spr_backGround.setTexture(curr_subm_textures[i]);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
				curr_subMenu = curr_subm_buttons[i].OnClick();
				btn_was_released = false;
			}
		}
	}
	if(popUp.getGlobalBounds().contains(mp)){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && btn_was_released){
			b_popUp = false;
			btn_was_released = false;
		}
	}
}