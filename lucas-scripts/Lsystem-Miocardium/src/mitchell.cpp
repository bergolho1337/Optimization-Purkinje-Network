#include "../include/mitchell.h"

/*
double dvdt (double t, double *y)
{
    return ( ((y[1]*pow(y[0],2)*(1-y[0]))/t_in) - (y[0]/t_out) );
}

double dhdt (double t, double *y)
{
	if (y[0] < v_gate)
		return ( (1-y[1])/t_open );
	else
		return ( -y[1]/t_close );
}

double* getInitialConditions ()
{
	double *y = new double[num_equations];
	y[0] = v0;
	y[1] = h0;
	return (y);
}

Func* getFunctions ()
{
	Func *f = new Func[num_equations];
	f[0] = dvdt;
	f[1] = dhdt;
	return (f);
}
*/