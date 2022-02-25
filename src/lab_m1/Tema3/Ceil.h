#pragma once

#include "components/simple_scene.h"

#define CEIL_LENGTH 8

namespace tema3 {
	class Ceil {
	public:
		Ceil();
		~Ceil();
		glm::vec3 getPosition();
		float getKd();
		float getKs();
		float getShininess();
	private:
		glm::vec3 position;
		float kd;
		float ks;
		float shiniess;
	};
}
