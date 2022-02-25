#pragma once

#include "components/simple_scene.h"
#include "FloorTile.h"

#define NR_OF_LIGHT_TILES 9
#define DANCER_WIDTH 0.5f
#define DANCER_HEIGHT 1.0f
#define XMIN -0.25f
#define XMAX 7.25f
#define ZMIN -0.25f
#define ZMAX 7.25f
#define DANCER_SPEED 1.5f

namespace tema3 {
	class Dancer {
	public:
		Dancer(glm::vec3 position);
		Dancer(FloorTile** danceFloor);
		~Dancer();

		glm::vec3 getPosition();
		float getKd();
		float getKs();
		float getShininess();
		glm::vec3* getLightingFTPosition();
		glm::vec3* getLightingFTColors();

		void move(float deltaTimeSeconds);
		void updateLightingTiles(FloorTile** danceFloor);

		void moveOX(float deltaTimeSeconds);
		void moveOZ(float deltaTimeSeconds);
	private:
		glm::vec3 position;
		float kd;
		float ks;
		float shininess;
		glm::vec3 lightingFloorTilesPosition[NR_OF_LIGHT_TILES];
		glm::vec3 lightingFloorTilesColor[NR_OF_LIGHT_TILES];
	};
}
