#include "Map.h"
#include "MazeAlgorithm.h"

#define ROWS 11
#define COLLUMNS 11

Map::Map() {
	int** grid = generateMaze(ROWS, COLLUMNS);

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLLUMNS; j++) {
			int type = -1;
			if (grid[i][j] == 1) {
				type = WALL;
			}
			else if (grid[i][j] == 0) {
				type = EMPTY;
			}
			else if (grid[i][j] == 2) {
				type = PLAYERSTART;
			}

			rooms.push_back(new Room(type, glm::vec3(i * ROOM_LENGTH, 5, j * ROOM_LENGTH)));
		}
	}
}

Map::~Map() {
}

vector<Room*> Map::getRooms() {
	return this->rooms;
}
