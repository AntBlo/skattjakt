#pragma once
#include "RobotInterface.h"

namespace Team_Lejla_Leon_Anton {

	class RobotControl :
		public RobotInterface
	{
	public:
		RobotControl();

		Command do_command(const Info &info) override;
	};

}

