#include "Method.h"
#include "Data.h"
#include "Output.h"

#include <sys/time.h>
#include <math.h>
#include <cstring>
#include <ctime>
#include <stdlib.h>

char instance[100];
int debug = 1; 
int numDecoders = 1;
int numLS;
int MAXTIME = 30;
int MAXRUNS = 1;
unsigned MAX_THREADS = 1;
float OPTIMAL = 0;

int n;

struct timeval Tstart, Tend, Tbest;
char nameTable[256]; 
char nameMH[256];
TSol bestSolution;


int main(int argc, char *argv[]) {
    char nameScenario[256]; // File that contain files names to be tested in a given scenario
    strncpy(nameScenario, argv[1], 255);
    int method = atoi(argv[2]);

    FILE *arq;
    arq = fopen(nameScenario, "r");

    if(arq == NULL){
        printf("\nERROR: File %s not found\n", nameScenario);
        getchar();
        exit(1);
    }

    if(fgets(nameTable, sizeof(nameTable), arq) == NULL) {
        printf("\nERROR: File %s not found\n", nameTable);
        getchar();
        exit(1);
    }

    TSol sBest;
    sBest.flag = 0;
    sBest.label = 0;
    sBest.objFValue = 0;
    sBest.similar = 0;
    sBest.promising = 0;
    sBest.vector.clear();

    while(!feof(arq)) {
        if (fscanf(arq, "%30[^ ,\n\t] %u[^ ,\n\t] %u[^ ,\n\t] %u[^ ,\n\t] %u[^ ,\n\t] %u[^ ,\n\t] %u[^ ,\n\t]",
                        nameTable, &debug, &numDecoders, &numLS, &MAXTIME, &MAXRUNS, &MAX_THREADS) == 0) {
            exit(1);
        }

        strcpy(instance, nameTable);

        double foBest = INFINITY, foAverage = 0;
        float timeBest = 0, timeTotal = 0;

        std::vector<double> ofvs;
        ofvs.clear();
        sBest.objFValue = INFINITY;

        printf("\n\nInstance: %s \nRun: ", instance);
        for(int j=0; j<MAXRUNS; j++) {
            if(debug == 1) srand(j+1);
            else srand(time(NULL));

            printf("%d ", j+1);

            gettimeofday(&Tstart, NULL);
            gettimeofday(&Tend, NULL);
            gettimeofday(&Tbest, NULL);

            bestSolution.objFValue = INFINITY;

            switch (method) {
                case 1:
                    strcpy(nameMH, "SA");
                    SA();
                break;
            
                default:
                break;
            }

            gettimeofday(&Tend, NULL);

            if (bestSolution.objFValue < sBest.objFValue)
                sBest = bestSolution;

            if(bestSolution.objFValue < foBest)
                foBest += bestSolution.objFValue;

            ofvs.push_back(bestSolution.objFValue);

            timeBest += ((Tbest.tv_sec  - Tstart.tv_sec) * 1000000u + Tbest.tv_usec - Tstart.tv_usec) / 1.e6;
            timeTotal += ((Tend.tv_sec  - Tstart.tv_sec) * 1000000u + Tend.tv_usec - Tstart.tv_usec) / 1.e6;             

        }

        foAverage = foAverage / MAXRUNS;
        timeBest = timeBest / MAXRUNS;
        timeTotal = timeTotal / MAXRUNS;

        WriteSolution(nameMH, sBest, n, instance, timeBest, timeTotal);
    }

    fclose(arq);
    return 0;
}