#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include "constants.h"


struct Point {
	float x;
	float y;

	float distanceSq(Point p) {
		return pow(x - p.x, 2) + pow(y - p.y, 2);
	}
	float distance(Point p) {
		return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
	}
	float mag() {
		float value = x * x + y * y;
		return sqrt(value);
	}
	void normalize() {
		if (mag() == 0) return;
		*this /= mag();
	}
	void scale(float scalar) {
		this->normalize();
		*this *= scalar;
	}
	Point& operator /= (float divisor) {
		x /= divisor;
		y /= divisor;
		return *this;
	}
	Point& operator *= (float mult) {
		x *= mult;
		y *= mult;
		return *this;
	}
	float angle() {
		return (atan2(y, x) * 180 / PI) + 90.f;
	}

};

const Point CENTER = { SCREEN_WIDTH / 2.f,SCREEN_HEIGHT / 2.f };
const Point ZERO = { 0,0 };


enum SpaceObjType { SHIP, SHIP_EXPLODING, SHIP_GONE, ASTEROID, ASTEROID_GONE, PHOTON_TORPEDO, TORPEDO_EXPLODING, TORPEDO_GONE };

/*  External Pointers to hold all new textures, shaders, and clocks used by SpaceObjects
	Used so not every object holds its own texture, which is slow, and so textures don't need to be passed
	into an object as an arg from the main program on creation*/
extern sf::Texture* shipTexture;
extern sf::Texture* asteroidTexture;
extern sf::Texture* torpedoTexture;
extern sf::Texture* boostTexture;
extern sf::Texture* distortionMap;
extern sf::Shader* heatShader;
extern sf::Clock* timer;

class SpaceObject {

public:

	/*---------------------------------
	*	Description Default constructor, creates an ASTEROID type SpaceObject
	*--------------------------------*/
	SpaceObject();

	/*---------------------------------
	*	Description SpaceObject constructor
	*	@param type - type of SpaceObject
	*	@param radius - radius of SpaceObject
	*	@param location - Point location of SpaceObject
	*	@param velocity - Point velocity of SpaceObject
	*	@param angle - angle of SpaceObject
	*--------------------------------*/
	SpaceObject(SpaceObjType type, float radius,
		Point location, Point velocity, float angle);
	//=============================================
	//mutators

	/*---------------------------------
	*	Description Sets radius to a given value, beween 0-screen size/2
	*	@param radius - radius to set
	*--------------------------------*/
	void setRadius(float radius);

	/*---------------------------------
	*	Description Sets SpaceObject location to a given x,y location, between 0-screen size
	*	@param x - x location
	*	@param y - y location
	*--------------------------------*/
	void setLocation(float x, float y);

	/*---------------------------------
	*	Description Sets SpaceObject velocity to a given vector limited relative to its size, between -MAX_MOMENTUM - MAX_MOMENTUM
	*	@param velocityX - x velocity
	*	@param velocityY - y velocity
	*--------------------------------*/
	void setVelocity(float velocityX, float velocityY);

	/*---------------------------------
	*	Description Sets SpaceObject angle to given angle, between 0 - 359 degrees
	*	@param angDeg - angle in degrees
	*--------------------------------*/
	void setAngle(float angDeg);

	/*---------------------------------
	*	Description Changes SpaceObject angle by a given number of degrees
	*	@param deltaDeg - angle to change by
	*--------------------------------*/
	void changeAngle(float deltaDeg);

	/*---------------------------------
	*	Description Sets a given texture to the sprite of a SpaceObject
	*	@param texture - texture to set
	*--------------------------------*/
	void setTexture(sf::Texture& texture);
	
	/*---------------------------------
	*	Description Increases velocity of a SpaceObject in the direction it is heading
	*	@param thrust - amount to increase the velocity vector - default value ENGINE_THRUST
	*--------------------------------*/
	void applyThrust(float thrust = ENGINE_THRUST);

	/*---------------------------------
	*	Description Set a SHIP type SpaceObject to SHIP_exploded, and starts explosion graphic 
	*--------------------------------*/
	void explode();


	/*---------------------------------
	*	Description Respawns a SHIP type SpaceObject
	*--------------------------------*/
	void respawn();

	/*---------------------------------
	*	Description Fires a Photon Torpedo
	*	@param torpedos[] - an array of pointers to PHOTON_TORPEDO type SpaceObjects
	*--------------------------------*/
	void fireTorpedo(SpaceObject* torpedos[]);

	/*---------------------------------
	*	description Splits an asteroid into two smaller ones
	*	@param asteroids[] - an array of pointers to ASTEROID type SpaceObjects
	*--------------------------------*/
	void splitAsteroid(SpaceObject* asteroids[]);

	//============================================
	//accessors

	/*---------------------------------
	*	Description Returns the radius of a SpaceObject
	*	@return radius
	*--------------------------------*/
	float getRadius() const;

	/*---------------------------------
	*	Description Returns the location of a SpaceObject as a Point
	*	@return location
	*--------------------------------*/
	Point getLocation() const;

	/*---------------------------------
	*	Description Returns the velocity of a SpaceObject
	*	@return velocity
	*--------------------------------*/
	Point getVelocity() const;

	/*---------------------------------
	*	Description Returns the angle of a SpaceObject
	*	@return angle
	*--------------------------------*/
	float getAngle() const;

	/*---------------------------------
	*	Description Returns whether a SpaceObject is alive or not
	*	@return bool, true if alive, false if not
	*--------------------------------*/
	bool isAlive() const;

	/*---------------------------------
	*	Description Returns whether a SHIP type SpaceObject can fire a torpedo
	*	@return bool, true if can, false if can't
	*--------------------------------*/
	bool canFire();


	//============================================
	//others

	/*---------------------------------
	*	Description Updates the position of a SpaceObject
	*--------------------------------*/
	void update();

	/*---------------------------------
	*	Description Checks for collisions between a SpaceObject and an array of SpaceObjects
	*	@param objects[] - Array of SpaceObject pointers
	*--------------------------------*/
	void checkCollisions(SpaceObject* objects[]);

	/*---------------------------------
	*	Description Renders a SpaceObject to the SFML window passed as a param
	*	@param win - SFML render window to draw on
	*--------------------------------*/
	void display(sf::RenderWindow& win);

	/*---------------------------------
	*	Description Returns a random point on the screen
	*	@param  avoidCenter - flag that avoids the center of screen if true, so ship doesn't instantly explode
	*	@return Random point
	*--------------------------------*/
	Point randomPoint(bool avoidCenter = true);

	/*---------------------------------
	*	Description Overload - Returns a random point within a user defined range
	*	@param min - minimum point value
	*	@param max - maximum point value
	*	@return Random point
	*--------------------------------*/
	Point randomPoint(float min, float max);

	/*---------------------------------
	*	Description Returns a random float between user specified range
	*	@param min - minimum value
	*	@param max - maximum value
	*	@return Random float
	*--------------------------------*/
	float random(float min, float max);


private:
	SpaceObjType type;		  //type of object
	Point location;			  //current location (x,y)
	Point velocity;			  //current velocity (in pixels/frame)
	float angleDeg;			  //angle object is facing (in degrees)
	float angVel;			  //current angular velocity (in degrees)
	float radius;			  //gross radius of object (for collision detection)
	int frameLife;			  //frames to keep an object alive
	int framesBetweenShots;	  //Frames until you can fire another shot
	bool boosting;			  //True if currently boosting, false if not
	sf::CircleShape shape;	  //holds a circle object to display
	sf::Sprite sprite;
	sf::Sprite effects;
	//sf::Texture texture;	  //holds a pointer to the texture to display on the circle
	
};
