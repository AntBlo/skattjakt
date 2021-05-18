#include "pch.h"
#include <cassert>
#include "World.h"

#include MAP_FILE

using namespace std;

World::World()
{
	world_size_x = WORLD_SIZE_X;
	world_size_y = WORLD_SIZE_Y;
	num_treasures = 0;

	for (int y = 0; y < WORLD_SIZE_Y; y++) {
		vector<Cell_content> row;
		for (int x = 0; x < WORLD_SIZE_X; x++) {
			Cell_content c;

			switch (initWorld[y][x]) {
			case 'X':
				c = Cell_content::WALL;
				break;
			case '$':
				c = Cell_content::TREASURE;
				num_treasures++;
				break;
			case '1':
				p1_startx = x;
				p1_starty = y;
				c = Cell_content::EMPTY;
				break;
			case '2':
				p2_startx = x;
				p2_starty = y;
				c = Cell_content::EMPTY;
				break;
			default:
				c = Cell_content::EMPTY;
			}
			row.push_back(c);
		}
		theMap.push_back(row);
	}
	assert(p1_startx > 0 && p1_starty > 0 && p2_startx > 0 && p2_starty); // check '1' and '2' in map
}

const Cell_content &World::getCell(int x, int y) const
{
	return theMap[y][x];
}

void World::setCellEmpty(int x, int y)
{
	theMap[y][x] = Cell_content::EMPTY;
}

bool World::tryPutTrap(int x, int y)
{
	if (theMap[y][x] == Cell_content::EMPTY) {
		theMap[y][x] = Cell_content::TRAP;
		return true;
	}
	return false;
}
