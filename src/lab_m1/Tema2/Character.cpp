#include "Character.h"

Character::Character() {
}

Character::Character(glm::vec3 startPos) {
	this->height = 6;
	this->width = 5;
	this->length = 1;
	this->angleOY = 0;
	this->angleOX = 0;
	this->speed = 12;
	this->centerPosition = startPos;
	this->TTF = 0;
	this->fireRate = 0.5f;
	this->lives = 3;
}

Character::~Character() {
}

void Character::moveForward(float mod) {
	centerPosition.x += mod * speed * cos(this->angleOY);
	centerPosition.z += mod * speed * sin(-this->angleOY);
}

void Character::moveRight(float mod) {
	centerPosition.x += mod * speed * sin(this->angleOY);
	centerPosition.z += mod * speed * cos(this->angleOY);
}

void Character::modAngleOY(float mod) {
	this->angleOY += mod;
}

void Character::modAngleOX(float mod) {
	this->angleOX += mod;
}

void Character::setAngleOX(float angle) {
	this->angleOX = angle;
}

void Character::setTTF(float ttf) {
	this->TTF = ttf;
}

void Character::modTTF(float mod) {
	this->TTF += mod;
}

void Character::decLives() {
	this->lives--;
}

float Character::getHeight() {
	return this->height;
}

float Character::getWidth() {
	return this->width;
}

float Character::getLength() {
	return this->length;
}

float Character::getAngleOY() {
	return this->angleOY;
}

float Character::getAngleOX() {
	return this->angleOX;
}

float Character::getSpeed() {
	return this->speed;
}

glm::vec3 Character::getPosition() {
	return this->centerPosition;
}

float Character::getTTF() {
	return this->TTF;
}

float Character::getFireRate() {
	return this->fireRate;
}

float Character::getLives() {
	return this->lives;
}

// checks if the player hits any walls while pressing W or S keys
bool Character::hitsWallForward(vector<Room*> fullRooms, float mod) {
	float newXCenter = this->centerPosition.x + mod * speed * cos(this->angleOY);
	float newZCenter = this->centerPosition.z + mod * speed * sin(-this->angleOY);

	for (auto fullRoom : fullRooms) {
		if (newXCenter + width / 2 > fullRoom->position.x - ROOM_LENGTH / 2 &&
			newXCenter - width / 2 < fullRoom->position.x + ROOM_LENGTH / 2 &&
			newZCenter + width / 2 > fullRoom->position.z - ROOM_LENGTH / 2 &&
			newZCenter - width / 2 < fullRoom->position.z + ROOM_LENGTH / 2) {

			return true;
		}
	}

	return false;
}

// checks if the player hits any walls while pressing D or A keys
bool Character::hitsWallRight(vector<Room*> fullRooms, float mod) {
	float newXCenter = this->centerPosition.x + mod * speed * sin(this->angleOY);
	float newZCenter = this->centerPosition.z + mod * speed * cos(this->angleOY);

	for (auto fullRoom : fullRooms) {
		if (newXCenter + width / 2 > fullRoom->position.x - ROOM_LENGTH / 2 &&
			newXCenter - width / 2 < fullRoom->position.x + ROOM_LENGTH / 2 &&
			newZCenter + width / 2 > fullRoom->position.z - ROOM_LENGTH / 2 &&
			newZCenter - width / 2 < fullRoom->position.z + ROOM_LENGTH / 2) {

			return true;
		}
	}

	return false;
}

// checks if the player found the exit
bool Character::won() {
	if (centerPosition.x < -10 ||
		centerPosition.x > 210 ||
		centerPosition.z < -10 ||
		centerPosition.z > 210) {
		return true;
	}

	return false;
}
