#include "Ceil.h"

tema3::Ceil::Ceil() {
	this->position = glm::vec3(CEIL_LENGTH / 2 - 0.5f, 4, CEIL_LENGTH / 2 - 0.5f);
	this->kd = 0.65f;
	this->ks = 0.58f;
	this->shiniess = 5;
}

tema3::Ceil::~Ceil() {
}

glm::vec3 tema3::Ceil::getPosition() {
	return this->position;
}

float tema3::Ceil::getKd() {
	return this->kd;
}

float tema3::Ceil::getKs() {
	return this->ks;
}

float tema3::Ceil::getShininess() {
	return this->shiniess;
}
