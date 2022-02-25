#pragma once

#include "glm/glm.hpp"
#include "Character.h"
#include "EnemyT2.h"
#include <vector>

using namespace std;
using namespace glm;

#define MAXTTL 0.90

class BulletT2 {
public:
	BulletT2();
	~BulletT2();

	BulletT2(Character* player);

	glm::vec3 getPosition();
	float getTTL();
	float getSpeed();
	float getAngleOY();
	float getRadius();
	bool exists();

	void step(float mod, vector<Room*> fullRooms, Character* player, vector<EnemyT2*> enemies);
	bool hitsWall(vector<Room*> fullRooms);
	bool hitsEnemy(vector<EnemyT2*> enemies);

private:
	glm::vec3 position;
	float TTL;
	float speed;
	float angleOY;
	float angleOX;
	bool isAlive;
	float radius;
};
