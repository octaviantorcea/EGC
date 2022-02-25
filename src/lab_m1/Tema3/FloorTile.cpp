#include "FloorTile.h"

using namespace std;
using namespace tema3;



tema3::FloorTile::FloorTile() {
	this->centerPosition = glm::vec3(-8);
	this->emittingColor = glm::vec3(0);
	this->lightRadius = 0;
	this->kd = 0.8f;
	this->ks = 0.64f;
	this->shininess = 8;
}

tema3::FloorTile::FloorTile(glm::vec3 centerPosition) {
	this->centerPosition = centerPosition;
	this->emittingColor = glm::vec3((float)(rand() % 256) / 256, (float)(rand() % 256) / 256, (float)(rand() % 256) / 256);
	this->lightRadius = 1.581f;
	this->kd = 0.8f;
	this->ks = 0.64f;
	this->shininess = 8;
}

tema3::FloorTile::~FloorTile() {
}

glm::vec3 tema3::FloorTile::getCenterPosition() {
	return this->centerPosition;
}

glm::vec3 tema3::FloorTile::getEmittingColor() {
	return this->emittingColor;
}

float tema3::FloorTile::getLightRadius() {
	return this->lightRadius;
}

float tema3::FloorTile::getKd() {
	return this->kd;
}

float tema3::FloorTile::getKs() {
	return this->ks;
}

float tema3::FloorTile::getShininess() {
	return this->shininess;
}
