
#include "../cant_stop.h"

using namespace std;



////////////////////////////////////////////////////////////////////////////
// Headers and variables that must be seen inside some function but cannot 
// be given in argument list --> global status

Combination selectCombination_target(int columns[][4], int markers[][2], int player, vector<Combination> &combinations);

int target = -1;



////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
	//////////////////////////////
	// Options
	
	bool specific_target = false;
	bool already_marked  = false;
	int  columns_to_fill = 0; // staring from center
	
	if (argc>1) if (string(argv[1])=="true") specific_target = true;
	if (argc>2) if (string(argv[2])=="true") already_marked  = true;
	if (argc>3) columns_to_fill = stoi(argv[3]);
	
	cout << "Options:" << endl;
	cout << "  specific_target = " << specific_target << endl;
	cout << "  already_marked  = " << already_marked  << endl;
	cout << "  columns_to_fill = " << columns_to_fill << endl;
	cout << endl;
	
	
	//////////////////////////////////////////
	// Simulation parameters
	
	SimulParams sparam;
	
	sparam.singleplayer = true;
	sparam.playOneTurn = true;
	sparam.silent = true;
	sparam.seed = 0; // will be overwritten
	
	decideToStop = &decideToStop_never;
	if (specific_target) selectCombination = &selectCombination_target;
	else                 selectCombination = &selectCombination_any;
	
	int numGames = 10000;
	
	
	////////////////////////////////////////////////
	// Rng for choosing seeds of each simulation
	
	random_device true_gen;
	int seed = -1; while (seed<0) seed = true_gen();
	
	uniform_int_distribution<int> dist(0,1000000);
	default_random_engine gen(seed);
	
	
	////////////////////////////////
	// Simulations and analysis
	
	double progression_avg[11];
	
	for (target=2; target<=12; target++)
	{
		// Initial simulation state (fill columns and position markers)
		
		SimulState sstate_in; initialiseDefaultState(sstate_in);
		
		int fill_order[11] = {7,6,8,5,9,4,10,3,11,2,12};
		int columns_filled = 0;
		
		for (int i=0; i<11 && columns_filled<columns_to_fill; i++)
		{
			int n = fill_order[i];
			if (n!=target)
			{
				sstate_in.columns[index(n)][3] = height(index(n))-1;
				columns_filled++;
			}
		}
		
		if (already_marked)
		{
			sstate_in.markers[0][0] = index(target);
			sstate_in.markers[0][1] = 0;
		}
		
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
				if (already_marked) progression[igame] =   sstate_out.markers[i][1];
				else                progression[igame] = 1+sstate_out.markers[i][1];
			}
		}
		
		// Compute average progression in the target column
		
		progression_avg[index(target)] = 0;
		for (int igame = 0; igame<numGames; igame++)
			progression_avg[index(target)] += progression[igame];
		progression_avg[index(target)] /= numGames;
	}
	
	
	///////////////////////////
	// Print results
	
	for (target=2; target<=12; target++)
	{
		cout << setw(2) << target << " : ";
		cout << setw(8) << progression_avg[index(target)] << " : ";
		cout << setw(8) << progression_avg[index(7)]/progression_avg[index(target)];
		cout << endl;
	}
	
	
	return 0;
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
	
	vector<Combination> targetcomb;
	
	for (int i=0; i<combinations.size(); i++)
	{
		// If both values are the target value, select without hesitation
		if (combinations[i].val1==target && combinations[i].val2==target)
			return combinations[i];
		
		// If one value is the target, record this combination with
		// target in first position, and other value in second position
		if (combinations[i].val1==target)
		{
			struct Combination comb = {target,combinations[i].val2};
			targetcomb.push_back(comb);
		}
		else if (combinations[i].val2==target)
		{
			struct Combination comb = {target,combinations[i].val1};
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

