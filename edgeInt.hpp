/* This function will determine whether or not an edge is interior.
 * 
 * Since every edge will be counted twice, the energy we eventually 
 * calculate may need to be divided by two.
 * 
 * Go through all of the cells in the mesh. For each cell, consider each vertex and its counterclockwise neighbor. 
 * If either one is inteior, we will include the edge in our energy calculation. 
 * 
 * Note again that each edge will be counted twice.
 */

#include  <vector>

#include "cell.hpp"
#include "coordinate.hpp"

using namespace std;

bool edgeInt(cell C, int idx)
{
	bool is_int = 0;
	vector<int> assoc_verts = C.GetVertices();
	coordinate curr = assoc_verts.at(idx);
	coordinate next = assoc_verts.at((idx+1)%(assoc_verts.size()));
	if((curr.IsInner)||(next.IsInner))
		is_int = 1;
	return is_int;
}
