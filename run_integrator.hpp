#include <fstream>
#include <vector>
#include "cell.hpp"

using namespace std;

void run_integrator(step, vector<cell>& simulation_cells)
{	
	for(int iter = 0; iter < num_iters; iter++)
	{
		NagaiHondaForce(coordinate_list, simulation_cells, step);
		if(iter % 10 == 0)
		{
			if(iter % 100 == 0)
				cout << "		Current iteration: "  << iter << endl;
		  ostringstream ss;
		  ss << setw(5) << setfill('0') << iter;
		  string s2(ss.str());

			OffFile.open("Images/NagaiHonda"+ s2 +".off");
			OffFile << "{appearance {+edge}\n" <<"OFF" << endl;
			OffFile << coordinate_list.size() << " " << simulation_cells.size() << " 0" << endl;
			
			for(vector<coordinate>::iterator pos = coordinate_list.begin(); 
				pos != coordinate_list.end(); ++pos)
			{
				OffFile << pos->x << " " << pos->y << " " << 0.00 << endl;
			}
			for(vector<cell>::iterator vert = simulation_cells.begin(); 
				vert != simulation_cells.end(); ++vert)
			{
				vert->PrintCell(OffFile);
			}
			OffFile << "}";
			OffFile.close();	
		}
	}
}
