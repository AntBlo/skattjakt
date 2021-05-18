#include "pch.h"
#include "RobotControl_player2.h"

using namespace Player2;

Player2::RobotControl::RobotControl()
{
	robot_bitmap_row = 0;          // Pick a unique monster icon for your robot!
	robot_bitmap_col = 4;
	team_name = L"Team bäst";
}

Command Player2::RobotControl::do_command(const Info &info)
{
	int d = rand() % 8;
	Dir dir = static_cast<Dir>(d);
	// dir = Dir::NE;
	return Command{Action::STEP,dir};
}
