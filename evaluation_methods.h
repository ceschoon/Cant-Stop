
#include "cant_stop.h"

using namespace std;

struct Statistics
{
	double avg_wins[4];
	double avg_turns[4];
	double avg_lostRounds[4];
};

Statistics simulGames(int numGames=10000, bool singleplayer=false);




////////////////////////////////////////////////////////////////////////////

Statistics simulGames(int numGames, bool singleplayer)
{
	//////////////////////////////////////////
	// Simulation parameters
	
	SimulParams sparam;
	
	sparam.singleplayer = singleplayer;
	sparam.playOneTurn = false;
	sparam.silent = true;
	sparam.seed = 0; // will be overwritten
	
	
	////////////////////////////////////////////////
	// Rng for choosing seeds of each simulation
	
	random_device true_gen;
	int seed = -1; while (seed<0) seed = true_gen();
	
	uniform_int_distribution<int> dist(0,100000000);
	default_random_engine gen(seed);
	
	
	///////////////////////////////////////////////////////////////////
	// Runs games and record statistics
	
	int wins[numGames][4];
	int turns[numGames][4];
	int lostRounds[numGames][4];
	
	for (int i=0; i<numGames; i++)
	{
		sparam.seed = dist(gen);
		SimulState sstate_out; initialiseDefaultState(sstate_out);
		simulGame(sparam, sstate_out);
		
		for (int p=0; p<4; p++)
		{
			if (sstate_out.winner==p) wins[i][p]=1; else wins[i][p]=0;
			turns[i][p] = sstate_out.turns[p];
			lostRounds[i][p] = sstate_out.lostRounds[p];
		}
	}
	
	
	////////////////////////////////////////////////////////
	// Averaged statistics
	
	Statistics stats;
	
	for (int p=0; p<4; p++)
	{
		double sum;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += wins[i][p];
		stats.avg_wins[p] = sum/numGames;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += turns[i][p];
		stats.avg_turns[p] = sum/numGames;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += lostRounds[i][p];
		stats.avg_lostRounds[p] = sum/numGames;
	}
	
	return stats;
}









