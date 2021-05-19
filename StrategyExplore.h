#pragma once
#include "AbstractStrategy.h"
class StrategyExplore : public AbstractStrategy
{
public:
	void apply(std::unique_ptr<StrategyData>& strategyData);
};

