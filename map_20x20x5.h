/* map file, is included by world.c */

#define WORLD_SIZE_X 20
#define WORLD_SIZE_Y 20

//
// X is wall
// $ is treasure
// 1 is starting position player 1
// 2 is starting position player 2
//
static const char *initWorld[WORLD_SIZE_Y] =
  {"XXXXXXXXXXXXXXXXXXXX",
   "X1                 X",
   "X     XXXXX        X",
   "X $   XXXXX $      X",
   "X     XXXXX        X",
   "X     XXXXX        X",
   "X                  X",
   "X                  X",
   "X                  X",
   "X     XXXXXXXXXXX  X",
   "X                  X",
   "X     XX           X",
   "X     XX           X",
   "X     XX     XXX   X",
   "X     XX     XXX   X",
   "X     XX     XXX$  X",
   "X       $    XXX   X",
   "X    $             X",
   "X                 2X",
   "XXXXXXXXXXXXXXXXXXXX"};
