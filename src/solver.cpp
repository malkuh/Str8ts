#include <iostream>
#include <chrono>
#include <ctime>

#include "Str8ts.hpp"
#include "Stats.hpp"
#include "runstats.hpp"

using namespace std;

bool opt_verbose = false;
bool opt_trace = false;

int main(int argc, char * argv[])
{
    Runstats * runstats = new Runstats();
    
    
    // loop over all input problems
    for (int j = 1; j < argc; j++) {
        string name = "file" + to_string(j);

        string file = argv[j];
        
        if (file == "-v") {
            opt_verbose = true;
            continue;
        }
        if (file == "-t") {
            opt_trace = true;
            continue;
        }

        cout << "Loading " << file << endl;

        Str8ts puzzle(file);

        if (puzzle.loaded()) {
            puzzle.print_problem();

            cout << "Solving the puzzle..." << endl;

            // time start
            chrono::time_point<chrono::system_clock> start, end;
            start = chrono::system_clock::now();

            // to find all solutions in a possibly not unique puzzle uncomment the following:
//             puzzle.solve_ext(true);
            puzzle.solve();

            puzzle.print_solutions();

            // time end
            end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            cout << "Solved in " << elapsed_seconds.count() << " sec.\n";
        }
        else {
            cout << "Couldn't load problem." << endl;
        }
    }
    if ( opt_trace ) {
        runstats->print_stats();
    }

    return 0;
}

