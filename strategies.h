
#include "cant_stop.h"


////////////////////////////////////////////////////////////////////////////

// main function to set strategy for each player
void setStrategy(string s0="", string s1="", string s2="", string s3="");

// stopping methods
bool decideToStop_noRisk(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);
bool decideToStop_score(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);

// selection methods
Combination selectCombination_lessMarkersOrAny(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);
Combination selectCombination_lessMarkersOrBestProg(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);
Combination selectCombination_lessMarkersOrFar7(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);
Combination selectCombination_lessMarkersOrNear7(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);
Combination selectCombination_immediateNextScore(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);

// score functions
void *score_params;
int (*score_)(int columns[11][4], int markers[3][2], int player);
int score_1(int columns[11][4], int markers[3][2], int player);

// secondary functions / utility
int numMarkersThatWillBeUsed(Combination comb, int markers[3][2]);
double columnProgression(int columns[11][4], int player, int icol);




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
		
		if (strategies[i] == "No risk")
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
		
		if (strategies[i] == "No risk 2")
		{
			if (i==0) decideToStop_0 = &decideToStop_noRisk;
			if (i==1) decideToStop_1 = &decideToStop_noRisk;
			if (i==2) decideToStop_2 = &decideToStop_noRisk;
			if (i==3) decideToStop_3 = &decideToStop_noRisk;
			
			if (i==0) selectCombination_0 = &selectCombination_lessMarkersOrBestProg;
			if (i==1) selectCombination_1 = &selectCombination_lessMarkersOrBestProg;
			if (i==2) selectCombination_2 = &selectCombination_lessMarkersOrBestProg;
			if (i==3) selectCombination_3 = &selectCombination_lessMarkersOrBestProg;
		}
		
		if (strategies[i] == "No risk 3")
		{
			if (i==0) decideToStop_0 = &decideToStop_noRisk;
			if (i==1) decideToStop_1 = &decideToStop_noRisk;
			if (i==2) decideToStop_2 = &decideToStop_noRisk;
			if (i==3) decideToStop_3 = &decideToStop_noRisk;
			
			if (i==0) selectCombination_0 = &selectCombination_lessMarkersOrFar7;
			if (i==1) selectCombination_1 = &selectCombination_lessMarkersOrFar7;
			if (i==2) selectCombination_2 = &selectCombination_lessMarkersOrFar7;
			if (i==3) selectCombination_3 = &selectCombination_lessMarkersOrFar7;
		}
		
		if (strategies[i] == "No risk 4")
		{
			if (i==0) decideToStop_0 = &decideToStop_noRisk;
			if (i==1) decideToStop_1 = &decideToStop_noRisk;
			if (i==2) decideToStop_2 = &decideToStop_noRisk;
			if (i==3) decideToStop_3 = &decideToStop_noRisk;
			
			if (i==0) selectCombination_0 = &selectCombination_lessMarkersOrNear7;
			if (i==1) selectCombination_1 = &selectCombination_lessMarkersOrNear7;
			if (i==2) selectCombination_2 = &selectCombination_lessMarkersOrNear7;
			if (i==3) selectCombination_3 = &selectCombination_lessMarkersOrNear7;
		}
		
		if (strategies[i] == "Next Score 1")
		{
			score_ = &score_1;
			
			int threshold = 30;
			stop_params = &threshold;
			
			if (i==0) decideToStop_0 = &decideToStop_score;
			if (i==1) decideToStop_1 = &decideToStop_score;
			if (i==2) decideToStop_2 = &decideToStop_score;
			if (i==3) decideToStop_3 = &decideToStop_score;
			
			if (i==0) selectCombination_0 = &selectCombination_immediateNextScore;
			if (i==1) selectCombination_1 = &selectCombination_immediateNextScore;
			if (i==2) selectCombination_2 = &selectCombination_immediateNextScore;
			if (i==3) selectCombination_3 = &selectCombination_immediateNextScore;
		}
	}
}




