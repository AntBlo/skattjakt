#include "pch.h"
#include "StrategyExplore.h"



void StrategyExplore::apply(std::unique_ptr<StrategyData>& strategyData)
{
	strategyData->strategy_command.action = Action::STEP;
	strategyData->strategy_command.step_dir = Dir::N;
}
