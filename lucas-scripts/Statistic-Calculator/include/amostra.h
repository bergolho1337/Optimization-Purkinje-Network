#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

// Valor de referencia para o T-Value (0.05 - 5 % - DF = 18) --> num_samples = 10
// Checar a tabela no arquivo (DF = num_samples1 + num_samples2 - 2)
const double PVALUE = 2.101;                 

struct Amostra
{
    int n;
    vector<double> data;
    double mean;
    double std;
}typedef Amostra;

Amostra* lerAmostra (char filename[]);
double calcMean (const vector<double> data, int n);
double calcStd (const vector<double> data, double mean, int n);
double calcTTest (Amostra *a1, Amostra *a2);
double calcTTest (double mean, double std, double ref_mean, double ref_std, int num_samples);
void imprimeAmostra (Amostra *a);
void printError (const char msg[]);