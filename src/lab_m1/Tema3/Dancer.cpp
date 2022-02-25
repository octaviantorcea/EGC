#include "Dancer.h"
#include <iostream>

tema3::Dancer::Dancer(glm::vec3 position) {
	this->position = position;
	this->kd = 0.4f;
	this->ks = 0.35f;
	this->shininess = 15;
}

tema3::Dancer::Dancer(FloorTile** danceFloor) {
	this->position = glm::vec3((float)rand() / (float)RAND_MAX * (XMAX - XMIN) + XMIN, DANCER_HEIGHT / 2, (float)rand() / (float)RAND_MAX * (ZMAX - ZMIN) + ZMIN);
	this->kd = 0.4f;
	this->ks = 0.35f;
	this->shininess = 8;
}

tema3::Dancer::~Dancer() {
}

glm::vec3 tema3::Dancer::getPosition() {
	return this->position;
}

float tema3::Dancer::getKd() {
	return this->kd;
}

float tema3::Dancer::getKs() {
	return this->ks;
}

float tema3::Dancer::getShininess() {
	return this->shininess;
}

glm::vec3* tema3::Dancer::getLightingFTPosition() {
	return this->lightingFloorTilesPosition;
}

glm::vec3* tema3::Dancer::getLightingFTColors() {
	return this->lightingFloorTilesColor;
}

void tema3::Dancer::move(float deltaTimeSeconds) {
	float newX = this->position.x;
	float newZ = this->position.z;

	float direction = rand() % 8;

	if (direction == 0 && newX - deltaTimeSeconds >= XMIN) {
		newX -= deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 1 && newX + deltaTimeSeconds <= XMAX) {
		newX += deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 2 && newZ - deltaTimeSeconds >= ZMIN) {
		newZ -= deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 3 && newZ + deltaTimeSeconds <= ZMAX) {
		newZ += deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 4 && newX - deltaTimeSeconds >= XMIN && newZ - deltaTimeSeconds >= ZMIN) {
		newX -= deltaTimeSeconds * DANCER_SPEED;
		newZ -= deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 5 && newX + deltaTimeSeconds <= XMAX && newZ - deltaTimeSeconds >= ZMIN) {
		newX += deltaTimeSeconds * DANCER_SPEED;
		newZ -= deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 6 && newX - deltaTimeSeconds >= XMIN && newZ + deltaTimeSeconds <= ZMAX) {
		newX -= deltaTimeSeconds * DANCER_SPEED;
		newZ += deltaTimeSeconds * DANCER_SPEED;
	}
	else if (direction == 7 && newX + deltaTimeSeconds <= XMAX && newZ + deltaTimeSeconds <= ZMAX) {
		newX += deltaTimeSeconds * DANCER_SPEED;
		newZ += deltaTimeSeconds * DANCER_SPEED;
	}

	this->position = glm::vec3(newX, this->position.y, newZ);
}

void tema3::Dancer::updateLightingTiles(FloorTile** danceFloor) {
	int tileXIndex = round(this->position.x);
	int tileZIndex = round(this->position.z);
	int tileIndex = tileXIndex * 8 + tileZIndex;

	if (tileIndex == 0) {
		lightingFloorTilesPosition[0] = danceFloor[0]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[0]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[1]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[1]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[8]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[8]->getEmittingColor();
		lightingFloorTilesPosition[3] = danceFloor[9]->getCenterPosition();
		lightingFloorTilesColor[3] = danceFloor[9]->getEmittingColor();

		for (int i = 4; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex == 7) {
		lightingFloorTilesPosition[0] = danceFloor[6]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[6]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[7]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[7]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[14]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[14]->getEmittingColor();
		lightingFloorTilesPosition[3] = danceFloor[15]->getCenterPosition();
		lightingFloorTilesColor[3] = danceFloor[15]->getEmittingColor();

		for (int i = 4; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex == 56) {
		lightingFloorTilesPosition[0] = danceFloor[48]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[48]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[49]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[49]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[56]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[56]->getEmittingColor();
		lightingFloorTilesPosition[3] = danceFloor[57]->getCenterPosition();
		lightingFloorTilesColor[3] = danceFloor[57]->getEmittingColor();

		for (int i = 4; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex == 63) {
		lightingFloorTilesPosition[0] = danceFloor[46]->getCenterPosition();
		lightingFloorTilesColor[0] = danceFloor[46]->getEmittingColor();
		lightingFloorTilesPosition[1] = danceFloor[47]->getCenterPosition();
		lightingFloorTilesColor[1] = danceFloor[47]->getEmittingColor();
		lightingFloorTilesPosition[2] = danceFloor[62]->getCenterPosition();
		lightingFloorTilesColor[2] = danceFloor[62]->getEmittingColor();
		lightingFloorTilesPosition[3] = danceFloor[63]->getCenterPosition();
		lightingFloorTilesColor[3] = danceFloor[63]->getEmittingColor();

		for (int i = 4; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex % 8 == 0 && tileIndex != 0 && tileIndex != 56) {
		int auxIndex = 0;

		for (int i = -1; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				lightingFloorTilesPosition[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getCenterPosition();
				lightingFloorTilesColor[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getEmittingColor();
				auxIndex++;
			}
		}

		for (int i = 7; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex < 8 && tileIndex != 0) {
		int auxIndex = 0;

		for (int i = 0; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				lightingFloorTilesPosition[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getCenterPosition();
				lightingFloorTilesColor[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getEmittingColor();
				auxIndex++;
			}
		}

		for (int i = 7; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else if (tileIndex % 8 == 7 && tileIndex != 7 && tileIndex != 63) {
		int auxIndex = 0;

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 1; j++) {
				lightingFloorTilesPosition[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getCenterPosition();
				lightingFloorTilesColor[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getEmittingColor();
				auxIndex++;
			}
		}

		for (int i = 7; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	} else if(tileIndex < 63 && tileIndex > 56) {
		int auxIndex = 0;

		for (int i = -1; i < 1; i++) {
			for (int j = -1; j < 2; j++) {
				lightingFloorTilesPosition[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getCenterPosition();
				lightingFloorTilesColor[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getEmittingColor();
				auxIndex++;
			}
		}

		for (int i = 7; i < NR_OF_LIGHT_TILES; i++) {
			lightingFloorTilesPosition[i] = danceFloor[64]->getCenterPosition();
			lightingFloorTilesColor[i] = danceFloor[64]->getEmittingColor();
		}
	}
	else {
		int auxIndex = 0;
		
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				lightingFloorTilesPosition[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getCenterPosition();
				lightingFloorTilesColor[auxIndex] = danceFloor[tileIndex + i * 8 + j]->getEmittingColor();
				auxIndex++;
			}
		}
	}
}

void tema3::Dancer::moveOX(float deltaTimeSeconds) {
	glm::vec3 aux = this->position;
	this->position = glm::vec3(aux.x + deltaTimeSeconds, aux.y, aux.z);
}

void tema3::Dancer::moveOZ(float deltaTimeSeconds) {
	glm::vec3 aux = this->position;
	this->position = glm::vec3(aux.x, aux.y, aux.z + deltaTimeSeconds);
}
