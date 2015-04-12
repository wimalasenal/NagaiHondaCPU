/* This is the meat and potatoes of the code. This algorithm will 
* move each vertex and store its new location in a temporary location
* before copying the temporary array to the old array.
*/

/* The following vector holds the temporary locations of the coordinates.
* I couldnt make this code work until I made a coordinate() default constructor.
* This is an important lesson learned.
*/

#include "cell.hpp"
#include "coordinate.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <omp.h>
#include <algorithm>
#include "Get_Assoc_Cells.hpp"
#include "AreaForce.hpp"
#include "PerAdhForce.hpp"

using std::cout;
using std::endl;
using std::abs;
using std::sqrt;

int NagaiHondaForce(std::vector<coordinate>& coords,
	 std::vector<cell>& sim_cells, double step, double delta, double * X,
     double * Y, double *TX, double * TY)
{	
	/* Calculate the new vertex positions*/
	double dist = 0;
		for(std::vector<coordinate>::iterator it = coords.begin();
		it != coords.end(); ++it)
	{
		if(it->IsInner)
		{
			std::vector<int> assoc_cells = GetCells(it->index, sim_cells);
			std::vector<double> area_force = AreaForce(it->index, sim_cells, assoc_cells, coords, X, Y);
			std::vector<double> per_adh_force = PerAdhForce(it->index, sim_cells, assoc_cells, coords, X, Y);
			TX[it->index] = step * (area_force.at(0) + per_adh_force.at(0));
			TY[it->index] = step * (area_force.at(1) + per_adh_force.at(1));

			/* Verify that no vertex has moved too much */
			dist = sqrt(pow(it->t_x, 2) + pow(it->t_y, 2));
			if(dist >  0.25*delta)
				return 0;
		}

	}

	/* Update the vertex positions*/
	for(int i = 0; i < coords.size(); i++)
	{
		X[i] += TX[i]; 
		Y[i] += TY[i];
	}	

	return 1;
}

