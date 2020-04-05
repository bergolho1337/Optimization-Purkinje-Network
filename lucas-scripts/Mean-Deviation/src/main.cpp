#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

void error (char *msg)
{
    printf("[-] ERROR! %s !\n",msg);
    exit(-1);
}

// Calcular a media de um vetor
double calculateMean (double v[], int N)
{
    double mean = 0;
    for (int i = 0; i < N; i++)
        mean += v[i];
    mean /= (double)N;
    return mean;
}

// Calcular o desvio padrao de um vetor
double calculateStandartDeviation (double v[], double mean, int N)
{
    double dev = 0;
    for (int i = 0; i < N; i++)
        dev += pow(v[i] - mean,2);
    dev = sqrt(dev / (double)N);
    return dev;
}

int main (int argc, char *argv[])
{
    int N;
    FILE *file = fopen("in4.dat","r");
    if (!fscanf(file,"%d",&N)) error("Reading file");
    double min[N], max[N];
    for (int i = 0; i < N; i++)
        if (!fscanf(file,"%lf %lf",&min[i],&max[i])) error("Reading file");
    double mean = calculateMean(min,N);
    double dev = calculateStandartDeviation(min,mean,N);
    printf("=================== MIN vector ========================\n");
    printf("Mean = %.5lf\n",mean);
    printf("Standart deviation = %.5lf\n",dev);
    printf("=======================================================\n");
    
    mean = calculateMean(max,N);
    dev = calculateStandartDeviation(max,mean,N);
    printf("=================== MAX vector ========================\n");
    printf("Mean = %.5lf\n",mean);
    printf("Standart deviation = %.5lf\n",dev);
    printf("=======================================================\n");

    fclose(file);
}
