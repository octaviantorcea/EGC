#include "Globe.h"

tema3::Globe::Globe(Texture2D* texture) {
	this->position = glm::vec3(3.5f, 3, 3.5f);
	this->texture = texture;
	this->intensity = 9.25f;
}

tema3::Globe::~Globe() {
}

glm::vec3 tema3::Globe::getPosition() {
	return this->position;
}

Texture2D* tema3::Globe::getTexture() {
	return this->texture;
}

float tema3::Globe::getIntensity() {
	return this->intensity;
}
