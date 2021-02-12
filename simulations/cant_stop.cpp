#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>


using namespace std;


struct Combination
{
	int val1;
	int val2;
};


int min (int a, int b) {return (a<b)?a:b;}
int index(int num) {return num-2;}
int height(int i) {return (i<5)?(i+2):(12-i);}

void throwDice(int dice[], default_random_engine &gen);
void scanValidCombinations(int dice[], vector<Combination> &combinations);

bool canBeAdvanced(int columns[][4], int markers[][2], int player, int num, int &imarker);
void printColumns(int columns[][4], int markers[][2]);

bool decideToStop(int columns[][4], int markers[][2], int player, vector<Combination> &combinations);
Combination selectCombination(int columns[][4], int markers[][2], int player, vector<Combination> &combinations);




int main()
{
	//////////////////////////////
	// Random number generator
	
	random_device true_gen;
	int seed = true_gen();
	cout << "seed = " << seed << endl;
	default_random_engine gen(seed);
	
	
	/////////////////////////////
	// Game variables
	
	int player = 0;
	
	int colsCompleted[4];
	for (int i=0; i<4; i++) colsCompleted[i] = 0;
	
	
	int turns[4]; // counter of played turns for each player
	for (int i=0; i<4; i++) turns[i] = 0;
	
	int columns[11][4]; // col index, player
	for (int i=0; i<11; i++) for (int p=0; p<4; p++) columns[i][p] = -1;
	
	int markers[3][2]; // first [] denotes which of the 3 makers, second [] is to select the marker's column (use [0]) or the position in the column (use [1])
	int dice[4];
	
	//////////////////////////////////////
	// Game loop
	
	while (colsCompleted[0]<3 && colsCompleted[1]<3 &&
	       colsCompleted[2]<3 && colsCompleted[3]<3)
	{
		// Reset markers and round variables
		
		for (int i=0; i<3; i++) markers[i][0] = -1;
		for (int i=0; i<3; i++) markers[i][1] = -1;
		
		bool lostRound = false;
		int roundCounter = 0;
		turns[player]++;
		
		// Play rounds
		
		cout << "==========================================" << endl;
		cout << "  New turn for player: " << player;
		cout <<                         "  turn:" << setw(3) << turns[player];
		cout <<                                   "        " << endl;
		cout << "==========================================" << endl;
		
		while (!lostRound)
		{
			// Throw dice and analyse the possible pairings
			
			vector<Combination> combinations;
			throwDice(dice, gen); scanValidCombinations(dice, combinations);
			
			roundCounter++;
			
			// Check if the combinations are still playable
			
			vector<Combination> combinations_playable;
			for (int i=0; i<combinations.size(); i++)
			{
				int imarker1, imarker2;
				bool val1ok = canBeAdvanced(columns, markers, player, combinations[i].val1, imarker1);
				bool val2ok = canBeAdvanced(columns, markers, player, combinations[i].val2, imarker2);
				
				if (val1ok || val2ok) 
				{
					combinations_playable.push_back(combinations[i]);
				}
			}
			
			combinations = combinations_playable;
			
			// Show dices and print board in console
			
			if (roundCounter>1) cout << "------------------------------------------" << endl;
			cout << "Player:  " << player << endl;
			cout << "Turn:    " << turns[player] << endl;
			cout << "Round:   " << roundCounter << endl;
			cout << "Dices:   "; for (int i=0; i<4; i++) cout << dice[i] << " "; cout << endl;
			cout << "Pairs:   "; for (int i=0; i<combinations.size(); i++) cout << "{" << combinations[i].val1 << "," << combinations[i].val2 << "} "; cout << endl;
			
			// Force stop if none can be played
			
			if (combinations.size()==0)
			{
				cout << ">>> Player lost the round" << endl;
				lostRound = true;
				break;
			}
			
			// Find the most desirable combination
			// and place/advance markers
			
			Combination comb = selectCombination(columns, markers, player, combinations);
			int imarker;
			
			if ( canBeAdvanced(columns, markers, player, comb.val1, imarker) )
			{
				if (markers[imarker][0]==-1) // place
				{
					markers[imarker][0] = index(comb.val1);
					markers[imarker][1] = columns[index(comb.val1)][player]+1;
				}
				else markers[imarker][1]++; // advance
			}
			
			if ( canBeAdvanced(columns, markers, player, comb.val2, imarker) &&
			     comb.val2 != comb.val1 )
			{
				if (markers[imarker][0]==-1) // place
				{
					markers[imarker][0] = index(comb.val2);
					markers[imarker][1] = columns[index(comb.val2)][player]+1;
				}
				else markers[imarker][1]++; // advance
			}
			
			// Print board state after round
			
			cout << "Board:   " << endl; printColumns(columns, markers);
			
			// Decide whether we stop or not
			
			if (decideToStop(columns, markers, player, combinations)) break;
		}
		
		// Save marker positions if player did not loose the last round
		
		if (!lostRound)
		{
			for (int i=0; i<3; i++) if (markers[i][0]>=0)
			{
				int col = markers[i][0];
				int pos = markers[i][1];
				columns[col][player] = pos;
			}
		}
		
		// Count number of completed columns
		// and update next player
		
		for (int p=0; p<4; p++) colsCompleted[p] = 0;
		for (int p=0; p<4; p++) for (int i=0; i<11; i++)
		if (columns[i][p]==height(i)-1) colsCompleted[p]++;
		
		player = (player+1)%4;
	}
	
	
	/////////////////////////////////////
	// Print final board state
	
	cout << "==========================================" << endl;
	cout << "  Game Over                               " << endl;
	cout << "==========================================" << endl;
	
	cout << "Columns completed: "; for (int p=0; p<4; p++) cout << colsCompleted[p] << " "; cout << endl;
	
	printColumns(columns, markers);
	
	/////////////////////////////////////
	// End of game loop
	
	return 0;
}



