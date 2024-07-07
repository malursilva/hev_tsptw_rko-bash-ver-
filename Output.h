#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Data.h"

void WriteSolution(char mh[], TSol s, int n, char instance[], float timeBest, float timeTotal) {
    printf("\n\n\nMetaheuristic: %s \nInstance: %s \nCustomers: ", mh, instance);
	for (int i=0; i<n/2; i++)
		printf("%d ", s.vector[i].value);
    printf("\nModes: ");
    for (int i=n/2; i<n; i++)
		printf("%d ", s.vector[i].value);

	printf("\nfo: %.2lf", s.objFValue); 
	printf("\nTotal time: %.3f",timeTotal);
	printf("\nBest time: %.3f\n\n",timeBest);
}
