#pragma once
#include "game_common.h"
#include <RobotInterface.h>

using namespace std;

namespace Team_Lejla_Leon_Anton {
	struct StrategyData
	{
		Info info;
		Command strategy_command;
		map<pair<int, int>, Cell_content> world_map;
		StrategyData(Info info, Command strategy_command, map<pair<int, int>, Cell_content> world_map)
		{
			this->info = info;
			this->strategy_command = strategy_command;
			this->world_map = world_map;
		}
	};

	class AbstractStrategy
	{
	public:
		AbstractStrategy() {}
		virtual void AbstractStrategy::apply(unique_ptr<StrategyData>& strategyData) {}
	};


	class StrategyExplore : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			strategyData->strategy_command.action = Action::STEP;
			strategyData->strategy_command.step_dir = Dir::N;
		}
	};

	class StrategyTakeTreasure : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			// TODO
		}
	};
}