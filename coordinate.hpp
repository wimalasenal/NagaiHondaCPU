/*
* The coordinate class needed to be included somewhere. 
* I am putting it here, because cells use this class, and 
* since the file will be included in the main file, we will have 
* this class available there as well.
*/

#ifndef COORDINATE
#define COORDINATE
class coordinate
{
public:
	coordinate(int idx, double X, double Y, bool t, double tx = 0.0, double ty = 0.0) : x(X), y(Y), index(idx), IsInner(t), t_x(tx), t_y(ty){};
	coordinate(double X, double Y){x = X; y = Y;};
	coordinate(){x = 100000; y = 100000; index = -1; IsInner = 0; t_x = 0.0; t_y = 0.0;};
	double x; 
	double y;
	void SetXY(double X, double Y){x = X; y = Y;};
	double t_x, t_y;
	int index;
	bool IsInner;
	inline bool operator==(const coordinate& rhs){return index == rhs.index;};
};

#endif
