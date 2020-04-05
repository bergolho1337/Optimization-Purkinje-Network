#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

const double PVALUE = 1.960;

// Valores de referencia da arvore histologica (Artigo -- Tabela 2)
const int ref_num_edge = 3798;
//const double ref_mean_edge = 0.36821;
//const double ref_std_edge = 0.13989;
const double ref_mean_edge = 1.035905;
const double ref_std_edge = 2.16845;

const int ref_num_seg = 1349;
const double ref_mean_seg = 1.035905;
const double ref_std_seg = 2.16845;

const int ref_num_ang = 674;
const double ref_mean_ang = 63.36497;
const double ref_std_ang = 26.826115;

// Valores de referencia da arvore do L-System
/*
const int ref_num_edge = 9194;
const double ref_mean_edge = 1.8271476446; 
const double ref_std_edge = 0.5009996144;

const int ref_num_seg = 2888;
const double ref_mean_seg = 5.8167574253;  
const double ref_std_seg = 5.5356374762;

const int ref_num_ang = 2888;
const double ref_mean_ang = 55.2519637786;
const double ref_std_ang = 28.1979470426;
*/

int CalculateMeanStd (const char filename[], double &mean, double &std)
{
    printf("%s\n",filename);
    double value;
    vector<double> values;
    FILE *in = fopen(filename,"r");
    mean = 0;
    while (fscanf(in,"%lf",&value) != EOF)
    {
        mean += value;
        values.push_back(value);
    }    
    mean /= (double)values.size();
    std = 0;
    for (int i = 0; i < (int)values.size(); i++)
        std += pow(values[i] - mean,2);
    std = sqrt(std / (double)values.size());
    fclose(in);

    return (int)values.size();
}

void CalculateTtest_Edge (double mean_edge, double std_edge, int num_edge, double &ttest_edge)
{
    double num = fabs(ref_mean_edge - mean_edge);
    double den = sqrt((pow(ref_std_edge,2) / ref_num_edge) + (pow(std_edge,2) / num_edge));
    ttest_edge = num / den;
}

void CalculateTtest_Ang (double mean_ang, double std_ang, int num_ang, double &ttest_ang)
{
    double num = fabs(ref_mean_ang - mean_ang);
    double den = sqrt((pow(ref_std_ang,2) / ref_num_ang) + (pow(std_ang,2) / num_ang));
    ttest_ang = num / den;
}

void CalculateTtest_Seg (double mean_seg, double std_seg, int num_seg, double &ttest_seg)
{
    double num = fabs(ref_mean_seg - mean_seg);
    double den = sqrt((pow(ref_std_seg,2) / ref_num_seg) + (pow(std_seg,2) / num_seg));
    ttest_seg = num / den;
}

int main (int argc, char *argv[])
{
    int num_edge, num_ang, num_seg;
    double mean_edge, std_edge;
    double mean_ang, std_ang;
    double mean_seg, std_seg;
    double ttest_edge, ttest_ang, ttest_seg;
    num_edge = CalculateMeanStd(argv[1],mean_edge,std_edge);
    num_ang = CalculateMeanStd(argv[2],mean_ang,std_ang);
    num_seg = CalculateMeanStd(argv[3],mean_seg,std_seg);
    

    CalculateTtest_Edge(mean_edge,std_edge,num_edge,ttest_edge);
    CalculateTtest_Ang(mean_ang,std_ang,num_ang,ttest_ang);
    CalculateTtest_Seg(mean_seg,std_seg,num_seg,ttest_seg);

    printf("====================== EDGES ==============================\n");
    printf("Mean +/- Std = %.10lf +- %.10lf\n",mean_edge,std_edge);
    printf("Number of edges = %d\n",num_edge);
    printf("Ttest = %.10lf\n",ttest_edge);
    (ttest_edge <= PVALUE) ? printf("NULL Hypothesis\n") : printf("ALTERNATIVE Hypothesis\n");
    printf("===========================================================\n");
    printf("====================== ANGLE ==============================\n");
    printf("Mean +/- Std = %.10lf +- %.10lf\n",mean_ang,std_ang);
    printf("Number of bifurcations = %d\n",num_ang);
    printf("Ttest = %.10lf\n",ttest_ang);
    (ttest_ang <= PVALUE) ? printf("NULL Hypothesis\n") : printf("ALTERNATIVE Hypothesis\n");
    printf("===========================================================\n");
    printf("====================== SEGMENT ============================\n");
    printf("Mean +/- Std = %.10lf +- %.10lf\n",mean_seg,std_seg);
    printf("Number of segments = %d\n",num_seg);
    printf("Ttest = %.10lf\n",ttest_seg);
    (ttest_seg <= PVALUE) ? printf("NULL Hypothesis\n") : printf("ALTERNATIVE Hypothesis\n");
    printf("===========================================================\n");
    return 0;
}