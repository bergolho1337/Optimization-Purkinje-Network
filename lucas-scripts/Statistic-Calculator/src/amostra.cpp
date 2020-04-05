#include "../include/amostra.h"

Amostra* lerAmostra (char filename[])
{
    Amostra *amostra = new Amostra();
    ifstream in(filename);

    double value;
    while (in >> value)
        amostra->data.push_back(value);
    amostra->n = amostra->data.size();
    
    // Calcular media e desvio padrao da amostra
    amostra->mean = calcMean(amostra->data,amostra->n);
    amostra->std = calcStd(amostra->data,amostra->mean,amostra->n);

    return amostra;
}

// Calcular a media de um vetor
double calcMean (const vector<double> data, int n)
{
    double mean = 0;
    for (int i = 0; i < n; i++)
        mean += data[i];
    mean /= (double)n;
    return mean;
}

// Calcular o desvio padrao de um vetor
double calcStd (const vector<double> data, double mean, int n)
{
    double dev = 0;
    for (int i = 0; i < n; i++)
        dev += pow(data[i] - mean,2);
    dev = sqrt(dev / (double)n);
    return dev;
}

// Calcular o Teste T de Student --> [ http://www.biologyforlife.com/t-test.html ]
// Usando a struct Amostra
double calcTTest (Amostra *a1, Amostra *a2)
{
    double num = fabs(a2->mean - a1->mean);
    double den = sqrt((pow(a1->std,2) / a1->n) + (pow(a2->std,2) / a2->n));
    double t = num / den;
    return t;
}

// Calcular o Teste T de Student --> [ http://www.biologyforlife.com/t-test.html ]
// Caso jah se possua os valores da media e desvio padrao
double calcTTest (double mean, double std, double ref_mean, double ref_std, int num_samples)
{
    double num = fabs(mean - ref_mean);
    double den = sqrt((pow(std,2) / num_samples) + (pow(ref_std,2) / num_samples));
    double t = num / den;
    return t;
}

// Imprimir informacoes sobre a amostra
void imprimeAmostra (Amostra *a)
{
    printf("=================== AMOSTRA ========================\n");
    printf("Numero de amostras = %d\n",a->n);
    printf("Media = %.5lf\n",a->mean);
    printf("Desvio Padrao = %.5lf\n",a->std);
    printf("=======================================================\n");
}

// Imprimir mensagem de erro
void printError (const char msg[])
{
    printf("[-] ERROR ! %s\n",msg);
    exit(EXIT_FAILURE);
}

