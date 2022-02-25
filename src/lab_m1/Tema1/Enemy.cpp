#include "Enemy.h"
#include <stdlib.h>

Enemy::Enemy() {
}

Enemy::Enemy(float xCoord, float yCoord) {
	this->xCoord = xCoord;
	this->yCoord = yCoord;
	this->angle = 0;
	this->isAlive = true;
	this->speed = rand() % 4 + 5; // nr between 5 and 8
	this->scale = 2;
	this->length = scale;
}

Enemy::~Enemy() {
}

float Enemy::getXCoord() {
	return xCoord;
}

float Enemy::getYCoord() {
	return yCoord;
}

float Enemy::getAngle() {
	return angle;
}

bool Enemy::exists() {
	return isAlive;
}

int Enemy::getSpeed() {
	return speed;
}

float Enemy::getScale() {
	return scale;
}

float Enemy::getLength() {
	return length;
}

void Enemy::modXCoord(float mod) {
	xCoord += mod;
}

void Enemy::modYCoord(float mod) {
	yCoord += mod;
}

void Enemy::setAngle(float newAngle) {
	angle = newAngle;
}

void Enemy::kill() {
	isAlive = false;
}
