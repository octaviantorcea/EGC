#include "Player.h"
#include <iostream>

Player::Player() {
	this->xCoord = 0;
	this->yCoord = 0;
	this->radius = 1;
	this->angle = 0;
	this->speed = 5;
	this->maxHealth = 50;
	this->health = 50;
	this->fireRate = 0.85f;
	this->TTF = 0;
	this->score = 0;
	this->maxScore = 20;
}

Player::~Player() {
}

float Player::getXCoord() {
	return xCoord;
}

float Player::getYCoord() {
	return yCoord;
}

void Player::setXCoord(float mod) {
	xCoord += mod;
}

void Player::setYCoord(float mod) {
	yCoord += mod;
}

float Player::getAngle() {
	return angle;
}

float Player::getSpeed() {
	return speed;
}

float Player::getMaxHealth() {
	return maxHealth;
}

float Player::getHealth() {
	return health;
}

float Player::getFireRate() {
	return fireRate;
}

float Player::getTTF() {
	return TTF;
}

float Player::getScore() {
	return score;
}

float Player::getMaxScore() {
	return maxScore;
}

void Player::setAngle(float newAngle) {
	angle = newAngle;
}

void Player::modHealth(float mod) {
	health += mod;
}

void Player::modTTF(float mod) {
	TTF += mod;
}

void Player::setTTF(float newTTF) {
	TTF = newTTF;
}

void Player::modScore(float mod) {
	score += mod;
}

void Player::resetScore() {
	score = 0;
}

void Player::resetHealth() {
	health = maxHealth;
}

void Player::resetCoords() {
	xCoord = 0;
	yCoord = 0;
}

// functions that check if the player hits the map border
bool Player::canMoveUp(Border border) {
	return (yCoord + radius < border.getYMax());
}

bool Player::canMoveDown(Border border) {
	return (yCoord - radius > border.getYMin());
}

bool Player::canMoveRight(Border border) {
	return (xCoord + radius < border.getXMax());
}

bool Player::canMoveLeft(Border border) {
	return (xCoord - radius > border.getXMin());
}

float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

/// <summary>
///		checks if in the next frame the player would hit an obstacle.
///		if it hits, the move is ignored.
///		circle - rectangle collision
/// </summary>
/// <param name="obstacles"> = vector that contains all the obstacles</param>
/// <param name="stepX"> = how much the player moves on the x axis</param>
/// <param name="stepY"> = how much the player moves on the x axis</param>
/// <returns>true or false</returns>
bool Player::hitsObstacle(std::vector<Obstacle> obstacles, float stepX, float stepY) {
	float xCenter = xCoord + stepX;
	float yCenter = yCoord + stepY;

	for (auto obstacle : obstacles) {
		float xDif = xCenter - obstacle.getXCenter();
		float yDif = yCenter - obstacle.getYCenter();

		float xClamped = clamp(xDif, -obstacle.getScaleX() / 2, obstacle.getScaleX() / 2);
		float yClamped = clamp(yDif, -obstacle.getScaleY() / 2, obstacle.getScaleY() / 2);

		float xClosest = obstacle.getXCenter() + xClamped;
		float yClosest = obstacle.getYCenter() + yClamped;

		xDif = xClosest - xCenter;
		yDif = yClosest - yCenter;

		float length = sqrt(xDif * xDif + yDif * yDif);

		if (length < radius) {
			return true;
		}
	}

	return false;
}

/// <summary>
///		checks if the player is hit by any enemy
/// </summary>
/// <param name="enemies"> = vector that contains all the enemies</param>
void Player::hitsEnemy(std::vector<Enemy*> &enemies) {
	for (auto enemy : enemies) {
		float xDif = xCoord - enemy->getXCoord();
		float yDif = yCoord - enemy->getYCoord();

		float xClamped = clamp(xDif, -enemy->getLength() / 2, enemy->getLength() / 2);
		float yClamped = clamp(yDif, -enemy->getLength() / 2, enemy->getLength() / 2);

		float xClosest = enemy->getXCoord() + xClamped;
		float yClosest = enemy->getYCoord() + yClamped;

		xDif = xClosest - xCoord;
		yDif = yClosest - yCoord;

		float length = sqrt(xDif * xDif + yDif * yDif);

		if (length < radius) {
			health -= enemy->getSpeed();
			enemy->kill();

			if (health <= 0) {
				health = 0;
				std::cout << "You lost. To restart press R...\n";
				enemies.clear();
			}
		}
	}
}
