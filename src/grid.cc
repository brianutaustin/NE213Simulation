#include <stdio.h>
#include "grid.hh"

Grid1D::Grid1D(double umin, double umax, int nbinter)
{
	umingrid = umin;
	umaxgrid = umax;
	nbinterval = nbinter;
	meshsize = nbinterval+1;
	umesh = new double[meshsize];
	meshvalue = new double[nbinterval];

	for(int i=0;i<meshsize;i++)
	{
		umesh[i] = umingrid + i*((umaxgrid-umingrid)/nbinterval);
	}
	for(int i=0;i<nbinterval;i++)
			meshvalue[i]=0;
}

Grid1D::Grid1D(double* mesh, int nbinter)
{
	nbinterval = nbinter;
	meshsize = nbinterval+1;
	umesh = new double[meshsize];
	meshvalue = new double[nbinterval];
	umingrid = mesh[0];
	umaxgrid = mesh[meshsize-1];

	for(int i=0;i<meshsize;i++)
	{
		umesh[i] = mesh[i];
	}
	for(int i=0;i<nbinterval;i++)
			meshvalue[i]=0;
}

Grid1D::~Grid1D()
{
	delete [] umesh;
	delete [] meshvalue;
}

int Grid1D::LocateInMesh(double value)
{
	int jl,ju,jm,n;
	jl= -1;
	n = meshsize;
	ju = n;
	double *mesh = umesh;
	while ((ju-jl)>1){
		jm = (ju+jl)>>1;
		if (value>=mesh[jm]) jl = jm;
		else ju = jm;
	}
	if (value==mesh[0]) return 0;
	else if (value==mesh[n-1]) return n-2;
	if (jl==-1 || jl==n-1) return -1;
	else return jl;
}

bool Grid1D::AddValue(double uvalue, double value)
{
	int index_u = LocateInMesh(uvalue);
	if(index_u!=-1 )
	{
		meshvalue[index_u] += value;
		return true;
	}
	else return false;
}

void Grid1D::MinMax(double &umin, double &umax)
{
	umin = umingrid;
	umax = umaxgrid;
}

double Grid1D::ValueAtIndex(int i)
{
	if((i>=0) && (i<nbinterval))
		return  meshvalue[i];
	else return 0;
}

void Grid1D::Dump()
{
	for(int i=0;i<nbinterval;i++)
		fprintf(stdout,"% E  % E  % E\n",umesh[i],umesh[i+1],meshvalue[i]);
}

void Grid1D::Dump(FILE *ofile)
{
	if(!ofile) return;
	else
		for(int i=0;i<nbinterval;i++)
			fprintf(ofile,"% E  % E  % E\n",umesh[i],umesh[i+1],meshvalue[i]);
}

void Grid1D::Normalise()
{
	double norm = 0;
	for(int i=0;i<nbinterval;i++) norm += meshvalue[i] *(umesh[i+1]-umesh[i]);
	for(int i=0;i<nbinterval;i++) meshvalue[i]/=norm;
}
