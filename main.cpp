/*
 * student: Ramisa Chowdhury
 * email: rrchowdhury@smu.edu
 * ID: 45539186
 * course: CS3353 Algorithms
 * project 4: Strongly Connected Components
 * @desc A program that finds strongly connected compnents with a trivial and robust (Kosajaru's) algorithm
 *      tests the functioning and the timing
 */

#include <string>
#include "Tester.h"
using namespace std;

int main(int argc, char** argv) {
    Tester t;
    ofstream fout_proof("out_proof.txt");
    if(fout_proof.is_open()){
        t.scc_proof(fout_proof);
        fout_proof.close();
    }

    ofstream fout_time("out_time.txt");
    if(fout_time.is_open()){
        t.scc_time(fout_time);
        fout_time.close();
    }

    ofstream fout_robust("out_robust.txt");
    if(fout_robust.is_open()){
        t.scc_robust(fout_robust);
        fout_robust.close();
    }

    return 0;
}