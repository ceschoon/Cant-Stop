
#include "cant_stop.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////
// Headers and variables that must be seen inside some function but cannot 
// be given in argument list --> global status

Combination selectCombination_target(int columns[][4], int markers[][2], int player, vector<Combination> &combinations);

double expectation(int target, int columns[11][4], int markers[3][2], int numGames);


////////////////////////////////////////////////////////////////////////////


double expectation(int target, int columns[11][4], int markers[3][2], int numGames)
{
	//////////////////////////////////////////
	// Simulation parameters
	
	SimulParams sparam;
	
	sparam.singleplayer = true;
	sparam.playOneTurn = true;
	sparam.silent = true;
	sparam.seed = 0; // will be overwritten
	
	decideToStop_0 = &decideToStop_never;
	if (target>0) selectCombination_0 = &selectCombination_target;
	else          selectCombination_0 = &selectCombination_any;
	
	stop_params = &target;
	
	
	////////////////////////////////////////////////
	// Rng for choosing seeds of each simulation
	
	random_device true_gen;
	int seed = -1; while (seed<0) seed = true_gen();
	
	uniform_int_distribution<int> dist(0,100000000);
	default_random_engine gen(seed);
	
	
	/////////////////////////////////////////
	// Initial state
	
	SimulState sstate_in; initialiseDefaultState(sstate_in);
	
	for (int i=0; i<11; i++) for (int j=0; j<4; j++) sstate_in.columns[i][j] = columns[i][j];
	for (int i=0; i<3; i++)  for (int j=0; j<2; j++) sstate_in.markers[i][j] = markers[i][j];
	
	
	///////////////////////////////////////////////////////////////////
	// Runs games and record progression for marker in target column
	// Note: there could be none, so init to zero
	
	vector<int> progression(numGames,0.0);
	for (int igame = 0; igame<numGames; igame++)
	{
		sparam.seed = dist(gen);
		SimulState sstate_out = sstate_in;
		simulGame(sparam, sstate_out);
		
		for (int i=0; i<3; i++) if (sstate_out.markers[i][0]==index(target))
		{
			progression[igame] = sstate_out.markers[i][1] + 1;
		}
	}
	
	////////////////////////////////////////////////////////
	// Return average progression in the target column
	
	int sum = 0;
	for (int igame = 0; igame<numGames; igame++) sum += progression[igame];
	
	return double(sum)/numGames;
}



////////////////////////////////////////////////////////////////////////////

// Note: sort values in selected combination so that the most desirable 
// value on which to place a marker is in the first position. Otherwise
// the wrong value can be placed in case only one marker is available.

// Choose in priority the combination that contains the target value
// Then choose combinations with the most likely values (close to 7)

Combination selectCombination_target(int columns[][4], int markers[][2], int player, vector<Combination> &combinations)
{
	// First, search for combinations containing the target value
	
	int _target = *(int*) stop_params;
	vector<Combination> targetcomb;
	
	for (int i=0; i<combinations.size(); i++)
	{
		// If both values are the target value, select without hesitation
		if (combinations[i].val1==_target && combinations[i].val2==_target)
			return combinations[i];
		
		// If one value is the target, record this combination with
		// target in first position, and other value in second position
		if (combinations[i].val1==_target)
		{
			struct Combination comb = {_target,combinations[i].val2};
			targetcomb.push_back(comb);
		}
		else if (combinations[i].val2==_target)
		{
			struct Combination comb = {_target,combinations[i].val1};
			targetcomb.push_back(comb);
		}
	}
	
	// Within combinations containing the target, select the one
	// where the second value is the most likely to get another time
	// (closest to 7)
	
	if (targetcomb.size()>0)
	{
		int ibest = 0;
		for (int i=0; i<targetcomb.size(); i++)
		{
			if (abs(targetcomb[i].val2-7)<abs(targetcomb[ibest].val2-7)) 
				ibest = i;
		}
		
		return targetcomb[ibest];
	}
	
	// If no combination contains the target value, search the combination
	// with the *two* most likely values to get another time 
	
	if (targetcomb.size()==0)
	{
		// Find best combination
		
		int ibest = 0;
		for (int i=0; i<combinations.size(); i++)
		{
			if (abs(combinations[i].val1-7)<abs(combinations[ibest].val1-7) && 
			    abs(combinations[i].val2-7)<abs(combinations[ibest].val2-7)) 
				ibest = i;
		}
		
		// If there is not combination with twice the same value,
		// select this one to save a marker
		// Note: not clear it this is best to do or not,
		//       the data shows little difference with and without 
		
		for (int i=0; i<combinations.size(); i++)
			if (combinations[i].val1 == combinations[i].val2) ibest = i;
		
		// Put the value closest to 7 in the first position
		
		struct Combination comb = combinations[ibest];
		if (abs(comb.val2-7)<abs(comb.val1-7))
		{
			comb.val1 = combinations[ibest].val2;
			comb.val2 = combinations[ibest].val1;
		}
		
		return comb;
	}
	
	
	// default return statement
	return combinations[0];
}


