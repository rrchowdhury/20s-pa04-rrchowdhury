/*
 * student: Ramisa Chowdhury
 * email: rrchowdhury@smu.edu
 * ID: 45539186
 * course: CS3353 Algorithms
 * project 4: Strongly Connected Components
 * @desc A Graph class made up of nodes (strings) and edges (connections between nodes) with accessing algorithms
 *       finding strongly connected compnents with a trivial and robust (Kosajaru's) algorithm
 */

#ifndef INC_20S_3353_PA03_GRAPH_H
#define INC_20S_3353_PA03_GRAPH_H
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <stack>

using namespace std;

class Graph {
public:

    //Constructors
    Graph();
    explicit Graph(const string&);
    ~Graph();

    //Access
    int get_num_elem();

    //Algorithm
    vector<vector<string>> scc();
    vector<vector<string>> scc_trivial();

private:

    //private algorithms accessed by public algorithms
    void dfs_scc(unordered_map<string, vector<string>>&, unordered_map<string, bool>&, unordered_map<string, bool>&, stack<string>&, stack<string>&);
    stack<string> path(string&);

    /* CLASS VARIABLES
     * num_elem: number of nodes in the graph
     * num_edges: number of edges in the graph
     * grph: adjacency list representing graph
     * grphTrans: adjacency list representing transposed graph
     * vec_default: vector used to fill up graph maps
     */

    int num_elem{};
    int num_edges{};
    unordered_map<string,vector<string>> grph;
    unordered_map<string,vector<string>> grphTrans;
    vector<string> vec_default;
};
#endif //INC_20S_3353_PA03_GRAPH_H
