#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transfrom3D.h"
#include "lab_m1/Tema2/camera.h"
#include "Character.h"
#include "Map.h"
#include "BulletT2.h"
#include "EnemyT2.h"

namespace tema2 {
	class Tema2 : public gfxc::SimpleScene {
	public:
		Tema2();
		~Tema2();

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
		void RenderMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color);
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnKeyPress(int key, int mods) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void RenderPlayer(bool isThirdPerson);
		void WorkOnBullets(float deltaTimeSeconds);
		void initEnemies();
		void RenderEnemy(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, int isHit, float time);
		void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
		void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

		implementedCamera::Camera* camera;
		glm::mat4 projectionMatrix;

		Character* player;
		bool isThirdPerson;
		Map* map;
		vector<Room*> fullRooms;
		vector<Room*> emptyRooms;
		vector<BulletT2*> bullets;
		vector<EnemyT2*> enemies;
		float timer;

		ViewportSpace viewSpace;
		LogicSpace logicSpace;

		gfxc::Camera* hudCamera;
		float timedef;
	};
}
