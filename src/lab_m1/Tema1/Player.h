#pragma once

#include "Border.h"
#include "Obstacle.h"
#include <vector>
#include "Enemy.h"

class Player {
public:
	Player();
	~Player();

	float getXCoord();
	float getYCoord();
	float getAngle();
	float getSpeed();
	float getMaxHealth();
	float getHealth();
	float getFireRate();
	float getTTF();
	float getScore();
	float getMaxScore();

	void setXCoord(float mod);
	void setYCoord(float mod);
	void setAngle(float newAngle);
	void modHealth(float mod);
	void modTTF(float mod);
	void setTTF(float newTTF);
	void modScore(float mod);
	void resetScore();
	void resetHealth();
	void resetCoords();

	bool canMoveUp(Border border);
	bool canMoveDown(Border border);
	bool canMoveRight(Border border);
	bool canMoveLeft(Border border);
	bool hitsObstacle(std::vector<Obstacle> obstacles, float stepX, float stepY);
	void hitsEnemy(std::vector<Enemy*> &enemies);

private:
	float xCoord, yCoord;
	float radius;
	float angle;
	float speed;
	float health, maxHealth;
	float fireRate;
	float TTF;
	float score;
	float maxScore;
};
