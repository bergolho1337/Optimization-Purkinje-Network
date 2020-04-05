/* ===================================================================================
	MITCHELL & SHAEFFER MODEL
    A Two-Current Model for the Dynamics of Cardiac Membrane (MITCHELL and SCHAEFFER)
   ===================================================================================
*/
/*
#include <cmath>

// Nome do modelo
const char model_name[40] = "Mitchell & Shaeffer";

// Duas equacoes para resolver
const int num_equations = 2;

// Constantes relacionadas a equacao da voltagem (dV/dt)
const double t_close = 150.0;
const double t_open = 120.0;
const double t_out = 6.0;
const double t_in = 0.3;

// Estimulo inicial: Paper --> 0.1 mV
const double v_stim = 0.1;

// Mudanca de voltagem
const double v_gate = 0.13;

// Condicoes iniciais: variaveis gate
const double v0 = 0.0;
const double h0 = 1.0;

// Define o tipo Func, igual esta no 'main.cpp'
typedef
	double (*Func) (double t, double *y);

// Definicoes das funcoes do modelo
double dvdt (double t, double *y);
double dhdt (double t, double *y);

// **************************************************************************************************
//	[!] Essas funcoes devem estar implementadas em qualquer cabecalho de modelo celular !!!
// **************************************************************************************************
double* getInitialConditions ();
Func* getFunctions ();
*/
