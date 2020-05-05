/*
 * student: Ramisa Chowdhury
 * email: rrchowdhury@smu.edu
 * ID: 45539186
 * course: CS3353 Algorithms
 * project 4: Strongly Connected Components
 * @desc A Graph class made up of nodes (strings) and edges (connections between nodes) with accessing algorithms
 *       finding strongly connected compnents with a trivial and robust (Kosajaru's) algorithm
 */

#include "Graph.h"

Graph::Graph()= default;

/*--------------------------------------------------------------------------
 * GRAPH CONSTRUCTION
 *
 * @param file: file of graph to be opened
 *
 * @desc goes through file and constructs an adjacency list representing the
 *       directed graph and its transpose
 --------------------------------------------------------------------------*/
Graph::Graph(const string& file) {

    ifstream fin(file);

    if(fin.is_open()) {

        // vertex1 of unordered map
        string vertex1;

        // name of node that is connected to the vertex1
        string vertex2;


        /* -----------------------------
         * gets number of nodes in graph
         ------------------------------*/
        char *in_char = new char[1024];
        fin.getline(in_char, 1024, '[');
        fin.getline(in_char, 1024, ']');
        char *pEnd;
        num_elem = (int) strtol(in_char, &pEnd, 10);
        fin.getline(in_char, 1024, '\n');


        /* ---------------------------------
         * fills out map with nodes in graph
         ----------------------------------*/
        for (int i = num_elem; i > 0; i--) {
            fin.getline(in_char, 1024, '\n');
            vertex1 = string(in_char);

            //removes carriage return
            if (!vertex1.empty() && vertex1[vertex1.size() - 1] == '\r')
                vertex1.erase(vertex1.size() - 1);

            //trim
            while (!vertex1.empty() && vertex1[vertex1.size() - 1] == ' ')
                vertex1.erase(vertex1.size() - 1);
            while (!vertex1.empty() && vertex1[0] == ' ')
                vertex1 = vertex1.substr(1, vertex1.length() - 1);

            //vertices.push_back(vertex1);

            grph[vertex1] = vec_default;
            grphTrans[vertex1] = vec_default;

        }


        /* -------------------------------------------------------------
         * filling vector of connections in unordered map of graph nodes
         --------------------------------------------------------------*/

        // gets number of edges
        fin.getline(in_char, 1024, '[');
        fin.getline(in_char, 1024, ']');
        num_edges = (int) strtol(in_char, &pEnd, 10);
        fin.getline(in_char, 1024, '\n');

        while (fin.getline(in_char, 1024, ',')) {

            // gets vertex1
            vertex1 = string(in_char);
            if (vertex1[0] != '\r' && vertex1[0] != '\n') {


                // vertex1; removes carriage return;
                if (!vertex1.empty() && vertex1[vertex1.size() - 1] == '\r')
                    vertex1.erase(vertex1.size() - 1);
                //trim
                while (!vertex1.empty() && vertex1[vertex1.size() - 1] == ' ')
                    vertex1.erase(vertex1.size() - 1);
                while (!vertex1.empty() && vertex1[0] == ' ')
                    vertex1 = vertex1.substr(1, vertex1.length() - 1);



                // gets vertex2; removes carriage return
                fin.getline(in_char, 1024, '\n');
                vertex2 = string(in_char);
                if (!vertex2.empty() && vertex2[vertex2.size() - 1] == '\r')
                    vertex2.erase(vertex2.size() - 1);
                // trim
                while (!vertex2.empty() && vertex2[vertex2.size() - 1] == ' ')
                    vertex2.erase(vertex2.size() - 1);
                while (!vertex2.empty() && vertex2[0] == ' ')
                    vertex2 = vertex2.substr(1, vertex2.length() - 1);


                // adds directed edges to graph and transposed graph
                grph[vertex1].push_back(vertex2);
                grphTrans[vertex2].push_back(vertex1);
            }
        }
        delete[] in_char;
    }
}

Graph::~Graph()= default;

/*
 * @return number of nodes in the GRAPH
 */
int Graph::get_num_elem() {
    return num_elem;
}

