//
//  solver.cpp
//  Str8tsSolver
//

#include <iostream>
#include <chrono>
#include <ctime>

#include "Str8ts.hpp"
#include "runstats.hpp"
#include "helpme.hpp"
#include "verbose.hpp"

using namespace std;

bool opt_trace = false;
bool opt_debug = false;
bool opt_all = false;

int main(int argc, char * argv[])
{
    
    // loop over all input problems
    for (int j = 1; j < argc; j++) {

        const string file = argv[j];
        
        if (file == "-v") {
            Verbose::on = true;
            continue;
        }
        if (file == "-t") {
            opt_trace = true;
            continue;
        }
        if (file == "-d") {
            opt_debug = true;
            continue;
        }
        if (file == "-a") {
            opt_all = true;
            continue;
        }

        if (file != "-i" and file[0] == '-' and file.size() > 1) {
            if (file != "-h" ) cerr << "option is invalid: " << file << '\n';
            givehelp();
            return 1;
        }
        
        cout << "Loading " << file << endl;

        Str8ts puzzle(file);

        if (puzzle.loaded()) {
            puzzle.print_problem();

            cout << "Solving the puzzle..." << endl;

            auto start = chrono::system_clock::now();

            puzzle.solve();

            puzzle.print_solutions();

            // time end
            auto end = chrono::system_clock::now();
            
            const string endmsg =
                puzzle.get_num_solutions() ?
                "Solved in " :
                "No solution found in "
            ;
            chrono::duration<double> elapsed_seconds = end - start;
            cout << endmsg << elapsed_seconds.count() << " sec.\n";
        }
        else {
            cout << "Couldn't load problem.\n";
        }
    }
    if ( opt_trace ) {
        Runstats::print_stats();
    }

    return 0;
}

