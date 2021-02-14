
#include "cant_stop.h"



bool decideToStop_noRisk(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);

Combination selectCombination_lessMarkersOrAny(int columns[11][4], int markers[3][2], int player, vector<Combination> &combinations);




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
	// Count the number of markers placed
	
	int numMarkers = 0;
	for (int i=0; i<3; i++) if (markers[i][0]>=0) numMarkers++;
	
	// Stop if above or equal to max allowed value
	
	//if (numMarkers >= numMarkersPlacedToStop(columns)) return true;
	if (numMarkers >= 3) return true;
	return false;
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
		bool existsMarkerOnVal1 = false;
		for (int j=0; j<3; j++) if (markers[j][0]==index(combinations[i].val1))
			existsMarkerOnVal1 = true;
		
		bool existsMarkerOnVal2 = false;
		for (int j=0; j<3; j++) if (markers[j][0]==index(combinations[i].val2))
			existsMarkerOnVal2 = true;
		
		int numNewMarkers = 2;
		if (existsMarkerOnVal1) numNewMarkers--;
		if (existsMarkerOnVal2) numNewMarkers--;
		
		if (numNewMarkers<bestNumNewMarkers) ibest=i;
	}
	
	return combinations[ibest];
}





