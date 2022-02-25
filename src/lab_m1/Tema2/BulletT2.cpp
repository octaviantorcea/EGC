#include "lab_m1/Tema2/BulletT2.h"

float maximum(float a, float b) {
	return a > b ? a : b;
}

float minimum(float a, float b) {
	return a < b ? a : b;
}

BulletT2::BulletT2() {
}

BulletT2::~BulletT2() {
}

BulletT2::BulletT2(Character* player) {
	this->position = player->getPosition();
	this->TTL = 0;
	this->speed = 40;
	this->isAlive = true;
	this->radius = 0.5f;
	this->angleOY = player->getAngleOY();
	this->angleOX = player->getAngleOX();
}

glm::vec3 BulletT2::getPosition() {
	return this->position;
}

float BulletT2::getTTL() {
	return this->TTL;
}

float BulletT2::getSpeed() {
	return this->speed;
}

float BulletT2::getAngleOY() {
	return this->angleOY;
}

float BulletT2::getRadius() {
	return this->radius;
}

bool BulletT2::exists() {
	return this->isAlive;
}

// process bullet movement for "mod" time
void BulletT2::step(float mod, vector<Room*> fullRooms, Character* player, vector<EnemyT2*> enemies) {
	position.x += cos(-angleOY) * mod * speed;
	position.z += sin(-angleOY) * mod * speed;
	position.y += sin(+angleOX) * mod * speed;
	TTL += mod;

	if (TTL > MAXTTL || hitsWall(fullRooms) || hitsEnemy(enemies)) {
		isAlive = false;
	}
}

// checks if the bullet hit any walls
bool BulletT2::hitsWall(vector<Room*> fullRooms) {
	for (auto room : fullRooms) {
		float x = maximum(room->position.x - ROOM_LENGTH / 2, minimum(position.x, room->position.x + ROOM_LENGTH / 2));
		float y = maximum(room->position.y - ROOM_LENGTH / 2, minimum(position.y, room->position.y + ROOM_LENGTH / 2));
		float z = maximum(room->position.z - ROOM_LENGTH / 2, minimum(position.z, room->position.z + ROOM_LENGTH / 2));

		float dist = sqrt((x - position.x) * (x - position.x) +
			(y - position.x) * (y - position.y) +
			(z - position.z) * (z - position.z));

		if (dist < radius || position.y - radius <= 0) {
			return true;
		}
	}

	return false;
}

// checks if the bullet hit any enemies
bool BulletT2::hitsEnemy(vector<EnemyT2*> enemies) {
	for (auto enemy : enemies) {
		float dist = sqrt((position.x - enemy->getPosition().x) * (position.x - enemy->getPosition().x) +
			(position.y - enemy->getPosition().y) * (position.y - enemy->getPosition().y) +
			(position.z - enemy->getPosition().z) * (position.z - enemy->getPosition().z));

		if (dist < this->radius + enemy->getRadius() && enemy->getIsHit() == 0) {
			enemy->setHit();
			return true;
		}
	}

	return false;
}
