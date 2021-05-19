#pragma once

#include <string>
#include "game_common.h"
#include <RobotInterface.h>

struct StrategyData
{
	Info info;
	Command strategy_command;
	StrategyData(Info info, Command strategy_command);
};


class AbstractStrategy
{
public:
	AbstractStrategy();
	virtual void AbstractStrategy::apply(std::unique_ptr<StrategyData>& strategyData);
};