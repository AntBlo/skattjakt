#include "pch.h"
#include "Game.h"
#include "RobotControl_player1.h"
#include "RobotControl_player2.h"

// Namespaces for player codes
#define PLAYER1 Player1
#define PLAYER2 Player2

Game::Game() :
	robot1(1, theWorld.p1_startx, theWorld.p1_starty),
	robot2(2, theWorld.p2_startx, theWorld.p2_starty)
{
	pRobotcontrol1 = new PLAYER1::RobotControl();
	pRobotcontrol2 = new PLAYER2::RobotControl();
}

Game::~Game()
{
	delete pRobotcontrol1;
	delete pRobotcontrol2;
}

void Game::run_next_robot()
{
	if (next_robot == 1) {
		run_one_robot(robot1, pRobotcontrol1);
		next_robot++;
	}
	else {
		run_one_robot(robot2, pRobotcontrol2);
		next_robot = 1;
		round++;
	}
}

void Game::run_one_robot(RobotState &state, RobotInterface *pRobot)
{
	// if trapped, count down, don't run do_command()
	if (state.trapped_rounds > 0) {
		state.trapped_rounds--;
		return;
	}

	//                 N, E, S, W,NE,SE,SW,NW
	int delta_x[] = { 0, 1, 0,-1, 1, 1,-1,-1 };
	int delta_y[] = { -1, 0, 1, 0,-1, 1, 1,-1 };

	Info info;
	info.last_event = state.last_event;

	// Build neigbor_cells:
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++) {
			int iy = state.pos_y + y - 1;
			int ix = state.pos_x + x - 1;
			auto cell = theWorld.getCell(ix, iy);
			if (cell == Cell_content::TRAP && rand() % 2 == 0)
				cell = Cell_content::EMPTY;      // 50% risk of missing spotting trap
			info.neighbor_cells[y][x] = cell;
			if (robot1.isRobotAtPosition(ix,iy) || robot2.isRobotAtPosition(ix,iy))
				info.neighbor_cells[y][x] = Cell_content::ROBOT;
		}

	Command com = pRobot->do_command(info);

	state.last_event = Event::OK;
	state.last_command = com;

	if (com.action == Action::PASS)
		return;

	// action is STEP or PLACE_TRAP
	int dir = static_cast<int>(com.step_dir);
	int new_x = state.pos_x + delta_x[dir];
	int new_y = state.pos_y + delta_y[dir];
	bool collisionWallRobot = false;
	if (theWorld.getCell(new_x, new_y) == Cell_content::WALL) {
		collisionWallRobot = true;
	}
	else if (new_x == robot1.pos_x && new_y == robot1.pos_y ||
		new_x == robot2.pos_x && new_y == robot2.pos_y) {
		collisionWallRobot = true;
	}
	if (collisionWallRobot) {
		state.last_event = Event::COLLISION;
	}
	else {
		if (com.action == Action::STEP) {
			state.pos_x = new_x;
			state.pos_y = new_y;
			if (theWorld.getCell(new_x, new_y) == Cell_content::TREASURE) {
				state.last_event = Event::TREASURE_CAPTURED;
				state.no_treasures_captured++;
				theWorld.setCellEmpty(new_x, new_y);
			}
			if (theWorld.getCell(new_x, new_y) == Cell_content::TRAP) {
				state.last_event = Event::TRAPPED;
				state.trapped_rounds = (rand() % 5) + 3;  // 3 - 7 rounds captured
				theWorld.setCellEmpty(state.pos_x, state.pos_y);
			}
		}
		else { // PLACE_TRAP
			if (state.no_traps_remaining > 0) {
				if (theWorld.tryPutTrap(new_x, new_y)) {
					state.last_event = Event::PLACED_TRAP;
					--state.no_traps_remaining;
				}
				else
					state.last_event = Event::COLLISION;
			}
			else
				state.last_event = Event::OK;
		}
	}
}
