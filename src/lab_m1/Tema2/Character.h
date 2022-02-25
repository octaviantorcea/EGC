#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "Map.h"

using namespace std;
using namespace glm;

class Character {
public:
	Character();
	Character(glm::vec3 startPos);
	~Character();

	void moveForward(float mod);
	void moveRight(float mod);
	void modAngleOY(float mod);
	void modAngleOX(float mod);
	void setAngleOX(float angle);
	void setTTF(float ttf);
	void modTTF(float mod);
	void decLives();

	float getHeight();
	float getWidth();
	float getLength();
	float getAngleOY();
	float getAngleOX();
	float getSpeed();
	glm::vec3 getPosition();
	float getTTF();
	float getFireRate();
	float getLives();

	bool hitsWallForward(vector<Room*> fullRooms, float mod);
	bool hitsWallRight(vector<Room*> fullRooms, float mod);
	bool won();

private:
	float height, width, length, angleOY, angleOX, speed, TTF, fireRate, lives;
	glm::vec3 centerPosition;
};
