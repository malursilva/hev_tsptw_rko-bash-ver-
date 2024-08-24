#include "Problem.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
//                      Utils auxiliar function                                    //
/////////////////////////////////////////////////////////////////////////////////////
bool sortByKey(const TSItem &lhs, const TSItem &rhs) { return lhs.key < rhs.key; }

bool sortInfoByCost(const TInfo &lhs, const TInfo &rhs) { return lhs.travel_cost < rhs.travel_cost; }

float generateRandomKey() {
    return (double) rand() / (RAND_MAX + 1.0);    
}

int getKeyInterval(float val) {
    if(val < 0.5) {
        if(val < 0.25) return C;
        else return CC;
    } else {
        if(val < 0.75) return E;
        else return B;
    }
}

void splitStringIntoIntVector(string line, vector<int> &vector) {
    stringstream ss(line);
    string sub;
    while(ss.good()) {
        getline(ss, sub, ',');
        if(!sub.empty())
            vector.push_back(stoi(sub));
    }
}

void initializeMatrix(vector<vector<double>> &matrix, int n) {
    for(int i=0; i<n; i++) {
        vector<double> aux(n);
        matrix.push_back(aux);
    }
}

void splitAllModesMatrix(char line[], int n, vector<vector<double>> &m1, vector<vector<double>> &m2, vector<vector<double>> &m3, vector<vector<double>> &m4) {
    stringstream ss(line);
    string sub;

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            getline(ss, sub, ',');
            m1[i][j] = stod(sub);
        }
    }
    
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            getline(ss, sub, ',');
            m2[i][j] = stod(sub);
        }
    }
    
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            getline(ss, sub, ',');
            m3[i][j] = stod(sub);
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            getline(ss, sub, ',');
            m4[i][j] = stod(sub);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////

void ReadData(char nameFile[]) {
    char name[200] = "./Instances/";
    FILE* file;
    int lineSize = 100000, lineCount = 1, mSize = 0;
    char line[lineSize];

    strcat(name, nameFile);
    file = fopen(name, "r");

    if (!file) {
        printf("\nERROR: File (%s) not found!\n",name);
        getchar();
        exit(1);
    } 
    // read all values from file that will be used
    while(fgets(line, lineSize, file)) {
        switch (lineCount) {
            case 5:
                nCustomers = stoi(line);
                mSize = nCustomers + 1;
            break;
        
            case 8:
                splitStringIntoIntVector(line, serviceTime);
            break;

            case 11:
                initialCharge = stoi(line);
            break;

            case 14:
                maxCharge = stoi(line);
            break;

            case 17:
                chargeRate = stoi(line) / 60.0;
            break;

            case 20:
                dischargeRate = stoi(line) / 60.0;
            break;

            case 29:
                initializeMatrix(c_timeMatrix, mSize);
                initializeMatrix(cc_timeMatrix, mSize);
                initializeMatrix(e_timeMatrix, mSize);
                initializeMatrix(b_timeMatrix, mSize);
                splitAllModesMatrix(line, mSize, c_timeMatrix, cc_timeMatrix, e_timeMatrix, b_timeMatrix);
            break;

            case 32:
                initializeMatrix(c_costMatrix, mSize);
                initializeMatrix(cc_costMatrix, mSize);
                initializeMatrix(e_costMatrix, mSize);
                initializeMatrix(b_costMatrix, mSize);
                splitAllModesMatrix(line, mSize, c_costMatrix, cc_costMatrix, e_costMatrix, b_costMatrix);
            break;

            case 35:
                splitStringIntoIntVector(line, timeWindows);
            break;

            default:
            break;
        }

        lineCount++;
    }

    // The keys vector doesn't include the depot node, but the path to return to depot needs to be included, making nCustomers nodes and nCustomers+1 edges
    n = (2 * nCustomers) + 1; 

    fclose(file);
}

double getCost(int nodeA, int nodeB, int mode) {
    switch (mode) {
        case C:
            return c_costMatrix[nodeA][nodeB];         
        break;
            
        case CC:
            return cc_costMatrix[nodeA][nodeB];
        break;

        case E:
            return e_costMatrix[nodeA][nodeB];
        break;

        case B:
            return b_costMatrix[nodeA][nodeB];
        break;

        default:
            return INFINITO;
        break;
    }
}

