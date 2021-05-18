#pragma once

#include "World.h"
#include "RobotState.h"
#include "RobotInterface.h"

class Game
{
public:
	World theWorld;
	RobotState robot1;
	RobotState robot2;
	RobotInterface *pRobotcontrol1;
	RobotInterface *pRobotcontrol2;
	int round;
	int next_robot = 1;

public:
	Game();
	~Game();

	void run_next_robot();
	void run_one_robot(RobotState &state, RobotInterface * pRobot);
};