/*
 * @desc: does a depth first search on the graph and keeps track of multiple through parameters passed by reference
 *        - All these pass by reference parameters or to make sure that DFS span the entire graph to create forests
 *          where necessary
 *        - They are also to make sure that it goes smoothly when called on both the graph and its transpose
 *        - this depth first search function was modified to specifically work for Kosajaru's algorithm
 *
 * @param: g
 *         This is the graph that depth first search is being run on. This is a parameter and not used as a class variable
 *         because the function does not distinguish whether the graph or its transpose is being called, so it's passed in
 *
 * @param: visited
 *         This keeps track of all visited nodes so that the depth first search does not access visited nodes again
 *         - this is passed by reference and is NOT initialized to all unvisited
 *         - in the case that the dfs does not reach all nodes, this is used so that when the function is called again
 *           another start node, it does not revisit these
 *
 * @param: unvisited
 *         This keeps track of which nodes are left. Once a node it visited, it is deleted entirely from this
 *         - If this is not empty by the end of the dfs, then the dfs did not reach all nodes
 *         - This is how whatever is calling this knows that dfs needs to called again from a different node
 *           to create the search forest
 *
 * @param: nodes_start
 *         This stack holds all the nodes in the graph and pops it off one by one each time dfs_scc() is called
 *         - only the top node is used as the start node every time dfs_scc is called
 *         - if the top of the start stack was accessed in a previous dfs_scc of the same graph in the same algorithm,
 *           then the function ends
 *
 * @param: nodes_scc
 *         This holds the strongly connected component that the start node is a part of DEPENDING ON WHEN IT IS CALLED
 *         - When dfs_scc() is called on the graph, once it reaches the end of a path, it pushes the final value into
 *           nodes_scc
 *         - When dfs_scc() is called of the TRANSPOSE of the graph, the previously computed nodes_scc is sent in as
 *           nodes_start, and the dfs trees that result from are the strongly connected components of the graph, now
 *           stores in nodes_scc
 */
void
Graph::dfs_scc(unordered_map<string, vector<string>> & g, unordered_map<string, bool> & visited, unordered_map<string, bool> & unvisited, stack<string> & nodes_start, stack<string> & nodes_scc) {

    //number of nodes that have no been visited
    int count_final = unvisited.size();

    //initializing nodes to be explored
    string start = nodes_start.top(); nodes_start.pop();
    string v = start;
    string u;

    //stacks to keep track of path of depth first search
    stack<string> nodes_dfs;
    stack<int> index_dfs;

    //current index of node v's child to be explored
    int i = 0;

    //only execute if the start node has not already been spanned by another call of deapth first search
    if(!visited[v]) {

        //visit start node
        visited[v] = true;
        unvisited.erase(v);

        //number of nodes visited in this run
        int count = 1;

        /*--------------------------------------------------------------
         * Graph traversal
         --------------------------------------------------------------*/
        while (count <= count_final) {

            //if node v's list of adjacent nodes has not been traversed
            if (i < g[v].size()) {

                //find node v's child and increment pointer of child list in Graph
                u = g[v][i];
                i++;

                //if this child has never been visited, it's a valid child for the output
                if (!visited[u]) {

                    //updates visited node map
                    visited[u] = true;
                    unvisited.erase(u);
                    count++;

                    //saves current node v into stack and sets child node u as current node v to continue traversal
                    //since it's node v's first traversal, start at first element in adjacent node list in Graph
                    nodes_dfs.push(v);
                    index_dfs.push(i);
                    v = u;
                    i = 0;
                }
            }

            /*----------------------------------------------------------------------
             * if node v's list of adjacent nodes has been traversed
             * set the current node and adjacent_node_index to the top of the stacks
             * to continue exploring that node's adjacent nodes
             *
             * also push the dead end node onto the scc stack.
             * if the graph is original, this will determine which nodes to start the
             *      dfs of the transpose from
             * if the graph is the transpose, this represents the strongly connected components
             ----------------------------------------------------------------------*/
            if (i >= g[v].size()) {
                if (!nodes_dfs.empty()) {
                    nodes_scc.push(v);
                    v = nodes_dfs.top();
                    nodes_dfs.pop();
                    i = index_dfs.top();
                    index_dfs.pop();
                } else {
                    count = num_elem + 1;
                }
            }
        }

        //pushes start node because it never gets pushed
        nodes_scc.push(start);
    }
}