double getTime(int nodeA, int nodeB, int mode) {
        switch (mode) {
        case C:
            return c_timeMatrix[nodeA][nodeB];         
        break;
            
        case CC:
            return cc_timeMatrix[nodeA][nodeB];
        break;

        case E:
            return e_timeMatrix[nodeA][nodeB];
        break;

        case B:
            return b_timeMatrix[nodeA][nodeB];
        break;

        default:
            return INFINITO;
        break;
    }
}

void Decoder1(TSol &s) {
    TSol temp;
    
    for(int i=0; i<n; i++) {
        if(i < nCustomers) {
            s.vector[i].value = i + 1;    
        } else {
            s.vector[i].value = getKeyInterval(s.vector[i].key);
        }
    }

    temp = s;
    s.objFValue = 0;

    nodes.clear();
    nodes.resize(nCustomers+1);

    edges.clear();
    edges.resize(nCustomers+1);

    sort(s.vector.begin(), s.vector.begin() + nCustomers, sortByKey);

    // Add depot node
    TNode auxNode;
    int index, nodeA, nodeB;
    auxNode.node = 0;
    auxNode.serviceTime = serviceTime[0];
    auxNode.timeWindow_start = timeWindows[0];
    auxNode.timeWindow_end = timeWindows[1];
    auxNode.departure_time = 0.0;
    nodes[0] = auxNode;

    // Add customers in the order they'll be visited
    for(int i=0; i<nCustomers; i++) {
        index = s.vector[i].value;
        auxNode.node = index;
        auxNode.serviceTime = serviceTime[index];
        auxNode.timeWindow_start = timeWindows[index*2];
        auxNode.timeWindow_end = timeWindows[(index*2)+1];
        nodes[i+1] = auxNode;
    }

    TEdge auxEdge;
    // Add edges with modes given by the random keys 
    for(int i=0; i<edges.size(); i++) {
        auxEdge.mode = s.vector[i+nCustomers].value;
        auxEdge.nodeStart = nodeA = nodes[i].node;
        auxEdge.nodeEnd = nodeB = nodes[(i+1) % nodes.size()].node;
        auxEdge.cost = getCost(nodeA, nodeB, auxEdge.mode);
        auxEdge.time = getTime(nodeA, nodeB, auxEdge.mode);
        edges[i] = auxEdge;
    }

    // Verify if path is viable

    // First check if the time-windows are begin respected
    int m = edges.size(), endIndex;
    double auxTime = 0.0;
    long double factor = 0.0;

    for(int i=0; i<m; i++) {
        endIndex = (i+1) % nodes.size();
        auxTime = nodes[i].departure_time + edges[i].time;
        
        if (auxTime < nodes[endIndex].timeWindow_start && edges[i].nodeEnd != 0) { // arrive earlier than time window start is valid
            nodes[endIndex].departure_time = nodes[endIndex].timeWindow_start + nodes[endIndex].serviceTime; // in this case the depature time will be always valid
        } else {
            if(edges[i].nodeEnd != 0) 
                nodes[endIndex].departure_time = auxTime + nodes[endIndex].serviceTime;
            
            if(auxTime > nodes[endIndex].timeWindow_end) { // arrived later than window
                factor += FACTOR_MULT * (auxTime - nodes[endIndex].timeWindow_end);
            } else if (nodes[endIndex].departure_time > nodes[endIndex].timeWindow_end) { // left later than needed
                factor += FACTOR_MULT * (nodes[endIndex].departure_time - nodes[endIndex].timeWindow_end);
            }
        }
    }

    // Calculate the costs, checking the battery level
    double level = initialCharge;
    for(int i=0; i<m; i++) {
        switch (edges[i].mode) {
            case CC:
                level += chargeRate * edges[i].time;
                if(level > maxCharge) 
                    level = maxCharge;
            break;

            case E:
                level -= dischargeRate * edges[i].time;
                if (level < 0) {
                    factor += FACTOR_MULT * level * -1;
                    level = 0;
                }
            break;

            case B:
                level -= dischargeRate * edges[i].time;
                if (level < 0) {
                    factor += FACTOR_MULT * level * -1;
                    level = 0;
                }
            break;
        
            default:
            break;
        }
        s.objFValue += edges[i].cost;
    }

    if (factor > 0) {
        s.objFValue += factor + BAD_SOLUTION_EXTRA;
    }   
   
    // return initial random-key sequence and maintain the solution sequence
    for (int i=0; i<n; i++){
        s.vector[i].key = temp.vector[i].key;
    }
}

