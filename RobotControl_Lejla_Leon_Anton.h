#pragma once
#include "RobotInterface.h"
#include "Strategies.h"

using namespace std;

namespace Team_Lejla_Leon_Anton {

	class RobotControl :
		public RobotInterface
	{
	public:
		RobotControl();
		map<pair<int, int>, Cell_content> world_map;
		pair<int, int> relative_x_y_to_start_position;
		vector<AbstractStrategy*> strategies;
		Command last_command;
		int num_traps_left;

		Command do_command(const Info &info) override;
	};
}

