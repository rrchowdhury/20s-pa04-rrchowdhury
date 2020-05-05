/*
 * student: Ramisa Chowdhury
 * email: rrchowdhury@smu.edu
 * ID: 45539186
 * course: CS3353 Algorithms
 * project 4: Strongly Connected Components
 * @desc A class to test all algorithms implemented in the project and analyse their times
 */

#ifndef INC_20S_3353_PA03_TESTER_H
#define INC_20S_3353_PA03_TESTER_H

#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <cmath>
#include "Graph.h"

class Tester {
public:
    Tester();
    ~Tester();
    void scc_time(std::ofstream&);
    void scc_proof(std::ofstream&);
    void scc_robust(std::ofstream&);
private:

    int iterations{};
    vector<vector<int>> vertices;
    vector<string> num_node_set;
    vector<string> infile_time;
    vector<string> infile_robust;
    vector<string> infile_proof;

    static bool sort_by_size (vector<string>, vector<string>);
    static bool sort_by_string (vector<string>, vector<string>);

    //void setup_set();

};


#endif //INC_20S_3353_PA03_TESTER_H