void Decoder2(TSol &s) {
    TSol temp;

    for(int i=0; i<n; i++) {
        if(i < nCustomers) {
            s.vector[i].value = i + 1;    
        } else {
            s.vector[i].value = getKeyInterval(s.vector[i].key);
        }
    }

    temp = s;
    s.objFValue = 0;

    nodes.clear();
    nodes.resize(0);
    edges.clear();
    edges.resize(0);

    sort(s.vector.begin(), s.vector.begin() + nCustomers, sortByKey);

    TNode auxNode;
    TEdge auxEdge;
    TInfo auxPosInfo;
    vector<TNode>::iterator nodePos;
    vector<TEdge>::iterator edgePos;
    vector<TInfo> infoDataVector;
    
    // Add depot node
    auxNode.node = 0;
    auxNode.serviceTime = serviceTime[0];
    auxNode.timeWindow_start = timeWindows[0];
    auxNode.timeWindow_end = timeWindows[1];
    auxNode.departure_time = 0;
    nodes.push_back(auxNode);

    // Add customers by the order given
    for(int i=0; i<nCustomers; i++) {
        // For each i that will be inserted, calculate the Info distance/costs to all possible positions to be inserted
        infoDataVector.clear();
        auxPosInfo.node = s.vector[i].value;
        auxPosInfo.mode = s.vector[i + nCustomers].value;
        auxPosInfo.service_time = serviceTime[auxPosInfo.node];
        
        for(int j=0; j<nodes.size(); j++) {
            auxPosInfo.pos = j+1;
            auxPosInfo.travel_cost = getCost(nodes[j].node, auxPosInfo.node, auxPosInfo.mode);            
            auxPosInfo.travel_time = getTime(nodes[j].node, auxPosInfo.node, auxPosInfo.mode);
            auxPosInfo.arrival_time = nodes[j].departure_time + auxPosInfo.travel_time;
            
            // Arrival (plus service) needs to be smaller than the end of the time window, or else the insertion is for sure not viable
            if ((auxPosInfo.arrival_time + auxPosInfo.service_time) < timeWindows[(auxPosInfo.node*2)+1]) {
                if(auxPosInfo.arrival_time < timeWindows[auxPosInfo.node*2]) {
                    auxPosInfo.beginning_service = timeWindows[auxPosInfo.node*2];
                    auxPosInfo.wait_time = auxPosInfo.beginning_service - auxPosInfo.arrival_time;
                } else {
                    auxPosInfo.beginning_service = auxPosInfo.arrival_time;
                    auxPosInfo.wait_time = 0.0;
                }
                auxPosInfo.end_service = auxPosInfo.beginning_service + auxPosInfo.service_time;

                infoDataVector.push_back(auxPosInfo);
            }
        }
        
        // Select first viable path with smallest cost
        sort(infoDataVector.begin(), infoDataVector.end(), sortInfoByCost);
        int x = 0;
        do {
            auxPosInfo = infoDataVector[x];
            if (auxPosInfo.wait_time > 0.0) {
                x = -1;
            } else if ((auxPosInfo.beginning_service > timeWindows[(auxPosInfo.node*2)+1]) || (auxPosInfo.end_service > timeWindows[(auxPosInfo.node*2)+1])) {
                x++;
            } else {
                x = -1;
            }
        } while(x >= 0 && x < infoDataVector.size());
    
        // Fill nodes info - (check later if these are necessary)
        auxNode.node = auxPosInfo.node;
        auxNode.timeWindow_start = timeWindows[auxNode.node*2];
        auxNode.timeWindow_end = timeWindows[(auxNode.node*2)+1];
        auxNode.serviceTime = serviceTime[auxNode.node];
        
        nodePos = (nodes.begin() + auxPosInfo.pos);
        nodes.insert(nodePos, auxNode);

        auxEdge.mode = auxPosInfo.mode;        
        edgePos = (edges.begin() + auxPosInfo.pos - 1);
        edges.insert(edgePos, auxEdge);

        infoDataVector.clear();
    }

    // Add path back to depot
    auxEdge.mode = s.vector[n-1].value;
    edges.push_back(auxEdge);

    int n = nodes.size();
    double level = initialCharge;
    double factor = 0.0, auxTime = 0.0;

    for(int i=0; i<edges.size(); i++) {
        edges[i].nodeStart = nodes[i].node;
        edges[i].nodeEnd = nodes[(i+1)%n].node;
        edges[i].cost = getCost(edges[i].nodeStart, edges[i].nodeEnd, edges[i].mode);
        edges[i].time = getTime(edges[i].nodeStart, edges[i].nodeEnd, edges[i].mode);

        switch (edges[i].mode) {
            case CC:
                level += chargeRate * edges[i].time;
            break;

            case E:
                level -= dischargeRate * edges[i].time;
            break;

            case B:
                level -= dischargeRate * edges[i].time;
            break;
        
            default:
            break;
        }

        if(level > maxCharge) {
            level = maxCharge;
        } else if (level < 0.0) {
            factor += FACTOR_MULT * (level* -1);
            level = 0.0;
        }

        auxTime = nodes[i].departure_time + edges[i].time;
        
        if (auxTime < nodes[(i+1)%n].timeWindow_start && edges[i].nodeEnd != 0) { // arrive earlier than time window start is valid
            nodes[(i+1)%n].departure_time = nodes[(i+1)%n].timeWindow_start + nodes[(i+1)%n].serviceTime; // in this case the depature time will be always valid
        } else {
            if(edges[i].nodeEnd != 0) 
                nodes[(i+1)%n].departure_time = auxTime + nodes[(i+1)%n].serviceTime;
            
            if(auxTime > nodes[(i+1)%n].timeWindow_end) { // arrived later than window
                factor += FACTOR_MULT * (auxTime - nodes[(i+1)%n].timeWindow_end);
            } else if (nodes[(i+1)%n].departure_time > nodes[(i+1)%n].timeWindow_end) { // left later than needed
                factor += FACTOR_MULT * (nodes[(i+1)%n].departure_time - nodes[(i+1)%n].timeWindow_end);
            }
        } 

        s.objFValue += edges[i].cost;   
    }

    if (factor > 0) {
        s.objFValue += factor + BAD_SOLUTION_EXTRA;
    }     

    for (int i=0; i<n; i++){
        s.vector[i].key = temp.vector[i].key;
    }
}

