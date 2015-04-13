/*
 * This is the file where the action happens.
 * 
 * I am following the google style guide for naming non-class variables 
 * for naming functions. See:
 * http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#Function_Names
 */
#include <sys/time.h>
#include <sstream> 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <stdio.h>
#include "coordinate.hpp"
#include "cell.hpp"
#include "NagaiHondaForce.hpp"
#include "mesh.hpp"
#include "printCellProperties.hpp"
#include <stdlib.h>
#include "EnergyCalculator.hpp"
#include "Topology.hpp"
#include <string.h>

int iter_notification = 100;

using namespace std;

void ReadConfig(int&, int&, int&, double&, int&, double&, int&, int&, double&, int&, int&);
void ReadMeshChanges(vector<cell>&);


// This is for timing the code.
double clkbegin, clkend;
double t;
double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}


int main()
{	
	/*********************** SOME VARIABLES ***************************/
	map<int, int> histogram;
	histogram.insert(pair<int, int>(3, 0));	
	histogram.insert(pair<int, int>(4, 0));
	histogram.insert(pair<int, int>(5, 0));
	histogram.insert(pair<int, int>(6, 0));
	histogram.insert(pair<int, int>(7, 0));
	histogram.insert(pair<int, int>(8, 0));
	histogram.insert(pair<int, int>(9, 0));
	histogram.insert(pair<int, int>(10, 0));
	histogram.insert(pair<int, int>(11, 0));
	map<int, int>::iterator hist_it;
	int s; 
	
	/***************** READ CONFIGURATION FILE ************************/
	int n, print_e, make_movie, num_perturb, num_iters, out_freq;
	int max_swaps, swap_odds, make_graph;
	double step, energy, delta, dt, swap_len; 
	double time = 0;
	ReadConfig(n, print_e, make_movie, step, num_iters, delta, out_freq, \
			   max_swaps, swap_len, swap_odds, make_graph);
	
    /**************** CONSTRUCT THE X AND Y ARRAYS ********************/
    double X[9*n*n];
    double Y[9*n*n];
    double TX[9*n*n];
    double TY[9*n*n];
    memset(X, 0, sizeof(X)); // Make the coordinate arrays.
    memset(Y, 0, sizeof(Y)); // Using old school memset!
    memset(X, 0, sizeof(TX));
    memset(X, 0, sizeof(TY));
    
	/************************ MAKE THE MESH ***************************/
	vector<coordinate> coords;
	vector<cell> sim_cells;
	hex_mesh(coords, sim_cells, n, X, Y);
    Random_T1s(sim_cells, coords, max_swaps, swap_len, swap_odds, X, Y);
	perturb_mesh(coords, sim_cells, n, num_perturb, X, Y);
    ReadMeshChanges(sim_cells);
	
	for(auto c : sim_cells) 
    {
		s = c.GetNumSides();
		if ( histogram.find(s) != histogram.end())
		{
			hist_it = histogram.find(s);
			hist_it->second++;
		}
		else
		{
			histogram.insert(pair<int, int>(s, 0));
			hist_it = histogram.find(s);
			hist_it->second++;
		}
	}    
	ofstream histFile;
	histFile.open("hist_data0.txt");
	for(hist_it=histogram.begin(); hist_it!= histogram.end(); hist_it++)
		histFile << hist_it->first << " " << hist_it->second << endl;
	histFile.close();


	//for(auto c : coords)
	//cout << c.index << " " << c.IsInner << endl;
	
    /************************* WELCOME ********************************/
	cout << "\n\n****************************************************\n";
	cout << "*         Nagai-Honda Model Simulation             *\n";
	cout << "****************************************************\n\n";
	cout << "The number of cells in this simulation is " << sim_cells.size() << endl;
	cout << "The number of coordinates in this simulation is " << coords.size() << endl;
	cout << "Starting the integration... \n";
	ofstream OffFile;
	ofstream PE; 
	PE.open("energy.dat");
	
	/************************** MAIN LOOP *****************************/
	ofstream vertices;
	vertices.open("vertices.txt");
    int proceed;
	clkbegin = rtclock();
	for(int iter = 0; iter < num_iters; iter++)
	{
	  vertices << "************************************************************" <<endl;
	  for(int i = 0; i < coords.size(); i++)
		vertices << i << " " << X[i] << " " << Y[i] << endl;
	  if(iter % iter_notification == 0)
        cout << "		Current iteration: "  << iter << endl;
        
		/******************* PRINT OUT AN OFF FILE ********************/
		// This should be a routine
		if(iter % out_freq == 0)
		{
		  ostringstream ss;
		  ss << setw(5) << setfill('0') << iter;
		  string s2(ss.str());
			
			OffFile.open("Images/NagaiHonda"+ s2 +".off");
			OffFile << "{appearance {+edge}\n" <<"OFF" << endl;
			OffFile << coords.size() << " " << sim_cells.size() << " 0" << endl;
			
			for(vector<coordinate>::iterator pos = coords.begin(); 
				pos != coords.end(); ++pos)
			{
				OffFile << X[pos->index] << " " << Y[pos->index] << " " << 0.00 << endl;
			}
			for(vector<cell>::iterator vert = sim_cells.begin(); 
				vert != sim_cells.end(); ++vert)
			{
				vert->PrintCell(OffFile);
			}
			OffFile << "}";
			OffFile.close();	

		}
		
		/******************* MOVE THE VERTICES ************************/
		dt = step;
		proceed = 0;
		while (proceed == 0)
		{
			proceed = NagaiHondaForce(coords, sim_cells, dt, delta, X, Y, TX, TY);
			dt = dt/2;
		}
		time += dt;
        
		/**************** PERFORM TOPOLOGICAL CHANGES *****************/
		Perform_T2s(sim_cells, coords, delta, X, Y);
		Perform_T1s(sim_cells, coords, delta, X, Y);
		Perform_T1s(sim_cells, coords, delta, X, Y);
		/*************** CHECK FOR GRAPHING ERRORS ********************/
		for(auto c : sim_cells)
		{
		  vector<int> points = c.GetVertices();
		  std::sort(points.begin(), points.end());
		  for(int i = 1; i < points.size(); i++)
			{
			  if ((points.at(i) - points.at(i-1)) == 0)
				cout << "cell contains duplicate vertices." << endl;
			}
		}
		
        /**************** CALCULATE ENERGY IN THE MESH ****************/
		energy = Energy(sim_cells, coords, X, Y);
		PE << time << " " << energy << endl;
		
	}
	clkend = rtclock();
    t = clkend-clkbegin;
	cout << "The computation finished in " << t << " seconds!" << endl;
	
	/*********************** MAKE THE MOVIE ***************************/
	if(make_movie == 1)
	{
		cout << "Making animation..." << endl;
		system("bash Images/make_movie.sh");
	}
		
	
	/******************* MAKE THE PLOTS *************************/
	histogram.clear();
	histogram.insert(pair<int, int>(3, 0));	
	histogram.insert(pair<int, int>(4, 0));
	histogram.insert(pair<int, int>(5, 0));
	histogram.insert(pair<int, int>(6, 0));
	histogram.insert(pair<int, int>(7, 0));
	histogram.insert(pair<int, int>(8, 0));
	histogram.insert(pair<int, int>(9, 0));
	histogram.insert(pair<int, int>(10, 0));
	histogram.insert(pair<int, int>(11, 0));
	for(auto c : sim_cells) 
    {
		s = c.GetNumSides();
		if ( histogram.find(s) != histogram.end())
		{
			hist_it = histogram.find(s);
			hist_it->second++;
		}
		else
		{
			histogram.insert(pair<int, int>(s, 0));
			hist_it = histogram.find(s);
			hist_it->second++;
		}
	}
	histFile.open("hist_data1.txt");
	for(hist_it=histogram.begin(); hist_it!= histogram.end(); hist_it++)
		histFile << hist_it->first << " " << hist_it->second << endl;
	histFile.close();
	
	    ofstream areaFile;
	    areaFile.open("area.txt");
	    ofstream perimFile;
	    perimFile.open("perim.txt");

	    for (auto c: sim_cells)
	    {
		areaFile << c.ComputeArea(X, Y) << endl;
		perimFile << c.ComputePerimeter(X, Y) << endl;
	    }
	    areaFile.close();
if(print_e == 1)
		{
		  cout << "Plotting ..." << endl;
		  system("python plot.py");
		}
	
	


	/****************************  DONE  ******************************/
	cout << "\n*************Simulation Complete********************\n\n";

}

