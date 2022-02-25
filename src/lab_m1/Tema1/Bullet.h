#pragma once

#include "Player.h"
#include "Enemy.h"

#define MAXTTL 1.25

class Bullet {
public:
	Bullet(Player player);
	~Bullet();

	float getXCoord();
	float getYCoord();
	float getTTL();
	float getSpeed();
	float getAngle();
	float getScale();
	bool exists();

	void step(float mod, std::vector<Obstacle> obstacles, Border border, std::vector<Enemy*> &enemies, Player &player);
	bool hitsObstacle(std::vector<Obstacle> obstacles);
	bool hitsBorder(Border border);
	bool hitsEnemy(std::vector<Enemy*> enemies);

private:
	float xCoord, yCoord;
	float TTL;
	float speed;
	float angle;
	bool isAlive;
	float scale;
	float length;
};
