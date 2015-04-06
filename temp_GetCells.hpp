#include <iostream>
#include <vector>
#include "cell.hpp"
#include "coordinate.hpp"

using std::cout;
using std::endl;

std::vector<int> Get_Associated_Cells(int cell_id, std::vector<cell> simulation_cells)
{		  
    
cout << "1 " << endl;    
		std::vector<int> associated_cells; 
		for(std::vector<cell>::iterator it2 = simulation_cells.begin();
			it2 != simulation_cells.end(); ++it2)
		{			
			// Collect all of the cells which include this vertex.
			if(it2->ContainsVertex(cell_id))
			{
				associated_cells.push_back(it2->GetIndex());
			}
		}
        
cout << "2 " << endl;    
			
		std::vector<int> verts = simulation_cells[associated_cells.at(0)].GetVertices();
		// find the position of the vertex cell_id in this vector.
		
		std::vector <int>::iterator i = find (verts.begin(), verts.end(), cell_id);
		
		int position = -1000; // set this to an arbitrary value
		
		if (i != verts.end())
		{
			position = distance(verts.begin(), i);
			// Get the position of the vertex in the vertices of the current cell.
		}

cout << "3 " << endl;    

		int next_vertex = verts.at((position + 1) % verts.size());
		
		// Okay, so  now for the first associated cell we know which vertex comes next counter clockwise.
		// We just need to find another cell with this vertex in it. 
		
		std::vector<int> remaining_cells = associated_cells;
		
		unsigned int current_cell = 1; // this is the index of the cell we are looking to put into the associated cells array.
		
		remaining_cells.erase(remaining_cells.begin()); // now we will not check the first one again.

cout << "4 " << endl;    
cout << "remaining cells . size() " << remaining_cells.size() << endl;
		while (remaining_cells.size() > 1)
		{
			unsigned int idx = 0;
			while(idx < remaining_cells.size())
			{
				cell check_for_vert = simulation_cells.at(remaining_cells.at(idx));
				if(check_for_vert.ContainsVertex(next_vertex))
				{
					// Then we know that this cell is clockwise from the previous one.
					std::vector<int> vertices = check_for_vert.GetVertices();
					std::vector<int>::iterator i2 = find (vertices.begin(), vertices.end(), cell_id);
					// Find our principal vertex in the cell.
					
					int location = -1000; // set this to an arbitrary value
					
					if (i2 != vertices.end())
					{
					  location = distance(vertices.begin(), i2);
					  // find the position of the principal vertex in this clockwise cell.
					}
					
					next_vertex = vertices.at((location + 1) % vertices.size());
					// find the location of the new next vertex
					associated_cells.at(current_cell) = remaining_cells.at(idx);
					remaining_cells.erase(remaining_cells.begin() + idx);
					//erase the cell we just put into the associated cells vector. 
					current_cell++;
					break;//leave the while loop
				}							
				idx++;
			}
		}

cout << "5 " << endl;    
        
		if(remaining_cells.size() == 1)
		{
		  associated_cells.at(current_cell) = remaining_cells.at(0);
		}		
		std::reverse(associated_cells.begin(), associated_cells.end());	
		return associated_cells;
}
