#pragma once

#include "components/simple_scene.h"

#define GLOBE_SCALE_FACTOR 2

namespace tema3 {
	class Globe {
	public:
		Globe(Texture2D* texture);
		~Globe();
		glm::vec3 getPosition();
		Texture2D* getTexture();
		float getIntensity();
	private:
		glm::vec3 position;
		Texture2D* texture;
		float intensity;
	};
}