////////////////////////////////////////////////////////////////////////////

// "No risk" Strategy

// Choose to stop as soon as all the markers are placed. The idea is to 
// avoid as much as possible the risk of loosing the current progress,
// even if it is not much. Because when many columns are filled there is
// a high probability of loosing the round despite being a marker left,
// we have to switch at some point to 2 then 1 marker placed for the
// stopping criterion.

// Note: Interestingly, switching to 2 or 1 marker to stop when columns
// are filled does not improve the performance. So ignoring this problem 
// and continuing to play untill 3 markers are placed is actually not bad. 
// As we want this be be a "no risk" strategy, we must however keep the 
// fraction of lost rounds small. Here, another surprise comes once again
// from simulations which show that the fraction of lost rounds it quite
// low even if one plays until 3 markers are placed. We get 0.043 when we
// always stop at 3 markers (infinite threshold limit) compared to 0.019
// when we always stop at 1 marker (zero threshold limit). The difference
// being small and the performance high when we stop at 3 markers, we
// choose to ignore the issue of filled columns and set the threshold to
// high "infinite" values.

/*
int numMarkersPlacedToStop(int columns[11][4])
{
	// Weight each column (a less likely column that is filled poses a
	// smaller risk than a common one).
	
	double weights[11];
	
	weights[index(7)] = 6.0;
	weights[index(6)] = weights[index( 8)] = 5.0;
	weights[index(5)] = weights[index( 9)] = 4.0;
	weights[index(4)] = weights[index(10)] = 3.0;
	weights[index(3)] = weights[index(11)] = 2.0;
	weights[index(2)] = weights[index(12)] = 1.0;
	
	double threshold1 = 20.0;
	double threshold2 = 30.0;
	
	// Evaluate the current thread posed by the filled columns
	
	double dangerScore = 0.0;
	for (int i=0; i<11; i++) for (int p=0; p<4; p++) 
	if (columns[i][p] == height(index(i))-1) dangerScore += weights[i];
	
	if (dangerScore>threshold1) return 1;
	if (dangerScore>threshold2) return 2;
	return 3;
}
*/

bool decideToStop_noRisk(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	// Stop immediately once a marker completed its column
	// Note: simulations show that this is actually counterproductive
	
	//for (int i=0; i<3; i++) if (markers[i][1]==height(markers[i][0])-1) 
	//	return true;
	
	// Count the number of markers placed
	
	int numMarkers = 0;
	for (int i=0; i<3; i++) if (markers[i][0]>=0) numMarkers++;
	
	// Stop if above or equal to max allowed value
	
	//if (numMarkers >= numMarkersPlacedToStop(columns)) return true;
	if (numMarkers >= 3) return true;
	return false;
}



////////////////////////////////////////////////////////////////////////////

// Utility function

int numMarkersThatWillBeUsed(Combination comb, int markers[3][2])
{
	// Calc number of free markers
	
	int numFreeMarkers = 0;
	for (int j=0; j<3; j++) if (markers[j][0]==-1) numFreeMarkers++;
	
	// Calc number of new markers that could be used
	// (looking at the combination and ignoring the markers availability)
	
	bool existsMarkerOnVal1 = false;
	for (int j=0; j<3; j++) if (markers[j][0]==index(comb.val1))
		existsMarkerOnVal1 = true;
	
	bool existsMarkerOnVal2 = false;
	for (int j=0; j<3; j++) if (markers[j][0]==index(comb.val2))
		existsMarkerOnVal2 = true;
	
	int numNewMarkers = 2;
	if (existsMarkerOnVal1) numNewMarkers--;
	if (existsMarkerOnVal2) numNewMarkers--;
	
	// Return how many new markers would actually be placed down
	// (considering now that some markers are not available)
	
	return (numNewMarkers<numFreeMarkers)?numNewMarkers:numFreeMarkers;
}



////////////////////////////////////////////////////////////////////////////

