#include "pch.h"
#include "RobotControl_Lejla_Leon_Anton.h"
#include "game_common.h"
#include <RobotInterface.h>
#include "Strategies.h"
#include <string>


namespace Team_Lejla_Leon_Anton{
	RobotControl::RobotControl()
	{
		robot_bitmap_row = 0;          // Pick a unique monster icon for your robot!
		robot_bitmap_col = 4;
		team_name = L"Team bäst";
	}


	map<pair<int, int>, Cell_content> world_map;

	AbstractStrategy* strategies[] =
	{
		new StrategyExplore(),
		new StrategyTakeTreasure(),
		// new StrategyPutTrap(),
		// new StrategyAvoidTrap(),
	};


	Command RobotControl::do_command(const Info &info)
	{
		auto cmd = Command{ Action::PASS, Dir::N };
		unique_ptr<StrategyData> strategyData = make_unique<StrategyData>(info, cmd, world_map);


		for each (AbstractStrategy* strategy in strategies)
		{
			strategy->apply(strategyData);
		}

		return strategyData->strategy_command;
	}
}