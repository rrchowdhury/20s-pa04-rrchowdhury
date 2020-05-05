/*
 * student: Ramisa Chowdhury
 * email: rrchowdhury@smu.edu
 * ID: 45539186
 * course: CS3353 Algorithms
 * project 4: Strongly Connected Components
 * @desc A class to test all algorithms implemented in the project and analyse their times
 */

#include "Tester.h"

/*--------------------------------------------------------------------------
 * CONSTRUCTOR
 *
 * @desc assigns whole data files will be read in
 *      determines number of iterations to be averaged
 --------------------------------------------------------------------------*/
Tester::Tester() {
    iterations = 5;

    infile_proof.emplace_back("input01.txt"); //12
    infile_proof.emplace_back("input02.txt"); //9
    infile_proof.emplace_back("input03.txt"); //68

    infile_time.emplace_back("input04.txt"); //10
    infile_time.emplace_back("input05.txt"); //20
    infile_time.emplace_back("input06.txt"); //30
    infile_time.emplace_back("input07.txt"); //40
    infile_time.emplace_back("input08.txt"); //50

    infile_robust.emplace_back("input04.txt"); //10
    infile_robust.emplace_back("input13.txt"); //100
    infile_robust.emplace_back("input15.txt"); //1000
    infile_robust.emplace_back("input16.txt"); //10000
    infile_robust.emplace_back("input17.txt"); //10000
    infile_robust.emplace_back("input18.txt"); //10000

}

Tester::~Tester() = default;

/*
 * @param fout: ofstream object to be written out to
 *              opened in main
 * @desc: calls both the trivial and robust version on all the same datasets
 *      prints out the time that the trivial and robust algorithms took side by side for comparison
 */
void Tester::scc_time(std::ofstream & fout) {

    // variable to keep track of time
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    int elapsed;
    long time;

    fout << "SCC TIME ANALYSIS: Average of " << iterations << "\n" << std::endl;

    for(const std::string& file : infile_time) {

        Graph g(file);
        time = 0;

        //information to distinguish which file the output is coming from
        fout << "[" << file << "]" << std::endl;
        fout << "NODES:  \t" << g.get_num_elem() << std::endl;

        //Times algorithm to find strongly connected components
        //does [iteration] tests
        fout << "TRIVIAL:\t";
        for (int i = 0; i < iterations; i ++){
            start = chrono::steady_clock::now();
            g.scc_trivial();
            end = chrono::steady_clock::now();

            elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
            time += elapsed;
        }

        //average of all the tests
        time /= iterations;
        fout << time << " μs\n";

        //reset
        time = 0;

        //Times Kosajaru's strongly connected component algorithm
        //does [iteration] tests
        fout << "ROBUST: \t";
        for (int i = 0; i < iterations; i ++){
            start = chrono::steady_clock::now();
            g.scc();
            end = chrono::steady_clock::now();
            elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
            time += elapsed;
        }

        //average of all the tests
        time /= iterations;
        fout << time << " μs\n\n";
    }
}

/*
 * @param fout: ofstream object to be written out to
 *              opened in main
 * @desc: prints all the stronly connected components of each graph for both trivial and robust
 *        for proof on functionality
 */
void Tester::scc_proof(std::ofstream & fout) {
    for(const string&  file: infile_proof) {

        fout << "[" << file << "]" << endl << endl;
        Graph g(file);

        vector<vector<string>> trivial = g.scc_trivial();

        vector<vector<string>> robust = g.scc();


        for(auto & i : trivial){
            sort(i.begin(), i.end());
        }
        sort(trivial.begin(), trivial.end(), sort_by_string);
        sort(trivial.begin(), trivial.end(), sort_by_size);

        for(auto & i : robust){
            sort(i.begin(), i.end());
        }
        sort(robust.begin(), robust.end(), sort_by_string);
        sort(robust.begin(), robust.end(), sort_by_size);

        fout << "TRIVIAL:" << endl;

        for (int i = 0; i < trivial.size(); i++) {
            fout << "[" << i + 1 << "]:";
            for (const auto &j : trivial[i]) {
                fout << " " << j;
            }
            fout << endl;
        }

        fout << "\nROBUST:" << endl;

        for (int i = 0; i < robust.size(); i++) {
            fout << "[" << i + 1 << "]:";
            for (const auto &j : robust[i]) {
                fout << " " << j;
            }
            fout << endl;
        }
        fout << endl;
        fout << "---------------------------" << endl;
        fout << "---------------------------" << endl;
        fout  << endl;
    }

}

/*
 * @param fout: ofstream object to be written out to
 *              opened in main
 * @desc: since Kosajaru's algorithms runs in linear time, this one times it on graphs up to 100000 nodes
 *        the trivial solution takes a long time and can't be run with such big datasets
 */
void Tester::scc_robust(std::ofstream & fout) {

    // variable to keep track of time
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    int elapsed;
    long time;

    fout << "KOSAJARU'S ALGORITHM ANALYSIS: Average of " << iterations << "\n" << std::endl;

    for(const std::string& file : infile_robust) {

        Graph g(file);
        time = 0;

        //information to distinguish which file the output is coming from
        fout << "[" << file << "]" << std::endl;
        fout << "NODES:  \t" << g.get_num_elem() << std::endl;


        //Times Kosajaru's linear time algorithm
        //does [iteration] tests
        fout << "ROBUST: \t";
        for (int i = 0; i < iterations; i ++){
            start = chrono::steady_clock::now();
            g.scc();
            end = chrono::steady_clock::now();

            elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
            time += elapsed;
        }

        //average of all the tests
        time /= iterations;
        fout << time << " μs\n\n";
    }
}

/*
 * @desc: comparison function using by algorithm::sort()
 *        sorts a vector of vectors in ascending order by size
 */
bool Tester::sort_by_size(vector<string> a, vector<string> b) {
    return a.size() < b.size();
}

/*
 * @desc: comparison function using by algorithm::sort()
 *        sorts a vector of vectors in ascending alphabetical order
 *          based off the first element
 */
bool Tester::sort_by_string(vector<string> a, vector<string> b) {
    return a[0] < b[0];
}