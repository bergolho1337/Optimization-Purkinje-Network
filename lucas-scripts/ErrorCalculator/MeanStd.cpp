#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

int CalculateMeanStd (const char filename[], double &mean, double &std)
{
    //printf("%s\n",filename);
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

int main (int argc, char *argv[])
{
    int num_pts;
    double mean_error, std_error;
    num_pts = CalculateMeanStd(argv[1],mean_error,std_error);
    
    printf("================ GLOBAL ERROR =============================\n");
    printf("Number of points = %d\n",num_pts);
    printf("Mean +/- Std = %.10lf +- %.10lf\n",mean_error,std_error);
    printf("===========================================================\n");
    return 0;
}