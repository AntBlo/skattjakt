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

		Dir x_y_to_dir(int x, int y)
		{
			Dir dir;
			if (x == 0 && y == 1) {
				dir = Dir::N;
			}
			else if (x == 1 && y == 0) {
				dir = Dir::E;
			}
			else if (x == -1 && y == 0) {
				dir = Dir::W;
			}
			else if (x == 0 && y == -1) {
				dir = Dir::S;
			}
			else if (x == 1 && y == 1) {
				dir = Dir::NE;
			}
			else if (x == -1 && y == 1) {
				dir = Dir::NW;
			}
			else if (x == 1 && y == -1) {
				dir = Dir::SE;
			}
			else {//if (x == -1 && y == -1) {
				dir = Dir::SW;
			}
			return dir;
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
					auto cell_content = strategyData->info.neighbor_cells[1-y][x+1];
					if (cell_content == Cell_content::ROBOT)cell_content = Cell_content::EMPTY;
					Position update_position = make_pair(strategyData->relative_x_y_to_start_position->first + x, strategyData->relative_x_y_to_start_position->second + y);
					auto stored_cell_content = strategyData->world_map->find(update_position);
					if (stored_cell_content == strategyData->world_map->end()) {
						strategyData->world_map->insert({ update_position, cell_content });
					}
				}
			}
		}
	};

	class StrategyExplore : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			int best_exploration_dir_score = 0;
			Dir best_exploration_dir = Dir::N;
			int xs[] = { -1, 0, 1 };
			int ys[] = { -1, 0, 1 };
			for each (int x in xs)
			{
				for each (int y in ys)
				{
					Position position_ahead = make_pair(strategyData->relative_x_y_to_start_position->first + x, strategyData->relative_x_y_to_start_position->second + y);
					if(strategyData->world_map->find(position_ahead)->second == Cell_content::EMPTY){
						int num_unexplored_cells = count_unexplored_cells_at(strategyData, position_ahead);
						if (best_exploration_dir_score < num_unexplored_cells) {
							best_exploration_dir = x_y_to_dir(x, y);
							best_exploration_dir_score = num_unexplored_cells;
						}
					}

				}
			}

			if (best_exploration_dir_score == 0) {
				strategyData->strategy_command.action = Action::PASS;
				strategyData->strategy_command.step_dir = Dir::N;

			}
			else {
				strategyData->strategy_command.action = Action::STEP;
				strategyData->strategy_command.step_dir = best_exploration_dir;
			}
		}

		int count_unexplored_cells_at(unique_ptr<StrategyData> &strategyData, Position position_ahead) {
			int count = 0;
			int xs[] = { -1, 0, 1 };
			int ys[] = { -1, 0, 1 };
			for each (int x in xs)
			{
				for each (int y in ys)
				{
					Position position_around = make_pair(position_ahead.first + x, position_ahead.second + y);
					auto stored_cell_content = strategyData->world_map->find(position_around);
					if (stored_cell_content == strategyData->world_map->end()) {
						count++;
					}
				}
			}
			return count;
		}
	};

	class StrategyLayTrap : public AbstractStrategy
	{
		int num_traps = 5;
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			if(num_traps > 0 && false){
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