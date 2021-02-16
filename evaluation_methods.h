
#include "cant_stop.h"

using namespace std;

struct Statistics
{
	int numGames;
	
	int max_turns[4];
	int max_lostRounds[4];
	
	double avg_wins[4];
	double avg_turns[4];
	double avg_lostRounds[4];
	double avg_lossFractions[4];
	
	double std_wins[4];
	double std_turns[4];
	double std_lostRounds[4];
	double std_lossFractions[4];
};

Statistics simulGames(int numGames=1000, bool singleplayer=false, bool silent=true);




////////////////////////////////////////////////////////////////////////////

Statistics simulGames(int numGames, bool singleplayer, bool silent)
{
	//////////////////////////////////////////
	// Simulation parameters
	
	SimulParams sparam;
	
	sparam.singleplayer = singleplayer;
	sparam.playOneTurn = false;
	sparam.silent = silent;
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
	// Statistics
	
	Statistics stats;
	stats.numGames = numGames;
	
	// Maximums
	
	for (int p=0; p<4; p++)
	{
		int max;
		
		max = 0; for (int i=0; i<numGames; i++) if (turns[i][p]>max) max=turns[i][p];
		stats.max_turns[p] = max;
		
		max = 0; for (int i=0; i<numGames; i++) if (lostRounds[i][p]>max) max=lostRounds[i][p];
		stats.max_lostRounds[p] = max;
	}
	
	// Averages
	
	for (int p=0; p<4; p++)
	{
		double sum;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += wins[i][p];
		stats.avg_wins[p] = sum/numGames;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += turns[i][p];
		stats.avg_turns[p] = sum/numGames;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += lostRounds[i][p];
		stats.avg_lostRounds[p] = sum/numGames;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += double(lostRounds[i][p])/turns[i][p];
		stats.avg_lossFractions[p] = sum/numGames;
	}
	
	// Standard deviations
	
	for (int p=0; p<4; p++)
	{
		double sum;
		
		sum = 0; for (int i=0; i<numGames; i++) sum += pow(wins[i][p]-stats.avg_wins[p],2);
		stats.std_wins[p] = sqrt(sum/(numGames-1));
		
		sum = 0; for (int i=0; i<numGames; i++) sum += pow(turns[i][p]-stats.avg_turns[p],2);
		stats.std_turns[p] = sqrt(sum/(numGames-1));
		
		sum = 0; for (int i=0; i<numGames; i++) sum += pow(lostRounds[i][p]-stats.avg_lostRounds[p],2);
		stats.std_lostRounds[p] = sqrt(sum/(numGames-1));
		
		sum = 0; for (int i=0; i<numGames; i++) sum += pow(double(lostRounds[i][p])/turns[i][p]-stats.avg_lossFractions[p],2);
		stats.std_lossFractions[p] = sqrt(sum/(numGames-1));
	}
	
	return stats;
}









