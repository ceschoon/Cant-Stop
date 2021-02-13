
#include "cant_stop.h"

using namespace std;


int main()
{
	SimulParams sparam; initialiseDefaultParameters(sparam);
	SimulState  sstate; initialiseDefaultState(sstate);
	
	decideToStop = &decideToStop_always;
	selectCombination = &selectCombination_any;
	
	simulGame(sparam, sstate);
	
	return 0;
}

