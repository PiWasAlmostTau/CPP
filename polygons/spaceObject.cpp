#pragma warning(disable : 4996)
#include "spaceObject.h"
	/*--------------------------------------------------------------*/
	SpaceObject::SpaceObject() {
		type = ASTEROID;
		location = this->randomPoint();
		velocity = this->randomPoint(-START_VELOCITY, START_VELOCITY);
		angleDeg = random(0,359);
		angVel = random(-RANDOM_SPIN, RANDOM_SPIN);
		setRadius(START_SIZE);
		frameLife = 0;
		setTexture(*asteroidTexture);
		boosting = false;
	}

	/*--------------------------------------------------------------*/
	SpaceObject::SpaceObject(SpaceObjType type, float radius,
		Point location, Point velocity, float angle = 0) {

		this->type = type;
		setRadius(radius);
		setAngle(angle);
		angVel = 0;
		frameLife = 0;
		setLocation(location.x, location.y);
		setVelocity(velocity.x, velocity.y);
		boosting = false;

		if (type == SHIP) {
			setTexture(*shipTexture);
			shape.setRadius(radius);
			shape.setFillColor(sf::Color::Transparent);
			shape.setOutlineColor(sf::Color::White);
			shape.setOutlineThickness(EXPLOSION_THICKNESS);
			framesBetweenShots = 0;
			effects.setTexture(*boostTexture);
			effects.setOrigin(56.f, -20.f);
			effects.scale(0.18f, 0.18f);
			
			
		}

		if (type == PHOTON_TORPEDO) {
			setTexture(*torpedoTexture);
			frameLife = TORPEDO_LIFE;
		}
		if (type == ASTEROID) {
			setTexture(*asteroidTexture);
			angVel = random(-RANDOM_SPIN, RANDOM_SPIN);
		}
	}

	void setObjectTextures() {
		//Ship textures
		shipTexture = new sf::Texture;
		shipTexture->loadFromFile("ship.png");
		shipTexture->setSmooth(true);
		boostTexture = new sf::Texture;
		boostTexture->loadFromFile("boost.png");
		boostTexture->setSmooth(true);
		//shader
		distortionMap = new sf::Texture;
		distortionMap->loadFromFile("distortion_map.png");
		distortionMap->setRepeated(true);
		distortionMap->setSmooth(true);

		heatShader = new sf::Shader;
		heatShader->loadFromFile("example_001.frag", sf::Shader::Fragment);
		heatShader->setParameter("currentTexture", sf::Shader::CurrentTexture);
		heatShader->setParameter("distortionMapTexture", *distortionMap);
		heatShader->setParameter("distortionFactor", 0.6f);
		heatShader->setParameter("riseFactor", 1.5f);
		timer = new sf::Clock;

		//Asteroid Textures
		asteroidTexture = new sf::Texture;
		asteroidTexture->loadFromFile("rock.png");
		asteroidTexture->setSmooth(true);

		torpedoTexture = new sf::Texture;
		torpedoTexture->loadFromFile("torpedo.png");
		torpedoTexture->setSmooth(true);
	}

	//==========================Mutators===============================

	void SpaceObject::setRadius(float radius) {
		if (radius < 0 || radius > SCREEN_HEIGHT / 2)
			this->radius = STANDARD_RADIUS;
		else
			this->radius = radius;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::setLocation(float x, float y) {
		this->location = { x, y };
		float maxX = (float) SCREEN_WIDTH;
		float maxY = (float) SCREEN_HEIGHT;
		while (this->location.x > maxX)
			this->location.x -= maxX;
		while (this->location.x < 0.f)
			this->location.x += maxX;
		
		while (this->location.y > maxY)
			this->location.y -= maxY;
		while (this->location.y < 0.f)
			this->location.y += maxY;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::setVelocity(float velocityX, float velocityY) {
		velocity = { velocityX , velocityY };
		float magV = velocity.mag();
		float maxV = MAX_MOMENTUM / radius;

		if (magV > maxV) 
			velocity.scale(maxV);
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::setAngle(float angDeg) {
		this->angleDeg = angDeg;

		if (this->angleDeg >= 360.f)
			this->angleDeg -= 360.f;
		else if (this->angleDeg < 0.f)
			this->angleDeg += 360.f;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::changeAngle(float deltaDeg) {
		float newAngle = angleDeg + deltaDeg;
		setAngle(newAngle);
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::setTexture(sf::Texture& texture) {
		this->sprite.setTexture(texture);
		sf::FloatRect size = this->sprite.getLocalBounds();
		float scaleX = this->radius * 2.f / size.width;
		float scaleY = this->radius * 2.f / size.height;
		if (type == PHOTON_TORPEDO) {
			this->sprite.setOrigin(PIX_RADIUS, PIX_RADIUS);
			scaleY = scaleX;
		}
		else {
			this->sprite.setOrigin(size.width / 2.f, size.height / 2.f);
		}
		
		this->sprite.scale(scaleX, scaleY);
		
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::applyThrust(float thrust) {
		float forcex = cos((angleDeg - 90)*PI / 180) * thrust;
		float forcey = sin((angleDeg - 90)*PI / 180) * thrust;
		setVelocity(velocity.x + forcex, velocity.y + forcey);
		boosting = true;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::explode() {
		this->type = SHIP_EXPLODING;
		this->frameLife = 100;
		this->velocity = { 0.f, 0.f };
		this->shape.setPosition(location.x, location.y);
		std::cout << "Ship Destroyed" << std::endl;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::respawn() {
		if (this->type == SHIP_GONE) {
			this->type = SHIP;
			this->location = CENTER;
			this->angleDeg = 0;
			this->shape.setOutlineThickness(10.f);
		}
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::fireTorpedo(SpaceObject* torpedos[]) {

		int i = 0;
		bool check = true;
		while (i < MAX_TORPEDOS && check){

			if (torpedos[i] == NULL) {
				framesBetweenShots = FRAMES_BETWEEN;
				torpedos[i] = new SpaceObject(PHOTON_TORPEDO, TOROPEDO_SIZE, this->location , ZERO, this->angleDeg);
				torpedos[i]->applyThrust(TORPEDO_VELOCITY);
				torpedos[i]->update();
				check = false;
			}
			i++;
		}
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::splitAsteroid(SpaceObject* asteroids[]) {

		if (this->radius < MIN_SIZE)
			return;
		float spawnVel = random(velocity.mag(), velocity.mag() * SIZE_MULT);
		float deltaAng = random(ANG_CHANGE_MIN, ANG_CHANGE_MAX);
		int i = 0;
		int check = 2;
		while (i < MAX_ASTEROIDS && check) {

			if (asteroids[i] == NULL) {
				asteroids[i] = new SpaceObject(ASTEROID, this->radius/1.8f, this->location, ZERO, this->velocity.angle());
				asteroids[i]->changeAngle((check == 2) ? -deltaAng : deltaAng);
				asteroids[i]->applyThrust(spawnVel);
				asteroids[i]->update();
				check--;
			}
			i++;
		}
	}

	//============================================
	//accessors
	float SpaceObject::getRadius() const {
		return radius;
	}
	/*--------------------------------------------------------------*/
	Point SpaceObject::getLocation() const {
		return location;
	}
	/*--------------------------------------------------------------*/
	Point SpaceObject::getVelocity() const {
		return velocity;
	}

	/*--------------------------------------------------------------*/
	float SpaceObject::getAngle() const {
		return angleDeg;
	}

	/*--------------------------------------------------------------*/
	bool SpaceObject::isAlive() const {
		if (this->type == SHIP || this->type == PHOTON_TORPEDO || this->type == ASTEROID)
			return true;
		else
			return false;
	}
	/*--------------------------------------------------------------*/
	bool SpaceObject::canFire() {
		if (framesBetweenShots <= 0 && this->isAlive()) {
			return true;
		}
		else
			return false;
	}

	//============================================
	//others
	void SpaceObject::update() {
		setLocation(location.x + velocity.x, location.y + velocity.y);
		sprite.setPosition(location.x, location.y);

		setAngle(angleDeg + angVel);
		sprite.setRotation(angleDeg);

		if (type == SHIP) {
			heatShader->setParameter("time", timer->getElapsedTime().asSeconds());
			if (boosting) {
				effects.setPosition(location.x, location.y);
				effects.setRotation(angleDeg);
			}
		}
		if (framesBetweenShots > 0)
			framesBetweenShots--;

		if (this->frameLife > 0)
			this->frameLife--;

		if (this->frameLife <= 0)
			if (type == SHIP_EXPLODING)
				this->type = SHIP_GONE;
			else if (type == PHOTON_TORPEDO)
				this->type = TORPEDO_GONE;
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::checkCollisions(SpaceObject* objects[]) {
		
		for (int i = 0; i < MAX_ASTEROIDS; ++i) {
			if (objects[i] != NULL) {
				float collisionDistanceSq = pow((this->radius + objects[i]->radius), 2);
				float distanceSq =  pow((this->location.x - objects[i]->location.x), 2)
					              + pow((this->location.y - objects[i]->location.y), 2);
				if (distanceSq <= collisionDistanceSq) {
					if (this->type == SHIP)
						this->explode();
					else if (this->type == PHOTON_TORPEDO) {
						this->type = TORPEDO_GONE;
						objects[i]->splitAsteroid((objects));
						objects[i]->type = ASTEROID_GONE;
					}
				}
			}
		}
	}

	/*--------------------------------------------------------------*/
	void SpaceObject::display(sf::RenderWindow& win){
		if (this->type == SHIP_GONE)
			return;

		if (type == SHIP) {
			if (boosting) {
				win.draw(effects,heatShader);
				boosting = false;
			}
		}

		if (this->type == SHIP_EXPLODING) {
			float explosionSize = (EXPLOSION_TIME + EXPLOSION_THICKNESS) - frameLife;

			this->shape.setRadius(explosionSize);
			this->shape.setOrigin(explosionSize, explosionSize);
			win.draw(shape);

			this->shape.setOutlineThickness(shape.getOutlineThickness() - (EXPLOSION_THICKNESS / EXPLOSION_TIME));
		}
		else if (type == PHOTON_TORPEDO)
			win.draw(sprite, heatShader);
		else
			win.draw(sprite);
	}

	/*--------------------------------------------------------------*/
	Point SpaceObject::randomPoint(bool avoidCenter) {
		Point result;
		do {
			result.x = random(0, SCREEN_WIDTH);
			result.y = random(0, SCREEN_HEIGHT);
			if (result.distance(CENTER) > 100.f)
				avoidCenter = false;
		} while (avoidCenter);
		
		return result;
	}

	/*--------------------------------------------------------------*/
	Point SpaceObject::randomPoint(float min, float max) {
		float x = random(min, max);
		float y = random(min, max);
		Point result = { x, y };
		return result;
	}

	/*--------------------------------------------------------------*/
	float SpaceObject::random(float min, float max) {
		std::random_device rand;
		std::mt19937 generator(rand());
		std::uniform_int_distribution<int> distribution(min * 100, max * 100);
		float num = distribution(generator) / 100.f;
		
		return num;
	}
