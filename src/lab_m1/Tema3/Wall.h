#pragma once

#include "components/simple_scene.h"
#include "FloorTile.h"

#define WALL_WIDTH 1
#define WALL_HEIGHT 4
#define NR_OF_LIGHT_FT 3

namespace tema3 {
	class Wall {
	public:
		Wall();
		Wall(glm::vec3 centerPosition, int index, FloorTile** danceFloor);
		~Wall();

		glm::vec3 getCenterPosition();
		float getKd();
		float getKs();
		float getShininess();
		glm::vec3* getLightingFTPosition();
		glm::vec3* getLightingFTColor();
	private:
		glm::vec3 centerPosition;
		float kd;
		float ks;
		float shininess;
		glm::vec3 lightingFloorTilesPosition[NR_OF_LIGHT_FT];
		glm::vec3 lightingFloorTilesColor[NR_OF_LIGHT_FT];
	};
}
