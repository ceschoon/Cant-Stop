
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
	setStrategy("No risk", "No risk", "No risk", "No risk");
	
	Statistics stats = simulGames();
	
	cout << fixed << setprecision(4);
	cout << "Avg wins:   "; for (int p=0;p<4;p++) cout << setw(12) << stats.wins[p]; cout << endl;
	cout << "Avg turns:  "; for (int p=0;p<4;p++) cout << setw(12) << stats.turns[p]; cout << endl;
	cout << "Avg losses: "; for (int p=0;p<4;p++) cout << setw(12) << stats.lostRounds[p]/stats.turns[p]; cout << endl;
	
	
	return 0;
}






////////////////////////////////////////////////////////////////////////////


