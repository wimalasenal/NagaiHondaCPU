#include <vector>
#include <iostream>

using namespace std;

int main()
{
vector<int> verts{1, 2, 3, 4};

for(int i = 0; i < verts.size(); i++)                // For each cell go through all of the vertices
{
            int Im1 = verts.at((i-1)%(verts.size()));                                     // These are the indices 
            int I = verts.at(i);
            int Ip1 = verts.at((i+1)%(verts.size()));
            int Ip2 = verts.at((i+2)%(verts.size()));
			cout << Im1 << " "
			     << I   << " "
			     << Ip1 << " "
			     << Ip2 << endl;
}
}
