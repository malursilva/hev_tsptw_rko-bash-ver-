#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Data.h"

void WriteSolutionScreen(char mh[], TSol s, int n, float timeBest, float timeTotal, char instance[]) {
    printf("\n\n\nMetaheuristic: %s \nInstance: %s \nCustomers: ", mh, instance);
	for (int i=0; i<n/2; i++)
		printf("%d ", s.vector[i].value);
    printf("\nModes: ");
    for (int i=n/2; i<n; i++)
		printf("%d ", s.vector[i].value);

	printf("\nfo: %.2f", s.objFValue); 
	printf("\nTotal time: %.3f",timeTotal);
	printf("\nBest time: %.3f\n\n",timeBest);
}

void WriteSolution(char mh[], TSol s, int n, float timeBest, float timeTotal, char instance[]) {
	FILE *arq;
    arq = fopen("./Results/Solutions.txt","a");

	if (!arq) {
		printf("\n\nFile not found Solutions.txt!!!");
		getchar();
		exit(1);
	}

    fprintf(arq,"\n\nInstance: %s", instance);
	fprintf(arq,"\nMethod: %s",mh);
	fprintf(arq,"\nSol: ");
	for (int i=0; i<n; i++)
		fprintf(arq,"%d ", s.vector[i].value);
	fprintf(arq,"\nfo: %.2f", s.objFValue);
  	fprintf(arq,"\nBest time: %.3f",timeBest);
	fprintf(arq,"\nTotal time:%.3f \n",timeTotal);

	fclose(arq);
}

void WriteResults(char mh[], double ofv, double ofvAverage, std::vector <double> ofvs, float timeBest, float timeTotal, char instance[]) {
	FILE *arq;
	arq = fopen("./Results/Results.csv", "a");

	if (!arq) {
		printf("\n\nFile not found Results.xls!!!");
		getchar();
		exit(1);
	}

	fprintf(arq,"\n%s", instance);
	fprintf(arq,"\t%s", mh);
	fprintf(arq,"\t%d", (int)ofvs.size());
	
	for (unsigned int i=0; i<ofvs.size(); i++){
		fprintf(arq,"\t%.2f", ofvs[i]);   
	}
	
	fprintf(arq,"\t%.1f", ofv);
	fprintf(arq,"\t%.1f", ofvAverage);
	fprintf(arq,"\t%.3f", timeBest);
	fprintf(arq,"\t%.3f", timeTotal);

	fclose(arq);
}