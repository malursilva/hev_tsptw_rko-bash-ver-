#include "SA.h"

/************************************************************************************
			                  GENERAL FUNCTIONS
*************************************************************************************/

void SA()
{
    // free memory with problem data
    //FreeMemoryProblem();

    // free memory of BRKGA components
    FreeMemory();

    //read data of the instance
    ReadData(nameTable);

    int reAnnealing = 0;
    float currentTime = 0;              // computational time of the search process

    // run the search process until stop criterion
    while(1) {
        if (reAnnealing == 0) {
            // Create the initial solution with random keys
            sMelhor.objFValue = INFINITO;
            for (int i=0; i<1000; i++) {
                TSol ind = CreateInitialSolutions(); 
                Decoder(ind);
                if (ind.objFValue < sMelhor.objFValue)
                    sMelhor = ind;
            }
        }
        // update the best solution found
        updateBestSolution(sMelhor);
    
        // define parameters
        T0 = 10000000;
        SAmax = 500;
        alfa = 0.99;

        // current solution
        s = sMelhor;
        IterT = 0;

        if (reAnnealing == 0) {
            T = T0;
        } else {
            T = 0.3 * T0;
        }

        while (T > 0.0001) {
            if(T < 200) {
                SAmax = 300;
                alfa = 0.99;
            } else if (T < 10) {
                SAmax = 500;
                alfa = 0.98;
            } else if (T < 1) {
                SAmax = 1000;
                alfa = 0.97;
            }

            while (IterT < SAmax) {
                IterT++;

                // Shake the current solution
                sViz = s;
                int shaking_type;
                int intensity = n * randomico(0.1, 0.3);
                if (intensity < 1.0)
                    intensity = 1;
                for(int k = 0; k < intensity; k++) 
                {
                    shaking_type = irandomico(1,4); 
                    int i = irandomico(0, n - 1);
                    if(shaking_type == 1){
                        // Invert value
                        if (sViz.vector[i].key > 0.0001)
                            sViz.vector[i].key = 1.0 - sViz.vector[i].key;
                        else
                            sViz.vector[i].key = 0.9999;
                    }
                    else 
                    if (shaking_type == 2){
                        // Swap two random positions
                        int j = irandomico(0, n - 1);
                        double temp = sViz.vector[i].key;
                        sViz.vector[i].key = sViz.vector[j].key;
                        sViz.vector[j].key = temp;
                    }
                    else
                    if(shaking_type == 3){
                        // Change to random value
                        sViz.vector[i].key = randomico(0,1);
                    }
                    i = irandomico(0, n - 2);
                    if(shaking_type == 4){
                        // Swap with neighbor
                        double temp = sViz.vector[i].key;
                        sViz.vector[i].key = sViz.vector[i+1].key;
                        sViz.vector[i+1].key = temp;
                    }
                }

                // calcular a FO
                Decoder(sViz);
//                sViz.objFValue =  rand()%(100000000-10000  + 1) + 10000;
                
                // calcular o delta
                delta = sViz.objFValue - s.objFValue;

                //definir a partir de qual solucao continuar a busca
                if (delta < 0) {
                    s = sViz;
                    updateBestSolution(s);
                } else {
                    double x = randomico(0,1);

                    if (x < (exp(-delta/T)))       
                        s = sViz;
                }
            } //Fim-SAmax

            if (debug)
                printf("\nT = %.4f \t FO = %.2lf \t melhorFO = %.2lf", T, s.objFValue, bestSolution.objFValue);

            T = T * alfa;
            IterT = 0;

            // terminate the search process in MAXTIME
            gettimeofday(&Tend, NULL);
            currentTime = ((Tend.tv_sec  - Tstart.tv_sec) * 1000000u + Tend.tv_usec - Tstart.tv_usec) / 1.e6; 
            
            // stop criterium
            if (currentTime >= MAXTIME || floorf(bestSolution.objFValue*10)/10 <= OPTIMAL) {  
                break;
            }

        } //Fim-T

        reAnnealing = 1;

        // generate a new seed
        if (!debug)
            srand(time(NULL)); 

        // stop criterium
        if (currentTime >= MAXTIME || floorf(bestSolution.objFValue*10)/10 <= OPTIMAL) {  
            break;
        }
    }

    // free memory with problem data
    FreeMemoryProblem();

    // free memory of BRKGA-QL components
    FreeMemory();
}

// Create random-key vector
TSol CreateInitialSolutions() {
	TSol s;
	TSItem aux;

    s.vector.clear();

	// create a random-key for each allelo (consider decoder type in the n-th random-key)
	for (int j=0; j<n; j++) {
        aux.key  = randomico(0,1);  // random value between [0,1[
        aux.value = 0;
        s.vector.push_back(aux);
	}

    // flag to control the local search memory
    s.flag = 0;

	return s;
}

void updateBestSolution(TSol s) {
    if (s.objFValue < bestSolution.objFValue){
        bestSolution = s;
        gettimeofday(&Tbest, NULL);
    }
}

void FreeMemory() {
    //methods
    
}

double randomico(double min, double max) {
	return ((double)(rand()%10000)/10000.0)*(max-min)+min;
}

int irandomico(int min, int max) {
	return (int)randomico(0,max-min+1.0) + min;
}