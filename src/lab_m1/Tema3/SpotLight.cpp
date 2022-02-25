#include "SpotLight.h"

tema3::SpotLight::SpotLight(glm::vec3 position) {
	this->position = position;
	this->direction = glm::vec3(0, -1, 0);
	this->cutoffAngle = 14.03f;
	this->color = glm::vec3((float)(rand() % 256) / 256, (float)(rand() % 256) / 256, (float)(rand() % 256) / 256);
	this->intensity = 3.5f;
	this->oxAngle = 0;
	this->oyAngle = 0;
	this->ozAngle = 0;
}

tema3::SpotLight::~SpotLight() {
}

glm::vec3 tema3::SpotLight::getPosition() {
	return this->position;
}

glm::vec3 tema3::SpotLight::getDirection() {
	return this->direction;
}

float tema3::SpotLight::getCutoffAngle() {
	return this->cutoffAngle;
}

glm::vec3 tema3::SpotLight::getColor() {
	return this->color;
}

float tema3::SpotLight::getIntensity() {
	return this->intensity;
}

float tema3::SpotLight::getOxAngle() {
	return this->oxAngle;
}

float tema3::SpotLight::getOyAngle() {
	return this->oyAngle;
}

float tema3::SpotLight::getOzAngle() {
	return this->ozAngle;
}

void tema3::SpotLight::updateDirection(float deltaTimeSeconds) {
	float xSpeed = rand() % 3 - 1;
	float ySpeed = rand() % 3 - 1;
	float zSpeed = rand() % 3 - 1;

	if (oxAngle + xSpeed * deltaTimeSeconds >= MIN_ANGLE && oxAngle + xSpeed * deltaTimeSeconds <= MAX_ANGLE) {
		oxAngle += xSpeed * CHANGE_SPEED;
	}

	if (oyAngle + ySpeed * deltaTimeSeconds >= MIN_ANGLE && oyAngle + ySpeed * deltaTimeSeconds <= MAX_ANGLE) {
		oyAngle += ySpeed * CHANGE_SPEED;
	}

	if (ozAngle + zSpeed * deltaTimeSeconds >= MIN_ANGLE && ozAngle + zSpeed * deltaTimeSeconds <= MAX_ANGLE) {
		ozAngle += zSpeed * CHANGE_SPEED;
	}

	glm::mat4 rotateMatrix = glm::mat4(1);
	rotateMatrix *= RotateOX(RADIANS(oxAngle));
	rotateMatrix *= RotateOY(RADIANS(oyAngle));
	rotateMatrix *= RotateOZ(RADIANS(ozAngle));

	this->direction = rotateMatrix * glm::vec4(0, -1, 0, 1);
}
