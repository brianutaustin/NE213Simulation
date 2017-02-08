#ifndef _MYGRID_H_
#define _MYGRID_H_

#include <stdio.h>

class Grid1D
{
protected:
	double	umingrid,umaxgrid;
	int		meshsize,nbinterval;
	double	*umesh;
	double	*meshvalue;
private:
public:
	Grid1D(double umin, double umax, int nbinter);
	Grid1D(double* mesh, int nbinter);
	virtual ~Grid1D();
	int LocateInMesh(double value);
	bool AddValue(double uvalue, double value);
	void MinMax(double &umin, double &umax);
	int NbInterval() {return nbinterval;}
	double ValueAtIndex(int i);
	void Normalise();
	void Dump();
	void Dump(FILE* ofile);
};

#endif
