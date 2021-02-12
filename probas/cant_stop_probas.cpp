#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>


using namespace std;


int index(int num) {return num-2;}


void count_from_combination(int dice[], int occur[])
{
	// Prepare a raw list to hold all the numbers that appears
	vector<int> output_raw;
	
	// First check that all dice do not have the same value
	if ((dice[0] == dice[1]) && (dice[1] == dice[2]) && (dice[2] == dice[3])) return;
	
	// Then try to group dice {0 1} and {2 3}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[2] && dice[1] == dice[3]) {;}
	else if (dice[0] == dice[3] && dice[1] == dice[2]) {;}
	else 
	{
		// get values from this grouping
		output_raw.push_back( dice[0] + dice[1] );
		output_raw.push_back( dice[2] + dice[3] );
	}
	
	// Then try to group dice {0 2} and {1 3}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[1] && dice[2] == dice[3]) {;}
	else if (dice[0] == dice[3] && dice[2] == dice[1]) {;}
	else 
	{
		// get values from this grouping
		output_raw.push_back( dice[0] + dice[2] );
		output_raw.push_back( dice[1] + dice[3] );
	}
	
	// Then try to group dice {0 3} and {1 2}
	// Check that the two groups are not the same -> if yes do nothing
	if (dice[0] == dice[1] && dice[3] == dice[2]) {;}
	else if (dice[0] == dice[2] && dice[3] == dice[1]) {;}
	else 
	{
		// get values from this grouping
		output_raw.push_back( dice[0] + dice[3] );
		output_raw.push_back( dice[1] + dice[2] );
	}
	
	// Count output number only once
	
	vector<int> output_unique;
	
	for (int num : output_raw)
	{
		bool has_been_counted = false;
		for (int num2 : output_unique) if (num==num2) has_been_counted = true;
		
		if (!has_been_counted) 
		{
			output_unique.push_back(num);
			occur[index(num)] ++;
		}
	}
	
	// Debug report
	
	//cout << "testing dices "; for (int i=0; i<4; i++) cout << dice[i] << " "; cout << endl;
	//cout << "got output_raw = "; for (int num : output_raw) cout << num << " "; cout << endl;
	//cout << "got output_unique = "; for (int num : output_unique) cout << num << " "; cout << endl;
	//cout << endl;
}


int main()
{
	// Init
	
	int occur[11];  // from 2 to 12 both included
	double probas[11];
	
	for (int i=0; i<11; i++) occur[i] = 0;
	
	// Brute force all possibilities
	
	for (int i1=1; i1<=6; i1++)
	for (int i2=1; i2<=6; i2++)
	for (int i3=1; i3<=6; i3++)
	for (int i4=1; i4<=6; i4++)
	{
		int dice[4] = {i1,i2,i3,i4};
		count_from_combination(dice, occur);
	}
	
	// Analysis: probabilities
	
	cout << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "Probability to get number from dice throw      " << endl;
	cout << "-----------------------------------------------" << endl;
	cout << endl;
	
	for (int i=0; i<11; i++) 
	{
		probas[i] = double(occur[i])/(6*6*6*6);
		cout << "proba for " << i+2 << " is " << probas[i] << endl;
		//cout << "occurrence for " << i+2 << " is " << occur[i] << endl;
	}
	
	// Analysis: difference from thumb rule: (n-1)*0.11 where n is min of n and 14-n
	
	cout << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "Difference from thumb rule (n-1)*0.11          " << endl;
	cout << "-----------------------------------------------" << endl;
	cout << endl;
	
	for (int i=0; i<11; i++) 
	{
		int n = (i<5)?(i+2):(12-i);
		cout << "difference for " << i+2 << " is " << probas[i]-(n-1)*0.11 << endl;
	}
	
	return 0;
}








