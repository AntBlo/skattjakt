#pragma once
#include "game_common.h"
#include <RobotInterface.h>

using namespace std;

namespace Team_Lejla_Leon_Anton {

	typedef std::pair<int, int> Position;

	struct StrategyData
	{
		Info info;
		Command strategy_command;
		map<Position, Cell_content>* world_map;
		Position* relative_x_y_to_start_position;
		StrategyData(Info info, Command strategy_command, map<Position, Cell_content> *world_map, Position* relative_x_y_to_start_position)
		{
			this->info = info;
			this->strategy_command = strategy_command;
			this->world_map = world_map;
			this->relative_x_y_to_start_position = relative_x_y_to_start_position;
		}
	};

	class AbstractStrategy
	{
	public:
		AbstractStrategy() {}
		virtual void AbstractStrategy::apply(unique_ptr<StrategyData>& strategyData) {}
		void dir_to_x_y(Dir dir, int& x, int& y)
		{
			if (dir == Dir::N) {
				x = 0;
				y = 1;
			}
			else if (dir == Dir::E) {
				x = 1;
				y = 0;
			}
			else if (dir == Dir::W) {
				x = -1;
				y = 0;
			}
			else if (dir == Dir::S) {
				x = 0;
				y = -1;
			}
			else if (dir == Dir::NE) {
				x = 1;
				y = 1;
			}
			else if (dir == Dir::NW) {
				x = -1;
				y = 1;
			}
			else if (dir == Dir::SE) {
				x = 1;
				y = -1;
			}
			else if (dir == Dir::SW) {
				x = -1;
				y = -1;
			}
		}
	};


	class StrategyUpdateWorldMap : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			int xs[] = { -1, 0, 1 };
			int ys[] = { -1, 0, 1 };
			for each (int x in xs)
			{
				for each (int y in ys)
				{
					auto not_center = !(x == 0 && y == 0);
					if (not_center) {
						auto cell_content = strategyData->info.neighbor_cells[1-y][x+1];
						Position update_position = make_pair(strategyData->relative_x_y_to_start_position->first + x, strategyData->relative_x_y_to_start_position->second + y);
						auto stored_cell_content = strategyData->world_map->find(update_position);
						if (stored_cell_content == strategyData->world_map->end()) {
							strategyData->world_map->insert({ update_position, cell_content });
							strategyData->world_map->insert({ update_position, cell_content });
						}
					}
				}
			}
		}
	};

	class StrategyExplore : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			strategyData->strategy_command.action = Action::STEP;
			strategyData->strategy_command.step_dir = Dir::N;
		}
	};

	class StrategyLayTrap : public AbstractStrategy
	{
		int num_traps = 5;
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			if(num_traps > 0){
				auto x = 0;
				auto y = 0;
				Dir dir = Dir::W;
				dir_to_x_y(dir, x, y);
				Position put_trap_at_position = make_pair(strategyData->relative_x_y_to_start_position->first + x, strategyData->relative_x_y_to_start_position->second + y);

				auto stored_cell_content = strategyData->world_map->find(put_trap_at_position);
				if (stored_cell_content->second == Cell_content::EMPTY){
					strategyData->strategy_command.action = Action::PLACE_TRAP;
					strategyData->strategy_command.step_dir = dir;
					stored_cell_content->second = Cell_content::TRAP;
					num_traps--;
				}
			}
		}
	};

	class StrategyTakeTreasure : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			// TODO
		}
	};

	class StrategyUpdateRobotPosition : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			if (strategyData->strategy_command.action == Action::STEP) {
				auto x = 0;
				auto y = 0;
				dir_to_x_y(strategyData->strategy_command.step_dir, x, y);
				strategyData->relative_x_y_to_start_position->first += x;
				strategyData->relative_x_y_to_start_position->second += y;
			}
		}
	};
}