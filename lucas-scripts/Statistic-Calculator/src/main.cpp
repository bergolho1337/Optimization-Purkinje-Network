#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "../include/amostra.h"

using namespace std;

int main (int argc, char *argv[])
{

    if (argc-1 < 2)
    {
        printf("=========================================================\n");
        printf("Usage:> %s <input_file1> <input_file2>\n",argv[0]);
        printf("Exemplo:> %s AT-CO-delta0.dat AT-CO-delta1.0.dat\n",argv[0]);
        printf("=========================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        Amostra *a1 = lerAmostra(argv[1]);
        Amostra *a2 = lerAmostra(argv[2]);
        double ttest = calcTTest(a1,a2);
        printf("\nResultado do T-Test = %lf\n",ttest);
        if (ttest > PVALUE)
            printf("There is a significant difference between the two groups;\nthe observed differences are most likely not due to chance or sampling error.\n");
        else
            printf("There is NOT a significant difference between the two groups;\nany observed differences may be due to chance and sampling error.\n");
        imprimeAmostra(a1);
        imprimeAmostra(a2);

        /*
        FILE *in = fopen(argv[1],"r");
        double mean_ref, std_ref;
        double mean_test, std_test;
        double ttest;
        if (!fscanf(in,"%lf %lf",&mean_ref,&std_ref)) printError("Reading file");
        for (int i = 0; i < 8; i++)
        {
            if (!fscanf(in,"%lf %lf",&mean_test,&std_test)) printError("Reading file");
            ttest = calcTTest(mean_test,std_test,mean_ref,std_ref,10);
            printf("\nResultado do T-Test = %lf\n",ttest);
            if (ttest > PVALUE)
                printf("There is a significant difference between the two groups;\nthe observed differences are most likely not due to chance or sampling error.\n");
            else
                printf("There is NOT a significant difference between the two groups;\nany observed differences may be due to chance and sampling error.\n");
        
        }
        fclose(in);
        */
        return 0;
    }
}
