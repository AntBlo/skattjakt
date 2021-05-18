#pragma once

#include <vector>

#include "game_common.h"

#define MAP_FILE "map_20x20x5.h"

struct Treasure {
	int pos_x, pos_y;    // a found treasure has pos_x, pos_y = 0
	int treasure_id;
};

struct Robotinit {
	int pos_x, pos_y;
};

class World
{
public:
	int world_size_x;
	int world_size_y;
	int p1_startx, p1_starty, p2_startx, p2_starty;
	int num_treasures;

public:
	World();

	const Cell_content &getCell(int x, int y) const;
	void setCellEmpty(int x, int y);
	bool tryPutTrap(int x, int y);

private:
	std::vector<std::vector<Cell_content>> theMap;
};

