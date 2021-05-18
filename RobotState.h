#pragma once

#include "game_common.h"

class RobotState
{
public:
	int id;  // player 1 = id 1, player 2 = id 2
	int pos_x, pos_y;
	int no_treasures_captured = 0;
	int no_traps_remaining = 5;
	int trapped_rounds = 0;           // > 0 means trapped
	Event last_event;
	Command last_command;

public:
	RobotState(int id, int startx, int starty);
	bool isRobotAtPosition(int x, int y);
};

