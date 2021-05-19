#include "pch.h"
#include "game_common.h"
#include <RobotInterface.h>
#include "AbstractStrategy.h"


StrategyData::StrategyData(Info info, Command strategy_command)
{
	this->info = info;
	this->strategy_command = strategy_command;
}

AbstractStrategy::AbstractStrategy()
{
}

void AbstractStrategy::apply(std::unique_ptr<StrategyData> &strategyData)
{
}
