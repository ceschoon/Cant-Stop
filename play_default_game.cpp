
#include "cant_stop.h"
//#include "strategies.h"

using namespace std;


int main()
{
	SimulParams sparam; initialiseDefaultParameters(sparam);
	SimulState  sstate; initialiseDefaultState(sstate);
	
	//setStrategy(); //shortcut in strategies.h equivalent to what follows
	
	decideToStop_0 = &decideToStop_always;
	decideToStop_1 = &decideToStop_always;
	decideToStop_2 = &decideToStop_always;
	decideToStop_3 = &decideToStop_always;
	
	selectCombination_0 = &selectCombination_any;
	selectCombination_1 = &selectCombination_any;
	selectCombination_2 = &selectCombination_any;
	selectCombination_3 = &selectCombination_any;
	
	//
	
	simulGame(sparam, sstate);
	
	return 0;
}

