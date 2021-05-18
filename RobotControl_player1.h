#pragma once
#include "RobotInterface.h"

namespace Player1 {

	class RobotControl :
		public RobotInterface
	{
	public:
		RobotControl();

		Command do_command(const Info &info) override;
	};

}

