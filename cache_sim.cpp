//main file
#include "CacheConfig.h"
#include "CacheSimulator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <config_file> <trace_file>" << endl;
        return 1;
    }

    srand(time(NULL)); //seed the random number generator for random replacement policy

    CacheConfig config = parseConfig(argv[1]);//parse configuration

    CacheSimulator simulator(config);//create the simulator

    
    processTrace(simulator, argv[2]);//process trace file

    simulator.printStats();// Print statistics

    return 0;
}
