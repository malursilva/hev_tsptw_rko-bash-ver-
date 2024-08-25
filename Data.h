#ifndef _DATA_H
#define _DATA_H

#include <vector>
#include <algorithm>

struct TSItem {
    double key;
    int value; // customer index or driving mode, depending on the vector index
};

struct TSol {
    std::vector <TSItem> vector;
    double objFValue;
    int label;
    int similar;
    int flag;
    int promising;
};

struct TNode {
    int node;
    int serviceTime;
    int timeWindow_start;
    int timeWindow_end;
    double departure_time;
};

struct TEdge {
    int nodeStart; // maybe not needed
    int nodeEnd;
    int mode;
    double cost;
    double time;
};

struct TInfo {
    int node;
    int pos;
    int mode;
    double arrival_time;
    double beginning_service;
    double end_service;
    double wait_time;
    double travel_time;
    double travel_cost;
    double service_time;
};

#endif