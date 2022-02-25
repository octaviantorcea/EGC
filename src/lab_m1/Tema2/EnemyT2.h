#pragma once

#include "glm/glm.hpp"
#include "Map.h"
#include "Character.h"

using namespace glm;

class EnemyT2 {
public:
	EnemyT2();
	~EnemyT2();

	EnemyT2(Room* room);

	glm::vec3 getPosition();
	bool isAlive();
	void move(float deltaTimeSeconds);
	float getRadius();
	float getHitTime();
	int getIsHit();

	void kill();
	void hitsPlayer(Character* player);
	void setHit();
	void decHitTime(float mod);

private:
	glm::vec3 position;
	float xMin, zMin, xMax, zMax;
	bool alive;
	float radius;
	float speed;
	int isHit;
	float hitTime;
};
