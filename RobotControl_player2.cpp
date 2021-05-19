#include "pch.h"
#include "RobotControl_player2.h"
#include "AbstractStrategy.h"
#include "StrategyExplore.h"

using namespace Player2;

Player2::RobotControl::RobotControl()
{
	robot_bitmap_row = 0;          // Pick a unique monster icon for your robot!
	robot_bitmap_col = 4;
	team_name = L"Team bäst";
}


class StrategyTakeTreasure: public AbstractStrategy
{
public:
	void apply(std::unique_ptr<StrategyData>& strategyData) {
		// TODO
	}
};

AbstractStrategy* strategies[] =
{
	new StrategyExplore(),
	new StrategyTakeTreasure(),
	// new StrategyPutTrap(),
	// new StrategyAvoidTrap(),
};

Command Player2::RobotControl::do_command(const Info &info)
{
	auto cmd = Command{ Action::PASS, Dir::N };
	std::unique_ptr<StrategyData> strategyData = std::make_unique<StrategyData>(info, cmd);


	for each (AbstractStrategy* strategy in strategies)
	{
		strategy->apply(strategyData);
	}

	return strategyData->strategy_command;
}
