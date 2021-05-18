#pragma once

// Use this as the base class for robot controllers

#include <string>
#include "game_common.h"

/**
 * This is the information structure sent to the do_command function
 * in each round.
 */
struct Info {
	/**
	 * 'last_event' is the result of the previous round:
	 *  OK - did nothing or moved successfully
	 *  COLLISION - tried to move or place trap but wall or other robot blocked
	 *  TREASURE_CAPTURED - your robot captured a treasure
	 *  PLACED_TRAP - your robot successfully placed a trap
	 *  TRAPPED - you stepped on trap and were captured for many rounds
	 */
	Event last_event;
	/**
	 * 'neighbor_cells' gives information about the surrounding cells
	 * Indexes is according to the following figure:
		[0][0]  [0][1]  [0][2]
		[1][0]   robot  [1][2]
		[2][0]  [2][1]  [2][2]
	 *  'neighbor_cells[1][1]' will always be ROBOT since that
	 *  is yourself.
	 */
	Cell_content neighbor_cells[3][3];
};

class RobotInterface
{
public:
	int robot_bitmap_row = 0;   // Pick monster icon for robot from Nevanda_32.png (the alltiles)
	int robot_bitmap_col = 13;
	std::wstring team_name;		// Team name or title

public:
	virtual Command do_command(const Info &info) = 0;
};