void throwDice(int dice[], default_random_engine &gen)
{
	uniform_int_distribution<int> dist(1,6);
	for (int i=0; i<4; i++) dice[i] = dist(gen);
}



void scanValidCombinations(int dice[], vector<Combination> &combinations)
{
	combinations.clear();
	
	// First check that all dice do not have the same value
	if ((dice[0] == dice[1]) && (dice[1] == dice[2]) && (dice[2] == dice[3])) return;
	
	// Then try to group dice {0 1} and {2 3}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[2] && dice[1] == dice[3]) {;}
	else if (dice[0] == dice[3] && dice[1] == dice[2]) {;}
	else 
	{
		// record that combination
		struct Combination comb = {dice[0] + dice[1], dice[2] + dice[3]};
		combinations.push_back(comb);
	}
	
	// Then try to group dice {0 2} and {1 3}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[1] && dice[2] == dice[3]) {;}
	else if (dice[0] == dice[3] && dice[2] == dice[1]) {;}
	else 
	{
		// record that combination
		struct Combination comb = {dice[0] + dice[2], dice[1] + dice[3]};
		combinations.push_back(comb);
	}
	
	// Then try to group dice {0 3} and {1 2}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[1] && dice[3] == dice[2]) {;}
	else if (dice[0] == dice[2] && dice[3] == dice[1]) {;}
	else 
	{
		// record that combination
		struct Combination comb = {dice[0] + dice[3], dice[1] + dice[2]};
		combinations.push_back(comb);
	}
}



bool canBeAdvanced(int columns[][4], int markers[][2], int player, int num, int &imarker)
{
	// Check that the column is not completed already
	
	bool completed = false;
	for (int p=0; p<4; p++) if (columns[index(num)][p]==height(index(num))-1) completed = true;
	
	if (completed)
	{
		imarker = -1;
		return false;
	}
	
	// Check if there is a marker already on this column
	
	for (int i=0; i<3; i++) if (markers[i][0]==index(num))
	{
		imarker = i;
		return true;
	}
	
	// Check if there is a marker not yet assigned
	
	for (int i=0; i<3; i++) if (markers[i][0]==-1)
	{
		imarker = i;
		return true;
	}
	
	// If we arrive here it means we cannot advance
	
	imarker = -1;
	return false;
}



void printColumns(int columns[][4], int markers[][2])
{
	cout << endl;
	for (int n=2; n<=12; n++)
	{
		// check if column completed
		
		bool completed = false; int owner = -1;
		for (int p=0; p<4; p++) if (columns[index(n)][p]==height(index(n))-1)
		{
			completed = true;
			owner = p;
		}
		
		cout << setw(3) << n << " : ";
		
		// if completed, fill with crosses
		if (completed)
		{
			for (int j=0; j<height(index(n)); j++) cout << "------";
			cout << " (" << owner << ")";
		}
		
		// else, print player number at their current position
		// and symbols indicating the position of the markers
		else
		{
			for (int j=0; j<height(index(n)); j++)
			{
				stringstream ss;
				for (int p=0; p<4; p++) if (columns[index(n)][p]==j) ss << p;
				
				char symbol = '.';
				for (int i=0; i<3; i++) 
				{
					if (markers[i][0]==index(n) && markers[i][1]==j) 
					symbol = '#';
				}
				
				cout << setw(5) << ss.str() << symbol;
			}
		}
		
		cout << endl;
	}
	cout << endl;
}



bool decideToStop(int columns[][4], int markers[][2], int player, vector<Combination> &combinations)
{
	// TODO (currently trivial -- stop every time)
	
	return true;
}



// Note: sort values in selected combination so that the most desirable 
// value on which to place a marker is in the first position. Otherwise
// the wrong value can be placed in case only one marker is available.

Combination selectCombination(int columns[][4], int markers[][2], int player, vector<Combination> &combinations)
{
	// TODO (currently trivial -- choose always the first one)
	
	return combinations[0];
}