void ReadConfig(int  &n, int &print_E, int &make_movie, double &step, int &num_iters, double &delta, int &out_freq, int &max_swaps, double &swap_len, int &swap_odds, int& make_graph)
{
	string junk; // a variable to store the junk we dont want from the conf file.
	ifstream conf("config.txt");
	conf >> n; // get the info we want
	getline(conf, junk); // throw away the rest of the line.
	conf >> print_E;
	getline(conf, junk);
	conf >> make_movie;
	getline(conf, junk);
	conf >> step;
	getline(conf, junk);
	conf >> num_iters;
	getline(conf, junk);
	conf >> out_freq;
	getline(conf, junk);
	conf >> delta;
	getline(conf, junk);
    conf >> max_swaps;
	getline(conf, junk);
    conf >> swap_len;
	getline(conf, junk);
    conf >> swap_odds;
	getline(conf, junk);
	conf >> make_graph;
	getline(conf, junk);
	cout << "Dimension of mesh: " << n << endl;
	cout << "Number of iterations " << num_iters << endl;
	cout << "Maximum step size for the integration " << step << endl;
	cout << "Delta for T1 swap " << delta << endl;
	cout << "An image will be printed every " << out_freq << " iterations." << endl;
    cout << "The maximum number of initial T1 swaps is: " << max_swaps << endl;
}

void ReadMeshChanges(vector<cell>& sim_cells)
{
    string comment;
    int index;
    double parameter;
    int num_gamma = 0, num_area;
    ifstream changes("change_mesh.txt");
    getline(changes, comment);
    changes >> num_gamma >> num_area;
    for(int line = 0; line < num_gamma; line++)
    {
        changes >> index >> parameter;
        sim_cells[index].SetGamma(parameter);
    }
    for(int line = 0; line < num_area; line++)
    {
        changes >> index >> parameter;
        sim_cells[index].SetTargetArea(parameter);
    }
}
