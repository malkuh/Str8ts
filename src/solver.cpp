#include <iostream>
#include <chrono>
#include <ctime>

#include "Str8ts.hpp"

using namespace std;

int main(int argc, char * argv[])
{
    // loop over all input problems
    for (int j = 1; j < argc; j++) {

        string file = argv[j];

        cout << "Loading " << file << endl;

        Str8ts puzzle(file);

        if (puzzle.loaded()) {
            puzzle.print_problem();

            cout << "Solving the puzzle..." << endl;

            // time start
            chrono::time_point<chrono::system_clock> start, end;
            start = chrono::system_clock::now();

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

    return 0;
}