// Utility function

double columnProgression(int columns[11][4], int player, int icol)
{
	return double (columns[icol][player]) / height(icol);
}



////////////////////////////////////////////////////////////////////////////

// Choose combination that requires to place as few new markers as possible.
// Do not consider anything else not even to choose between combinations
// requiring the same number of new markers.

Combination selectCombination_lessMarkersOrAny(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	int ibest = 0;
	int bestNumNewMarkers = 2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers<bestNumNewMarkers)
		{
			ibest=i;
			bestNumNewMarkers = numNewMarkers;
		}
	}
	
	return combinations[ibest];
}


// Variant where we then select the column with the best progression

Combination selectCombination_lessMarkersOrBestProg(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	// Find smallest number of new markers achievable
	
	int ibest = 0;
	int bestNumNewMarkers = 2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers<bestNumNewMarkers)
		{
			ibest=i;
			bestNumNewMarkers = numNewMarkers;
		}
	}
	
	// List all combinations equally best (fewest markers)
	
	vector<Combination> combinations2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers==bestNumNewMarkers) combinations2.push_back(combinations[i]);
	}
	
	// Second selection:
	// Within the equally best, find the one that has the best progression
	
	double bestProgression = 0.0;
	Combination bestComb = combinations2[0];
	
	for (int i=0; i<combinations2.size(); i++)
	{
		double prog1 = columnProgression(columns, player, combinations2[i].val1);
		double prog2 = columnProgression(columns, player, combinations2[i].val2);
		
		if (prog1>prog2 && prog1>bestProgression)
		{
			bestProgression = prog1;
			bestComb = {combinations2[i].val1, combinations2[i].val2};
		}
		
		if (prog2>prog1 && prog2>bestProgression)
		{
			bestProgression = prog2;
			bestComb = {combinations2[i].val2, combinations2[i].val1};
		}
	}
	
	return bestComb;
}



// Variant where we then select the most extreme column (futhest from 7)

Combination selectCombination_lessMarkersOrFar7(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	// Find smallest number of new markers achievable
	
	int ibest = 0;
	int bestNumNewMarkers = 2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers<bestNumNewMarkers)
		{
			ibest=i;
			bestNumNewMarkers = numNewMarkers;
		}
	}
	
	// List all combinations equally best (fewest markers)
	
	vector<Combination> combinations2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers==bestNumNewMarkers) combinations2.push_back(combinations[i]);
	}
	
	// Second selection:
	// Within the equally best, find the one in the further from 7
	
	int bestDist = 0;
	Combination bestComb = combinations2[0];
	
	for (int i=0; i<combinations2.size(); i++)
	{
		double dist1 = abs(combinations2[i].val1-7);
		double dist2 = abs(combinations2[i].val2-7);
		
		if (dist1>dist2 && dist1>bestDist)
		{
			bestDist = dist1;
			bestComb = {combinations2[i].val1, combinations2[i].val2};
		}
		
		if (dist2>dist1 && dist2>bestDist)
		{
			bestDist = dist2;
			bestComb = {combinations2[i].val2, combinations2[i].val1};
		}
	}
	
	return bestComb;
}


// Variant where we then select the most central column (closest to 7)

