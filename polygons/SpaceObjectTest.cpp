/* --------------------------------------------------------
* @file    Polygons
* @author  Anthony Major
* @version 1.0
*
* CS162-01 - Assignment 2.1
*	Low budget Asteroids knock-off
*
* ------------------------------------------------------------ */

#include "spaceObject.h"


// External Textures
sf::Texture* shipTexture;
sf::Texture* asteroidTexture;
sf::Texture* torpedoTexture;
sf::Texture* boostTexture;
sf::Texture* distortionMap;
sf::Shader* heatShader;
sf::Clock* timer;
void setObjectTextures();


int main() {

	setObjectTextures();

	//View Window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Polygons");
	sf::View view(sf::FloatRect(30.f, 30.f, SCREEN_WIDTH - 60.f, SCREEN_HEIGHT - 60.f));
	window.setView(view);
	window.setFramerateLimit(60);

	//Background
	sf::Texture backgroundTexture;
	sf::Sprite background;
	background.setColor(sf::Color(100, 100, 100));
	backgroundTexture.loadFromFile("background.png");
	background.setTexture(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	//Create Ship Object
	SpaceObject* ship = new SpaceObject(SHIP, 20, CENTER, ZERO, 0);
	
	
	//Create Asteroids
	SpaceObject* asteroids[MAX_ASTEROIDS] = { 0 };
	for (int i = 0; i < 5; ++i) {
		asteroids[i] = new SpaceObject;
	}

	//Create Torpedo Array
	SpaceObject* torpedos[MAX_TORPEDOS] = { 0 };
	
	//Main Game Loop
	while (window.isOpen())
	{
		//Event Checking
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//User inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && ship->isAlive())
			ship->changeAngle(-TURN_RATE);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && ship->isAlive())
			ship->changeAngle(TURN_RATE);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && ship->isAlive())
			ship->applyThrust();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ship->canFire())
			ship->fireTorpedo(torpedos);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !ship->isAlive())
			ship->respawn();

		//Update positions
		ship->update();
		for (int i = 0; i < MAX_ASTEROIDS; ++i) {
			if (asteroids[i] != NULL) {
				asteroids[i]->update();
				if (!asteroids[i]->isAlive()) {
					delete asteroids[i];
					asteroids[i] = NULL;
				}
			}
		}
		for (int i = 0; i < MAX_TORPEDOS; ++i) {
			if (torpedos[i] != NULL) {
				torpedos[i]->update();
				if (!torpedos[i]->isAlive()) {
					delete torpedos[i];
					torpedos[i] = NULL;
				}
			}
		}

		//Collision checking
		ship->checkCollisions(asteroids);
		for (int i = 0; i < MAX_TORPEDOS; ++i) {
			if (torpedos[i] != NULL)
				torpedos[i]->checkCollisions(asteroids);
		}

		//Display
		window.clear();
		window.draw(background);
		for (int i = 0; i < MAX_ASTEROIDS; ++i) {
			if (asteroids[i] != NULL)
				asteroids[i]->display(window);
		}	
		for (int i = 0; i < MAX_TORPEDOS; ++i) {
			if (torpedos[i] != NULL)
				torpedos[i]->display(window);
		}
		ship->display(window);
		window.display();
	}

	return 0;
}