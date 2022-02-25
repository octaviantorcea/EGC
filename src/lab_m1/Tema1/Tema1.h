#pragma once

#include "components/simple_scene.h"
#include "Border.h"
#include "Player.h"
#include "transform2D.h"
#include "Obstacle.h"
#include "Bullet.h"
#include <vector>
#include "Enemy.h"

namespace tema1 {
	class Tema1 : public gfxc::SimpleScene {
	public:
		Tema1();
		~Tema1();
		void Init() override;

		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		
		void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
		void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
		void WorkOnBullets(std::vector<Bullet*> &bullets, float deltaTimeSeconds, std::vector<Obstacle> obstacles,
						Border border, Player &player, std::vector<Enemy*> &enemies);

		ViewportSpace viewSpace;
		LogicSpace logicSpace;

		glm::mat3 modelMatrix, visMatrix;
		float scaleXBorder, scaleYBorder;

		Player player;
		Border border;

		const int circleLines = 32;
		std::vector<Obstacle> obstacles;
		std::vector<Bullet*> bullets;
		std::vector<Enemy*> enemies;

		float TTSE;
		float minSpawnDistance;
		float xMinGlobal;
		float yMinGlobal;
		float xMaxGlobal;
		float yMaxGlobal;
	};
}
