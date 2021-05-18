#pragma once

// Common declarations

// Used both in robot interface and in world:
enum class Cell_content { EMPTY, WALL, TREASURE, TRAP, ROBOT };
enum class Event { OK, COLLISION, TREASURE_CAPTURED, PLACED_TRAP, TRAPPED };

// In each round, robot can PASS (do nothing) or STEP in any direction
// or PLACE_TRAP in some direction

enum class Action { PASS, STEP, PLACE_TRAP };
enum class Dir { N = 0, E, S, W, NE, SE, SW, NW };

// RobotControl should return this struct
struct Command {
	Action action;
	Dir step_dir;
};
