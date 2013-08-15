#ifndef MULTIPLAYERLAUNCHER_RE68Q1BO
#define MULTIPLAYERLAUNCHER_RE68Q1BO

#include "player_group.h"

class MultiplayerLauncher
{
	public:
		int getState();
		void process(PlayerGroup& playerGroup);
		Grid* getGrid();
		void setGrid(Grid& g);
	private:
		int state;
		Grid grid;
};

#endif /* end of include guard: MULTIPLAYERLAUNCHER_RE68Q1BO */