/*
 * @desc: KOSAJARU'S ALGORITHM
 *        calls depth first search on the graph, pushes nodes onto stack with deepest at the
 *          bottom and shallowest(?) at the top
 *        calls depth first search on the transpose starting with the previously computed
 *          shallowest to deepest
 *          the search trees they produce are the strongly connected components
 */
vector<vector<string>> Graph::scc() {

    // keeps track of which nodes have been visited from all the
    // calls on dfs for the original graph
    unordered_map<string, bool> visited;
    unordered_map<string, bool> unvisited;

    //stack of nodes that dfs uses to start
    //updates every time dfs is called
    //   the reason it is a stack is because the algorithm requires it to be a stack when
    //   dfs is called on the transpose
    stack<string> start;

    // initializes the start stack and all nodes to unvisited
    for(const auto& x: grph){
        visited[x.first]= false;
        unvisited[x.first]= false;
        start.push(x.first);
    }

    // final: the stack of nodes from deep to shallow on a dfs called on start.pop()
    stack<string> final;

    //calls dfs and updates 'final' until all nodes have been visited once
    while(!unvisited.empty()) {
        dfs_scc(grph, visited, unvisited, start, final);
    }


    //resets everything to unvisited for the dfs to be called on the transpose
    visited.clear();
    unvisited.clear();
    for(const auto& x: grph){
        visited[x.first]= false;
        unvisited[x.first]= false;
    }

    // a vector of stacks holding all the strongly connected components
    //      each stack is a strongly connected component
    vector<stack<string>> final_scc;

    // calls dfs on the transpose and pushes each scc (temp) onto the
    //      scc vector until all nodes have been visited
    while(!final.empty()){
        stack<string> temp;
        dfs_scc(grphTrans, visited, unvisited, final, temp);
        if(!temp.empty()) {
            final_scc.push_back(temp);
        }
    }


    // converts vector of stacks to a vector of vectors and returns it

    vector<vector<string>> list_of_sccs;
    for (auto & i : final_scc) {
        vector<string> sub_scc;
        while (!i.empty()) {
            sub_scc.push_back(i.top());
            i.pop();
        }
        list_of_sccs.push_back(sub_scc);
    }
    return list_of_sccs;
}

/*
 * @return: a stack of nodes representing the longest path from the start node to itself
 *
 * @param: the start node for which to compute the longest path to itself of
 *
 * @desc: this algorithm of finding the longest part to itself is used to find
 *        the strongly connected components of a graph
 *        - a component is strongly connected if it every node in it can cycle back to itself
 *
 *        used by trivial scc algorithm
 */
stack<string> Graph::path(string & start) {

    // stack of all nodes of the longest paths
    stack<string> final;

    /*----------------------------------------------------------------------------
     * Creates another unordered map with the same keys as the Graph unordered map
     * Sets all the values of the keys to 0
     * Values will equal 1 when the node is on a path and back to 0 when it's off the path
     ----------------------------------------------------------------------------*/
    unordered_map<string, int> visited;
    for (const auto& pair: grph){
        visited[pair.first]=0;
    }

    //Edge = {v->u}
    string u;
    string v = start;

    //stack of node v, last node visited before exploring v's child
    stack<string> nodes;

    //stack of indexes of node v's next child to be explored
    stack<int> index;

    //current index of node v's child to be explored
    int i = 0;

    //visit start node
    visited[start] = 1;


    /*--------------------------------------------------------------
     * Graph traversal
     --------------------------------------------------------------*/

    //if node v's list of adjacent nodes has not been traversed
    while(i<grph[v].size()) {

        //find node v's child and increment pointer of child list in Graph
        u = grph[v][i];
        i++;

        //if this child hasn't been visited, it's a valid child for the output
        if (visited[u] == 0) {

            //updates visited node map
            visited[u] = 1;

            //saves current node v into stack and sets child node u as current node v to continue traversal
            //since it's node v's first traversal, start at first element in adjacent node list in Graph
            nodes.push(v);
            index.push(i);
            v = u;
            i = 0;
        }

        // if u has been visited on the path and is also the start node
        // a path to itself has been found
        if(u == start){

            // push the last node of the path on
            nodes.push(v);

            // save the path if its the largest
            if(nodes.size() > final.size()){
                final = nodes;
            }

            // remove the last node again so that it doesn't mess with the algorithm
            nodes.pop();
        }


        /*----------------------------------------------------------------------
         * if node v's list of adjacent nodes has been traversed
         * set the current node and adjacent_node_index to the top of the stacks
         * to continue exploring that node's adjacent nodes
         *
         * set the node to unvisited, as it's no longer on the path and is
         * viable to be visited again on another path
         ----------------------------------------------------------------------*/
        while (i >= grph[v].size() && !nodes.empty()) {
            visited[v] = 0;

            v = nodes.top(); nodes.pop();
            i = index.top(); index.pop();
        }
    }

    // if no paths have been found, then the component is all by itself
    if(final.empty()){
        final.push(start);
    }

    return final;
}

