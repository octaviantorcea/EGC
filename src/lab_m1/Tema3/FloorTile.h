#pragma once

#include "components/simple_scene.h"

#define FLOORTILE_RADIUS 0.5f

namespace tema3 {
	class FloorTile {
	public:
		FloorTile();
		FloorTile(glm::vec3 centerPosition);
		~FloorTile();

		glm::vec3 getCenterPosition();
		glm::vec3 getEmittingColor();
		float getLightRadius();
		float getKd();
		float getKs();
		float getShininess();
	private:
		glm::vec3 centerPosition;
		glm::vec3 emittingColor;
		float lightRadius;
		float kd;
		float ks;
		float shininess;
	};
}