Combination selectCombination_lessMarkersOrNear7(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	// Find smallest number of new markers achievable
	
	int ibest = 0;
	int bestNumNewMarkers = 2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers<bestNumNewMarkers)
		{
			ibest=i;
			bestNumNewMarkers = numNewMarkers;
		}
	}
	
	// List all combinations equally best (fewest markers)
	
	vector<Combination> combinations2;
	
	for (int i=0; i<combinations.size(); i++)
	{
		int numNewMarkers = numMarkersThatWillBeUsed(combinations[i],markers);
		if (numNewMarkers==bestNumNewMarkers) combinations2.push_back(combinations[i]);
	}
	
	// Second selection:
	// Within the equally best, find the one in the most extreme column (e.g. 2 or 12)
	
	int bestDist = 0;
	Combination bestComb = combinations2[0];
	
	for (int i=0; i<combinations2.size(); i++)
	{
		double dist1 = abs(combinations2[i].val1-7);
		double dist2 = abs(combinations2[i].val2-7);
		
		if (dist1<dist2 && dist1<bestDist)
		{
			bestDist = dist1;
			bestComb = {combinations2[i].val1, combinations2[i].val2};
		}
		
		if (dist2<dist1 && dist2<bestDist)
		{
			bestDist = dist2;
			bestComb = {combinations2[i].val2, combinations2[i].val1};
		}
	}
	
	return bestComb;
}





////////////////////////////////////////////////////////////////////////////

// Score based on progression in weighted columns

int score_1(int columns[11][4], int markers[3][2], int player)
{
	int score = 0;
	
	// Progression in weighted columns
	
	for (int i=0; i<3; i++)
	{
		int val = markers[i][0] + 2;
		int prog = markers[i][1] - columns[index(val)][player];
		
		int weight = 0;
		if (val==2 || val==12) weight = 10;
		if (val==3 || val==11) weight = 5;
		if (val==4 || val==10) weight = 3;
		if (val==5 || val==9 ) weight = 2;
		if (val==6 || val==8 ) weight = 1;
		if (val==7)            weight = 1;
		
		score += weight*prog;
	}
	
	// Reward for preserving markers
	
	int numFreeMarkers = 0;
	for (int i=0; i<3; i++) if (markers[i][0]==-1) numFreeMarkers++;
	
	score += numFreeMarkers * 10;
	
	return score;
}





////////////////////////////////////////////////////////////////////////////


bool decideToStop_score(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations)
{
	// Stop immediately once a marker completed its column
	
	for (int i=0; i<3; i++) if (markers[i][1]==height(markers[i][0])-1) 
		return true;
	
	// Otherwise act according to the score value
	
	int score = score_(columns, markers, player);
	int threshold = *(int*) stop_params;
	
	return (score>threshold)?true:false;
}



////////////////////////////////////////////////////////////////////////////


Combination selectCombination_immediateNextScore(int columns[11][4], int markers_current[3][2], int player, vector<Combination> &combinations)
{
	// For each combination, simulate placement of markers and
	// compute new score
	
	int scores_new[combinations.size()];
	
	for (int i=0; i<combinations.size(); i++)
	{
		Combination comb = combinations[i];
		
		int markers[3][2];
		for (int j=0; j<3; j++) 
		{
			markers[j][0] = markers_current[j][0];
			markers[j][1] = markers_current[j][1];
		}
		
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
		
		scores_new[i] = score_(columns, markers, player);
	}
	
	
	// Do the same for combination in reverse order (val2, val1)
	// This is not the same because the first value in the combination
	// is placed first. The second is not placed if there are no markers
	// left.
	
	int scores2_new[combinations.size()];
	
	for (int i=0; i<combinations.size(); i++)
	{
		Combination comb = {combinations[i].val2, combinations[i].val1};
		
		int markers[3][2];
		for (int j=0; j<3; j++) 
		{
			markers[j][0] = markers_current[j][0];
			markers[j][1] = markers_current[j][1];
		}
		
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
		
		scores2_new[i] = score_(columns, markers, player);
	}
	
	// Find best score across both sets of combinations
	
	int bestScore = scores_new[0];
	Combination bestComb = combinations[0];
	
	for (int i=0; i<combinations.size(); i++) if (scores_new[i]>bestScore)
	{
		bestScore = scores_new[i];
		bestComb = combinations[i];
	}
	
	for (int i=0; i<combinations.size(); i++) if (scores_new[i]>bestScore)
	{
		bestScore = scores2_new[i];
		bestComb = {combinations[i].val2, combinations[i].val1};
	}
	
	return bestComb;
}







