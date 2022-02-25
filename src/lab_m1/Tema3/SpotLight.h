#pragma once

#include "components/simple_scene.h"
#include "transform3D.h"

#define MIN_ANGLE -17.5f
#define MAX_ANGLE 17.5f
#define CHANGE_SPEED 0.14f

namespace tema3 {
	class SpotLight {
	public:
		SpotLight(glm::vec3 position);
		~SpotLight();

		glm::vec3 getPosition();
		glm::vec3 getDirection();
		float getCutoffAngle();
		glm::vec3 getColor();
		float getIntensity();
		float getOxAngle();
		float getOyAngle();
		float getOzAngle();
		void updateDirection(float deltaTimeSeconds);
	private:
		glm::vec3 position;
		glm::vec3 direction;
		float cutoffAngle;
		glm::vec3 color;
		float intensity;
		float oxAngle;
		float oyAngle;
		float ozAngle;
	};
}
