#include "EnemyT2.h"

float maxim(float a, float b) {
	return a > b ? a : b;
}

float minim(float a, float b) {
	return a < b ? a : b;
}

EnemyT2::EnemyT2() {
}

EnemyT2::~EnemyT2() {
}

EnemyT2::EnemyT2(Room* room) {
	this->radius = 3;
	this->xMin = room->position.x - ROOM_LENGTH / 2 + radius;
	this->xMax = room->position.x + ROOM_LENGTH / 2 - radius;
	this->zMin = room->position.z - ROOM_LENGTH / 2 + radius;
	this->zMax = room->position.z + ROOM_LENGTH / 2 - radius;
	this->position = glm::vec3(xMin, radius, zMin);
	this->alive = true;
	this->speed = 32;
	this->isHit = 0;
	this->hitTime = 2;
}

glm::vec3 EnemyT2::getPosition() {
	return this->position;
}

bool EnemyT2::isAlive() {
	return this->alive;
}

void EnemyT2::move(float deltaTimeSeconds) {
	if (this->position.z <= zMin && this->position.x <= xMax) {
		this->position.x += deltaTimeSeconds * speed;
	}

	if (this->position.z <= zMax && this->position.x >= xMax) {
		this->position.z += deltaTimeSeconds * speed;
	}

	if (this->position.z >= zMax && this->position.x >= xMin) {
		this->position.x -= deltaTimeSeconds * speed;
	}

	if (this->position.z >= zMin && this->position.x <= xMin) {
		this->position.z -= deltaTimeSeconds * speed;
	}
}

float EnemyT2::getRadius() {
	return this->radius;
}

float EnemyT2::getHitTime() {
	return this->hitTime;
}

int EnemyT2::getIsHit() {
	return this->isHit;
}

void EnemyT2::kill() {
	this->alive = false;
}

void EnemyT2::hitsPlayer(Character* player) {
	float x = maxim(player->getPosition().x - player->getWidth() / 2, minim(this->position.x, player->getPosition().x + player->getWidth() / 2));
	float y = maxim(player->getPosition().y - player->getWidth() / 2, minim(this->position.y, player->getPosition().y + player->getWidth() / 2));
	float z = maxim(player->getPosition().z - player->getWidth() / 2, minim(this->position.z, player->getPosition().z + player->getWidth() / 2));

	float dist = sqrt((x - this->position.x) * (x - this->position.x) +
		(y - this->position.y) * (y - this->position.y) +
		(z - this->position.z) * (z - this->position.z));

	if (dist < this->radius) {
		player->decLives();
		this->alive = false;
	}
}

void EnemyT2::setHit() {
	this->isHit = 1;
}

void EnemyT2::decHitTime(float mod) {
	this->hitTime -= mod;
}
