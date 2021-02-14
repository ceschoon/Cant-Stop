
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#include "../strategies.h"
#include "../evaluation_methods.h"

using namespace std;


void setStrategy(string s0="", string s1="", string s2="", string s3="");



////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
	//setStrategy("Trivial", "Trivial", "Trivial", "Trivial");
	setStrategy("No risk", "No risk", "No risk", "No risk");
	
	Statistics stats = simulGames();
	
	cout << fixed << setprecision(4);
	cout << "Avg wins:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.wins[p]; cout << endl;
	cout << "Avg turns:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.turns[p]; cout << endl;
	cout << "Avg losses: "; for (int p=0;p<4;p++) cout << setw(12) << stats.lostRounds[p]/stats.turns[p]; cout << endl;
	
	
	return 0;
}




////////////////////////////////////////////////////////////////////////////

void setStrategy(string s0, string s1, string s2, string s3)
{
	vector<string> strategies = {s0,s1,s2,s3};
	
	for (int i=0; i<4; i++)
	{
		if (strategies[i] == "Trivial" || strategies[i] == "")
		{
			if (i==0) decideToStop_0 = &decideToStop_always;
			if (i==1) decideToStop_1 = &decideToStop_always;
			if (i==2) decideToStop_2 = &decideToStop_always;
			if (i==3) decideToStop_3 = &decideToStop_always;
			
			if (i==0) selectCombination_0 = &selectCombination_any;
			if (i==1) selectCombination_1 = &selectCombination_any;
			if (i==2) selectCombination_2 = &selectCombination_any;
			if (i==3) selectCombination_3 = &selectCombination_any;
		}
		
		if (strategies[i] == "No risk") // No risk strategy
		{
			if (i==0) decideToStop_0 = &decideToStop_noRisk;
			if (i==1) decideToStop_1 = &decideToStop_noRisk;
			if (i==2) decideToStop_2 = &decideToStop_noRisk;
			if (i==3) decideToStop_3 = &decideToStop_noRisk;
			
			if (i==0) selectCombination_0 = &selectCombination_lessMarkersOrAny;
			if (i==1) selectCombination_1 = &selectCombination_lessMarkersOrAny;
			if (i==2) selectCombination_2 = &selectCombination_lessMarkersOrAny;
			if (i==3) selectCombination_3 = &selectCombination_lessMarkersOrAny;
		}
	}
}



////////////////////////////////////////////////////////////////////////////


