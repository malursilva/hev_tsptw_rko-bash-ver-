#ifndef _PROBLEM_H
#define _PROBLEM_H

#define INFINITO 999999999
#define BAD_SOLUTION_EXTRA 1000000
#define FACTOR_MULT 1000
#define NUM_MODES 4
#define C 1
#define CC 2
#define E 3
#define B 4

#include <vector>
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include "Data.h"

using namespace std;

extern int n;

///////////////////////////////////////////////////////
//              HEV-TSPTW given values              //
//////////////////////////////////////////////////////
static int nCustomers;
// Battery values adapted do Watt/min
static double initialCharge;
static double maxCharge;
static double chargeRate;
static double dischargeRate;
// Time values all given in minutes
static vector<int> serviceTime;
static vector<int> timeWindows;
static vector<vector<double>> c_timeMatrix;
static vector<vector<double>> cc_timeMatrix;
static vector<vector<double>> e_timeMatrix;
static vector<vector<double>> b_timeMatrix;
static vector<vector<double>> c_costMatrix;
static vector<vector<double>> cc_costMatrix;
static vector<vector<double>> e_costMatrix;
static vector<vector<double>> b_costMatrix;

// Constructed Path Variables 
static vector<TNode> nodes;
static vector<TEdge> edges;


void ReadData(char nameFile[]);

void Decoder(TSol &s, int numDecoders);

double CalculateFitness(TSol s);

void Constructive(TSol &s);

void FreeMemoryProblem();

void splitStringIntoIntVector(string line, vector<int> &vector);

void initializeMatrix(vector<vector<double>> &matrix, int n);

void splitAllModesMatrix(string line, int n, vector<vector<double>> &m1, vector<vector<double>> &m2, 
                                             vector<vector<double>> &m3, vector<vector<double>> &m4);

#endif