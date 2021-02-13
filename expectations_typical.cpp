
#include "expectations.h"

using namespace std;



////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
	//////////////////////////////
	// Options
	
	bool already_marked  = false;
	int  columns_to_fill = 0; // starting from center
	
	if (argc>1) if (string(argv[1])=="true") already_marked  = true;
	if (argc>2) columns_to_fill = stoi(argv[2]);
	
	cout << "Options:" << endl;
	cout << "  already_marked  = " << already_marked  << endl;
	cout << "  columns_to_fill = " << columns_to_fill << endl;
	cout << endl;
	
	
	////////////////////////////////
	// Simulations and analysis
	
	double progression_avg[11];
	
	for (int target=2; target<=12; target++)
	{
		// Initial simulation state (fill columns and position markers)
		
		SimulState sstate_in; initialiseDefaultState(sstate_in);
		
		int fill_order[11] = {7,6,8,5,9,4,10,3,11,2,12};
		int columns_filled = 0;
		
		for (int i=0; i<11 && columns_filled<columns_to_fill; i++)
		{
			int n = fill_order[i];
			if (n!=target)
			{
				sstate_in.columns[index(n)][3] = height(index(n))-1;
				columns_filled++;
			}
		}
		
		if (already_marked)
		{
			sstate_in.markers[0][0] = index(target);
			sstate_in.markers[0][1] = 0;
		}
		
		// Compute expectation
		
		progression_avg[index(target)] = expectation(target, sstate_in.columns, sstate_in.markers, 10000);
		if (already_marked) progression_avg[index(target)] -= 1.0;
	}
	
	
	///////////////////////////
	// Print results
	
	for (int target=2; target<=12; target++)
	{
		cout << setw(2) << target << " : ";
		cout << setw(8) << progression_avg[index(target)] << " : ";
		cout << setw(8) << progression_avg[index(7)]/progression_avg[index(target)];
		cout << endl;
	}
	
	
	return 0;
}



