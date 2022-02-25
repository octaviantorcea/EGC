#include "Bullet.h"
#include <iostream>

Bullet::Bullet(Player player) {
	this->TTL = 0;
	this->speed = 15;
	this->isAlive = true;
	this->scale = 0.5f;
	this->length = scale;
	this->xCoord = player.getXCoord();
	this->yCoord = player.getYCoord();
	this->angle = player.getAngle();
}

Bullet::~Bullet() {
}

float Bullet::getXCoord() {
	return xCoord;
}

float Bullet::getYCoord() {
	return yCoord;
}

float Bullet::getTTL() {
	return TTL;
}

float Bullet::getSpeed() {
	return speed;
}

float Bullet::getAngle() {
	return angle;
}

float Bullet::getScale() {
	return scale;
}

bool Bullet::exists() {
	return isAlive;
}

/// <summary>
///		moves the bullet with "mod" and checks all the collisions and their effects
/// </summary>
/// <param name="mod"> = deltaTimeSeconds</param>
/// <param name="obstacles"> = vector that contains all the obstacles</param>
/// <param name="border"> = map border</param>
/// <param name="enemies"> = vector that contains all the enemies</param>
/// <param name="player"> = player object</param>
void Bullet::step(float mod, std::vector<Obstacle> obstacles, Border border, std::vector<Enemy*> &enemies, Player &player) {
	xCoord += (float) cos(angle) * mod * speed;
	yCoord += (float) sin(angle) * mod * speed;
	TTL += mod;

	if (hitsEnemy(enemies)) {
		isAlive = false;
		player.modScore(1);

		if (player.getScore() >= player.getMaxScore()){
			std::cout << "You won! To restart press R...\n";
			enemies.clear();
		}
	}

	if (TTL > MAXTTL || hitsObstacle(obstacles) || hitsBorder(border)) {
		isAlive = false;
	}
}

/// <summary>
///		checks if the bullet hits an obstacle
/// </summary>
/// <param name="obstacles"> = vector that contains all the obstacles</param>
/// <returns> true or false</returns>
bool Bullet::hitsObstacle(std::vector<Obstacle> obstacles) {
	for (auto obstacle : obstacles) {
		if (xCoord + length / 2 > obstacle.getXMin() &&
			xCoord - length / 2 < obstacle.getXMax() &&
			yCoord + length / 2 > obstacle.getYMin() &&
			yCoord - length / 2 < obstacle.getYMax()) {
			return true;
		}
	}

	return false;
}

/// <summary>
///		checks if the bullet hits map border
/// </summary>
/// <param name="border"> = map border</param>
/// <returns> true or false</returns>
bool Bullet::hitsBorder(Border border) {
	if (xCoord + length / 2 > border.getXMax() ||
		xCoord - length / 2 < border.getXMin() ||
		yCoord + length / 2 > border.getYMax() ||
		yCoord - length / 2 < border.getYMin()) {
		return true;
	}

	return false;
}

/// <summary>
///		checks if bullet hits an enemy.
///		if it hits, the enemy is killed
/// </summary>
/// <param name="enemies"> = vector that contains all the enemies</param>
/// <returns></returns>
bool Bullet::hitsEnemy(std::vector<Enemy*> enemies) {
	for (auto enemy : enemies) {
		if (xCoord + length / 2 > enemy->getXCoord() - enemy->getLength() / 2 &&
			xCoord - length / 2 < enemy->getXCoord() + enemy->getLength() / 2 &&
			yCoord + length / 2 > enemy->getYCoord() - enemy->getLength() / 2 &&
			yCoord - length / 2 < enemy->getYCoord() + enemy->getLength() / 2) {
			enemy->kill();
			return true;
		}
	}

	return false;
}
