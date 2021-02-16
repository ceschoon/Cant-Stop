
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#include "../strategies.h"
#include "../evaluation_methods.h"

using namespace std;




////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
	//setStrategy("Trivial", "Trivial", "Trivial", "Trivial");
	//setStrategy("No risk", "No risk", "No risk", "No risk");
	//setStrategy("No risk 2", "No risk 2", "No risk 2", "No risk 2");
	//setStrategy("No risk 3", "No risk 3", "No risk 3", "No risk 3");
	//setStrategy("No risk 4", "No risk 4", "No risk 4", "No risk 4");
	//setStrategy("Next Score 1", "Next Score 1", "Next Score 1", "Next Score 1");
	//setStrategy("Next Score 2", "Next Score 2", "Next Score 2", "Next Score 2");
	
	setStrategy("Next Score 2", "No risk 3", "No risk 3", "No risk 3");
	//setStrategy("No risk 3", "No risk 3", "No risk 3", "Next Score 2");
	
	Statistics stats = simulGames(10000, false, true);
	//simulGames(1, false, false);
	
	cout << fixed << setprecision(4);
	cout << "Avg wins:    "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_wins[p]; cout << endl;
	//cout << "Avg turns:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_turns[p]; cout << endl;
	//cout << "Avg losses:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_lossFractions[p]; cout << endl;
	cout << "Avg wturns:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_wturns[p]; cout << endl;
	cout << "Avg wlosses: "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_wlossFractions[p]; cout << endl;
	
	
	cout << endl;
	cout << "Err wins:    "; for (int p=0;p<4;p++) cout << setw(12) << stats.std_wins[p]/sqrt(stats.numGames); cout << endl;
	//cout << "Err turns:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.std_turns[p]/sqrt(stats.numGames); cout << endl;
	//cout << "Err losses:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.std_lossFractions[p]/sqrt(stats.numGames); cout << endl;
	cout << "Err wturns:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.std_wturns[p]/sqrt(stats.numGames*stats.avg_wins[p]); cout << endl;
	cout << "Err wlosses: "; for (int p=0;p<4;p++) cout << setw(12) << stats.std_wlossFractions[p]/sqrt(stats.numGames*stats.avg_wins[p]); cout << endl;
	
	cout << endl;
	cout << "Max turns:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.max_turns[p]; cout << endl;
	cout << "Max lostRd.: "; for (int p=0;p<4;p++) cout << setw(12) << stats.max_lostRounds[p]; cout << endl;
	
	
	return 0;
}






////////////////////////////////////////////////////////////////////////////


