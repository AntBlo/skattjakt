#pragma once
#include "game_common.h"
#include <RobotInterface.h>
#include <deque>
#include <set>
#include <map>
#include <memory>
#include <stdlib.h>

using namespace std;

namespace Team_Lejla_Leon_Anton {

	typedef std::pair<int, int> Position;

	struct StrategyData
	{
		Info info;
		Command strategy_command;
		map<Position, Cell_content>* world_map;
		Position* relative_x_y_to_start_position;
		Command last_command;
		StrategyData(Info info, Command strategy_command, map<Position, Cell_content> *world_map, Position* relative_x_y_to_start_position, Command last_command)
		{
			this->info = info;
			this->strategy_command = strategy_command;
			this->world_map = world_map;
			this->relative_x_y_to_start_position = relative_x_y_to_start_position;
			this->last_command = last_command;
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

	class StrategyUpdateRobotPosition : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			if (strategyData->last_command.action == Action::STEP
				&& strategyData->info.last_event != Event::COLLISION
				&& strategyData->info.last_event != Event::TRAPPED)
			{
				auto x = 0;
				auto y = 0;
				dir_to_x_y(strategyData->last_command.step_dir, x, y);
				strategyData->relative_x_y_to_start_position->first += x;
				strategyData->relative_x_y_to_start_position->second += y;
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
					Position position_ahead = make_pair(
						strategyData->relative_x_y_to_start_position->first + x, 
						strategyData->relative_x_y_to_start_position->second + y
					);
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
				//go_towards_nearest_unexplored_empty_cell();
				deque<Position> empty_cells;
				set<Position> visited_cells;
				set<Position> seen_cells;
				map<Position, Position> backtrack_tree;
				Position start_cell = *strategyData->relative_x_y_to_start_position;
				empty_cells.push_back(start_cell);

				int best_cell_score = 0;
				Position best_cell;

				while (!empty_cells.empty()) {
					Position current_cell = empty_cells.front();
					empty_cells.pop_front();
					visited_cells.insert(current_cell);
					int xs[] = { -1, 0, 1 };
					int ys[] = { -1, 0, 1 };

					int num_unexplored_cells = count_unexplored_cells_at(strategyData, current_cell);
					if (best_cell_score < num_unexplored_cells) {
						best_cell_score = num_unexplored_cells;
						best_cell = current_cell;
					}

					for each (int x in xs)
					{
						for each (int y in ys)
						{
							if (x == 0 && y == 0)continue;
							Position next_cell = make_pair(
								current_cell.first + x,
								current_cell.second + y
							);
							auto stored_cell_content = strategyData->world_map->find(next_cell);
							if (stored_cell_content == strategyData->world_map->end())continue;

							auto next_cell_is_empty = stored_cell_content->second == Cell_content::EMPTY;
							auto has_not_been_visited = !visited_cells.count(stored_cell_content->first);
							if (next_cell_is_empty
								&& has_not_been_visited)
							{
								if(!seen_cells.count(stored_cell_content->first)){
									empty_cells.push_back(stored_cell_content->first);
									seen_cells.insert(stored_cell_content->first);
									if (current_cell != *strategyData->relative_x_y_to_start_position) {
										if (backtrack_tree.find(next_cell) == backtrack_tree.end())
											backtrack_tree.insert({ next_cell, current_cell });
									}
								}
							}
						}
					}
				}
				bool cell_exist = true;
				Position nearby_cell_to_go_to = best_cell;
				do {
					auto stored_cell_content = backtrack_tree.find(nearby_cell_to_go_to);
					cell_exist = stored_cell_content != backtrack_tree.end();
					if (cell_exist) {
						nearby_cell_to_go_to = stored_cell_content->second;
					}
				} while (cell_exist);
				int x = nearby_cell_to_go_to.first - strategyData->relative_x_y_to_start_position->first;
				int y = nearby_cell_to_go_to.second - strategyData->relative_x_y_to_start_position->second;
				best_exploration_dir = x_y_to_dir(x, y);
				if (best_cell_score != 0) {
					strategyData->strategy_command.action = Action::STEP;
					strategyData->strategy_command.step_dir = best_exploration_dir;
				}
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

	class StrategyLayTrapByOpponent : public AbstractStrategy
	{
	public:
		int* num_traps_left;

		StrategyLayTrapByOpponent(int* num_traps_left) {
			this->num_traps_left = num_traps_left;
		}

		void apply(unique_ptr<StrategyData>& strategyData) {
			if(*this->num_traps_left > 0){
				bool found_other_robot = false;
				int robot_x = 0;
				int robot_y = 0;
				int xs[] = { -1, 0, 1 };
				int ys[] = { -1, 0, 1 };
				for each (int x in xs)
				{
					for each (int y in ys)
					{
						if (x == 0 && y == 0)continue;
						auto cell_content = strategyData->info.neighbor_cells[1 - y][x + 1];
						if (cell_content == Cell_content::ROBOT) {
							found_other_robot = true;
							robot_x = x;
							robot_y = y;
						}
					}
				}
				if (found_other_robot) {
					auto cell_content = strategyData->info.neighbor_cells[robot_y][robot_x];
					if(abs(robot_x) == abs(robot_y)){
						bool trap_laid = try_lay_trap_at(strategyData, robot_x, 0);
						if (!trap_laid)
							try_lay_trap_at(strategyData, 0, robot_y);
					}
					else if (abs(robot_x) == 1) {
						bool trap_laid = try_lay_trap_at(strategyData, robot_x, -1);
						if (!trap_laid)
							try_lay_trap_at(strategyData, robot_x, 1);
					}
					else if (abs(robot_y) == 1) {
						bool trap_laid = try_lay_trap_at(strategyData, -1, robot_y);
						if (!trap_laid)
							try_lay_trap_at(strategyData, 1, robot_y);
					}
				}
			}
		}

		bool try_lay_trap_at(std::unique_ptr<Team_Lejla_Leon_Anton::StrategyData>& strategyData, int x, int y)
		{
			Position put_trap_at_position = make_pair(strategyData->relative_x_y_to_start_position->first + x, strategyData->relative_x_y_to_start_position->second + y);
			auto stored_cell_content = strategyData->world_map->find(put_trap_at_position);
			if (stored_cell_content->second == Cell_content::EMPTY) {
				strategyData->strategy_command.action = Action::PLACE_TRAP;
				Dir dir = x_y_to_dir(x, y);
				strategyData->strategy_command.step_dir = dir;
				return true;
			}
			else {
				return false;
			}
		}
	};

	class StrategyTakeTreasure : public AbstractStrategy
	{
	public:
		void apply(unique_ptr<StrategyData>& strategyData) {
			int xs[] = { -1, 0, 1 };
			int ys[] = { -1, 0, 1 };
			for each (int x in xs)
			{
				for each (int y in ys)
				{
					auto cell_content = strategyData->info.neighbor_cells[1 - y][x + 1];
					if (cell_content == Cell_content::TREASURE) {
						strategyData->strategy_command.action = Action::STEP;
						strategyData->strategy_command.step_dir = x_y_to_dir(x, y);
					}
				}
			}
		}
	};

	class StrategyUpdateLaidTrapLocation : public AbstractStrategy
	{
	public:
		int* num_traps_left;

		StrategyUpdateLaidTrapLocation(int* num_traps_left) {
			this->num_traps_left = num_traps_left;
		}
		void apply(unique_ptr<StrategyData>& strategyData) {
			if (strategyData->strategy_command.action == Action::PLACE_TRAP)
			{
				Dir trap_laid_in_dir = strategyData->strategy_command.step_dir;
				auto x = 0;
				auto y = 0;
				dir_to_x_y(strategyData->strategy_command.step_dir, x, y);
				x = strategyData->relative_x_y_to_start_position->first + x;
				y = strategyData->relative_x_y_to_start_position->second + y;
				Position position_ahead = make_pair(x, y);
				auto stored_cell_content = strategyData->world_map->find(position_ahead);
				stored_cell_content->second = Cell_content::TRAP;
				(*this->num_traps_left)--;
			}
		}
	};
}