#pragma once

#include "glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

#define EMPTY 0
#define WALL 1
#define PLAYERSTART 2
#define ROOM_LENGTH 20

class Room {
public:
	int type;
	glm::vec3 position;

	Room() {}
	~Room() {}

	Room(int type, glm::vec3 position) {
		this->type = type;
		this->position = position;
	}
};

class Map {
public:
	Map();
	~Map();
	vector<Room*> getRooms();

private:
	vector<Room*> rooms;
};
