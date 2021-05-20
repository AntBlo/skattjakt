#include "pch.h"
#include "RobotControl_Lejla_Leon_Anton.h"
#include "game_common.h"
#include <RobotInterface.h>
#include "Strategies.h"
#include <string>


namespace Team_Lejla_Leon_Anton{
	RobotControl::RobotControl()
	{
		robot_bitmap_row = 8;          // Pick a unique monster icon for your robot!
		robot_bitmap_col = 5;
		team_name = L"Team Lejla Leon Anton";
		relative_x_y_to_start_position = make_pair(0, 0);
		strategies =
		{
			new StrategyUpdateWorldMap(),
			new StrategyExplore(),
			new StrategyLayTrap(),
			new StrategyUpdateRobotPosition(),
		};
	}

	Command RobotControl::do_command(const Info &info)
	{
		auto cmd = Command{ Action::PASS, Dir::N };
		unique_ptr<StrategyData> strategyData = make_unique<StrategyData>(info, cmd, &this->world_map, &this->relative_x_y_to_start_position);
		
		for each (AbstractStrategy* strategy in this->strategies)
		{
			strategy->apply(strategyData);
		}
		return strategyData->strategy_command;
	}
}