#pragma once

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

// adapted from: https://github.com/john-science/mazelib/blob/master/docs/MAZE_GEN_ALGOS.md
// Aldous-Broder algorithm

vector<pair<int, int>> getNeighbors(int row, int col, int** grid, int nr_rows, int nr_collumns) {
	vector<pair<int, int>> neighbors;

	if (row > 1 && grid[row - 2][col] == 1) {
		neighbors.push_back(make_pair(row - 2, col));
	}

	if (row < nr_rows - 2 && grid[row + 2][col] == 1) {
		neighbors.push_back(make_pair(row + 2, col));
	}

	if (col > 1 && grid[row][col - 2] == 1) {
		neighbors.push_back(make_pair(row, col - 2));
	}

	if (col < nr_collumns - 2 && grid[row][col + 2] == 1) {
		neighbors.push_back(make_pair(row, col + 2));
	}

	random_shuffle(neighbors.begin(), neighbors.end());
	return neighbors;
}

int **generateMaze(int nr_rows, int nr_collumns) {
	int** grid = new int*[nr_rows];

	for (int i = 0; i < nr_rows; i++) {
		grid[i] = new int[nr_collumns];
	}

	for (int i = 0; i < nr_rows; i++) {
		for (int j = 0; j < nr_collumns; j++) {
			grid[i][j] = 1;
		}
	}

	int crow = rand() % (nr_rows - 1) + 1;
	if (crow % 2 != 1) {
		crow -= 1;
	}


	int ccol = rand() % (nr_collumns - 1) + 1;
	if (ccol % 2 != 1) {
		ccol -= 1;
	}

	int srow = crow;
	int scol = ccol;

	vector<pair<int, int>> track;
	track.push_back(make_pair(crow, ccol));
	grid[crow][ccol] = 0;

	while (!track.empty()) {
		crow = track.back().first;
		ccol = track.back().second;

		auto neigh = getNeighbors(crow, ccol, grid, nr_rows, nr_collumns);

		if (neigh.empty()) {
			track.pop_back();
		}
		else {
			int nrow = neigh.front().first;
			int ncol = neigh.front().second;
			grid[nrow][ncol] = 0;
			grid[(nrow + crow) / 2][(ncol + ccol) / 2] = 0;

			track.push_back(make_pair(nrow, ncol));
		}
	}

	grid[srow][scol] = 2;

	// generate exit
	int exit = rand() % 4;
	int erow, ecol, ok;

	switch (exit)
	{
	case 0:
		erow = 0;
		ok = 0;
		ecol;

		while (ok == 0) {
			ecol = rand() % (nr_collumns - 2) + 1;
			
			if (grid[erow + 1][ecol] == 0) {
				ok = 1;
			}
		}

		grid[erow][ecol] = 0;
		break;

	case 1:
		ecol = nr_collumns - 1;
		ok = 0;
		erow;

		while (ok == 0) {
			erow = rand() % (nr_rows - 2) + 1;

			if (grid[erow][ecol - 1] == 0) {
				ok = 1;
			}
		}

		grid[erow][ecol] = 0;
		break;

	case 2:
		erow = nr_rows - 1;
		ok = 0;
		ecol;

		while (ok == 0) {
			ecol = rand() % (nr_collumns - 2) + 1;

			if (grid[erow - 1][ecol] == 0) {
				ok = 1;
			}
		}

		grid[erow][ecol] = 0;
		break;

	case 3:
		ecol = 0;
		ok = 0;
		erow;

		while (ok == 0) {
			erow = rand() % (nr_rows - 2) + 1;

			if (grid[erow][ecol + 1] == 0) {
				ok = 1;
			}
		}

		grid[erow][ecol] = 0;
		break;
	}

	return grid;
}
