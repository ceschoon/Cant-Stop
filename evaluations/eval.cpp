
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
	setStrategy("No risk 3", "No risk 3", "No risk 3", "No risk 3");
	//setStrategy("No risk 4", "No risk 4", "No risk 4", "No risk 4");
	//setStrategy("Next Score 1", "Next Score 1", "Next Score 1", "Next Score 1");
	
	Statistics stats = simulGames(1000, false);
	
	cout << fixed << setprecision(4);
	cout << "Avg wins:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_wins[p]; cout << endl;
	cout << "Avg turns:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_turns[p]; cout << endl;
	cout << "Avg losses: "; for (int p=0;p<4;p++) cout << setw(12) << stats.avg_lostRounds[p]/stats.avg_turns[p]; cout << endl;
	
	return 0;
}






////////////////////////////////////////////////////////////////////////////


