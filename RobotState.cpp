#include "pch.h"
#include "RobotState.h"
#include "World.h"

RobotState::RobotState(int id, int startx, int starty)
{
	this->id = id;
	pos_x = startx;
	pos_y = starty;
}

bool RobotState::isRobotAtPosition(int x, int y)
{
	return (x == pos_x && y == pos_y);
}
