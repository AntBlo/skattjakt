#include "pch.h"
#include <debugapi.h>
#include "RobotControl_player1.h"

using namespace Player1;

Player1::RobotControl::RobotControl()
{
	robot_bitmap_row = 0;          // Pick a unique monster icon for your robot!
	robot_bitmap_col = 12;
	team_name = L"Team vinnarna";
}

Command Player1::RobotControl::do_command(const Info &info)
{
	OutputDebugString(L"Nu körs do_command()\n");
	if (info.last_event == Event::COLLISION)
		OutputDebugString(L"Krockade i förra rundan!\n");

	int d = rand() % 8;
	Dir dir = static_cast<Dir>(d);
	Action act = Action::STEP;
	if (rand() % 20 == 0)
		act = Action::PLACE_TRAP;
	return Command{act,dir};
}
