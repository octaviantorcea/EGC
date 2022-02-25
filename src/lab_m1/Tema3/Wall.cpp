#include "Wall.h"

using namespace std;
using namespace tema3;

tema3::Wall::Wall() {
}

tema3::Wall::Wall(glm::vec3 centerPosition, int index, FloorTile** danceFloor) {
	this->centerPosition = centerPosition;
	this->kd = 0.5f;
	this->ks = 0.35f;
	this->shininess = 10;

	if (index == 0) {
		lightingFloorTilesPosition[0] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[64]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[0]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[0]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[8]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[8]->getEmittingColor();
	}
	else if (index > 0 && index < 7) {
		for (int i = 0; i < 3; i++) {
			lightingFloorTilesPosition[i] = danceFloor[(i + index - 1) * 8]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[(i + index - 1) * 8]->getEmittingColor();
		}
	}
	else if (index == 7) {
		lightingFloorTilesPosition[0] = danceFloor[48]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[48]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[56]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[56]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[64]->getEmittingColor();
	}
	else if (index == 8) {
		lightingFloorTilesPosition[0] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[64]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[56]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[56]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[57]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[57]->getEmittingColor();
	}
	else if (index > 8 && index < 15) {
		for (int i = 0; i < 3; i++) {
			lightingFloorTilesPosition[i] = danceFloor[i + index + 47]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[i + index + 47]->getEmittingColor();
		}
	}
	else if (index == 15) {
		lightingFloorTilesPosition[0] = danceFloor[62]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[62]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[63]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[63]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[64]->getEmittingColor();
	}
	else if (index == 16) {
		lightingFloorTilesPosition[0] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[64]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[7]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[7]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[15]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[15]->getEmittingColor();
	}
	else if (index > 16 && index < 23) {
		for (int i = 0; i < 3; i++) {
			lightingFloorTilesPosition[i] = danceFloor[(i + index % 8) * 8 - 1]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[(i + index % 8) * 8 - 1]->getEmittingColor();
		}
	}
	else {
		lightingFloorTilesPosition[0] = danceFloor[55]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[55]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[63]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[63]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[64]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[64]->getEmittingColor();
	}
}

tema3::Wall::~Wall() {
}

glm::vec3 tema3::Wall::getCenterPosition() {
	return this->centerPosition;
}

float tema3::Wall::getKd() {
	return this->kd;
}

float tema3::Wall::getKs() {
	return this->ks;
}

float tema3::Wall::getShininess() {
	return this->shininess;
}

glm::vec3* tema3::Wall::getLightingFTPosition() {
	return this->lightingFloorTilesPosition;
}

glm::vec3* tema3::Wall::getLightingFTColor() {
	return this->lightingFloorTilesColor;
}
