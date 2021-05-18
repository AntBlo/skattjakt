#pragma once
#include "RobotInterface.h"

namespace Player2 {

	class RobotControl :
		public RobotInterface
	{
	public:
		RobotControl();

		Command do_command(const Info &info) override;
	};

}