/*
 * @desc: trivial solution to finding stringly connected components of a graph
 *        - gets the longest path to itself of every component in the graph unless
 *          it is a part of another node's longest paths
 *        - longest paths don't necessarily mean scc
 *          so the algorithm unions every longest path with any of the same nodes
 *          to create scc's
 * @return: a vector of vectors representing the scc's
 */
vector<vector<string>> Graph::scc_trivial() {

    //keeps track of which nodes are already a part of a strongly connected component
    unordered_map<string, bool> visited;
    unordered_map<string, bool> unvisited;

    //the nodes of the graph; determines which node path() is called on
    stack<string> nodes;

    //initializes all nodes to unvisited
    for(const auto& x: grph){
        visited[x.first]= false;
        unvisited[x.first]= false;
        nodes.push(x.first);
    }

    //list of all the strongly connected components of the graph
    vector<vector<string>> list_of_sccs;
    string start;

    //loop to call path() on every node in the graph that hasn't been visited
    while(!unvisited.empty()){
        start = nodes.top();
        nodes.pop();

        //only execute if start node isn't already in an scc
        if(visited[start]==0){

            //gets the longest path of start to itself
            stack<string> output = path(start);

            //vector of the scc of start
            vector<string> sub_scc;

            //convert the output scc into a vector
            //label every node in the scc as visited
            while(!output.empty()){
                string scc_node = output.top();
                sub_scc.push_back(scc_node);
                unvisited.erase(scc_node);
                visited[scc_node] = true;
                output.pop();
            }

            //pushes the scc onto the masterlist
            list_of_sccs.push_back(sub_scc);
        }
    }


    //list of all nodes in the graph; vector this time
    vector<string> nodes_vec;
    for(const auto& x: grph){
        nodes_vec.push_back(x.first);
    }

    //search through sccs for common nodes
    //iterates through the list of and finds every single longest path that it's in
    for(const auto& x: nodes_vec){

        //vector of pointers to the longest paths that x is on
        vector<vector<vector<string>>::iterator> found;

        //search every vector in the scc list for x
        for(auto scc = list_of_sccs.begin(); scc != list_of_sccs.end(); scc++){

            //if found, push the whole vector iterator onto found vector
            if(find(scc->begin(), scc->end(), x)!=scc->end()){
                found.push_back(scc);
            }
        }

        //if there are more than 1 paths with this node, the paths have to be unioned into one path
        while(found.size()>1){

            //keeps track of which nodes have been unioned so that there are no repeats
            visited.clear();
            for(const auto& z: grph){
                visited[z.first]= false;
            }

            // result vector of the union
            vector<string> unioned;

            // copies of the two vectors with x in them
            vector<string> one = *found[found.size()-1];
            vector<string> two = *found[found.size()-2];

            // push every node of the paths with x in them onto the unioned vector
            // keep track of what has been put on and ignore repeats
            for(auto y: one){
                if(!visited[y]){
                    visited[y] = true;
                    unioned.push_back(y);
                }
            }
            for(auto y: two){
                if(!visited[y]){
                    visited[y] = true;
                    unioned.push_back(y);
                }
            }

            //completely erase the original two paths that were just unioned from the
            //list of longest paths and from the found paths
            list_of_sccs.erase(found[found.size() - 1]);
            list_of_sccs.erase(found[found.size() - 2]);
            found.pop_back();
            found.pop_back();

            //put the new unioned vector onto the list of longest paths and onto the
            //list of found paths with x in them
            list_of_sccs.push_back(unioned);
            found.push_back(list_of_sccs.end() - 1);
        }
    }

    //after all paths have been unioned into scc's
    return list_of_sccs;
}
