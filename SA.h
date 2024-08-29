#ifndef _SA_H
#define _SA_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <sys/time.h>
#include <utility>  // pair
#include <numeric>  // iota
#include <map>
#include <limits>
#include <random>
#include <chrono>
#include <iomanip> //graph
#include <sstream> //graph
#include <fstream> //graph

#include "Data.h"
#include "Problem.h"
#include "Method.h"


// Variables declared in main.cpp
extern char nameTable[100];                 // name of instance
extern int debug;                           // 0 - run mode      		    1 - debug mode
extern int numDecoders;                     // number of decoders
extern int numLS;                           // 0 - without local search     > k - number of local search heuristics
extern int MAXTIME;                         // maximum runtime
extern float OPTIMAL;                       // optimal solution (if it is known)
extern struct timeval Tstart, Tend, Tbest;  // computational time (unix systems)  

extern int n;                               // size of cromossoms
extern TSol bestSolution;                   // best solution found in the A-BRKGA


//SA
static TSol s;                              // solução corrente do SA
static TSol sMelhor;                        // melhor solução do SA
static TSol sViz;                           // solução vizinha

static double delta;

static double T0;                           // temperatura inicial
static double T;                            // temperatura corrente
static double alfa;                         // taxa de resfriamento
static int SAmax;                           // numero maximo de iteracoes
static int IterT;                           // iteracao corrente



//****************************** General Functions **********************************

/************************************************************************************
 Method: updateBestSolution()
 Description: Update the best solution found during the run
*************************************************************************************/
static void updateBestSolution(TSol s);

/************************************************************************************
 Method: CREATE INITIAL SOLUTIONS
 Description: create a initial chromossom with random keys
*************************************************************************************/
static TSol CreateInitialSolutions();

/************************************************************************************
 Method: RANDOMICO
 Description: Generate a double random number between min and max
*************************************************************************************/
static double randomico(double min, double max);

/************************************************************************************
 Method: IRANDOMICO
 Description: Generate a int random number between min and max
*************************************************************************************/
static int irandomico(int min, int max);

#endif