void Decoder(TSol &s) {
    if (s.vector[n-1].key < 0.75) {
        Decoder1(s);
    } else {
        Decoder2(s);
    }
}

void Constructive(TSol &s) {
    s.objFValue = 0;
    s.vector.resize(n);
    
    // Create an initial solution following the criteria: ordering the customers by the time window start time and setting combustion to all edges modes
    vector<TSItem> auxNodes;
    TSItem aux;
    int index, nodeA, nodeB;
    auxNodes.reserve(nCustomers);

    for(int i=1; i<=nCustomers; i++) {
        aux.value = i;
        aux.key = timeWindows[i];
        auxNodes.push_back(aux);
    }
    // Define visit order sorting customers by their time window start time
    sort(auxNodes.begin(), auxNodes.end(), sortByKey);

    // Define solution key vector
    double factor = 1.0/nCustomers;
    double min, max;
    for(int i=0; i<nCustomers; i++) { // first positions with keys to determined nodes 
        min = factor * auxNodes[i].value;
        max = factor * (auxNodes[i].value + 1);
        s.vector[i].key = ((double)(rand()%10000)/10000.0)*(max-min)+min;
        s.vector[i].value = auxNodes[i].value;
    }
    factor = 1.0/NUM_MODES;
    min = factor * C;
    max = factor * (C+1);
    for(int i=nCustomers; i<n; i++) { // second half with keys to same mode combustion
        s.vector[i].key = ((double)(rand()%10000)/10000.0)*(max-min)+min;
        s.vector[i].value = C;
    }

    Decoder(s);
}

void FreeMemoryProblem() {
    nodes.clear();
    edges.clear();
}

