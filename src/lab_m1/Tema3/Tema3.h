#pragma once

#include "components/simple_scene.h"
#include "transform3D.h"
#include "components/transform.h"
#include "FloorTile.h"
#include "Wall.h"
#include "Dancer.h"
#include "SpotLight.h"
#include "Ceil.h"
#include "Globe.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define NR_TILES 8
#define NR_OF_DANCERS 6
#define CONE_LINES 32
#define SPOTLIGHTS 4

namespace tema3 {
	class Tema3 : public gfxc::SimpleScene {
	public:
		Tema3();
		~Tema3();

		void Init() override;
	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void RenderFloorTile(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, FloorTile* floorTile);
		void RenderWall(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, Wall* wall);
		void RenderDancer(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, Dancer* dancer);
		void RenderCone(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, glm::vec3 color);
		void RenderCeil(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix);
		void RenderGlobe(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix);
		Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
		void OnKeyPress(int key, int mods) override;
		void OnInputUpdate(float deltaTime, int mods) override;

		FloorTile** danceFloor;
		Wall** walls;
		Dancer* dancers[NR_OF_DANCERS];
		SpotLight* spotlights[SPOTLIGHTS];
		Ceil* ceil;
		Globe* globe;
		glm::vec3 spotlightsPositions[SPOTLIGHTS];
		glm::vec3 spotlightsDirections[SPOTLIGHTS];
		glm::vec3 spotlightsColors[SPOTLIGHTS];
		std::unordered_map<std::string, Texture2D*> mapTextures;

		//// test
		//Dancer* special;

		int lightFromDanceFloor;
		int lightFromSpotLight;
		int lightFromGlobe;
		float deltaTime;
	};
}
