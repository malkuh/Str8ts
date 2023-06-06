#include "Str8ts.hpp"
#include "runstats.hpp"
#include "verbose.hpp"

using namespace std;

Str8ts::Str8ts(string filename) : table(filename), map(table)
{
    extern bool opt_all;
    solve_extensively = opt_all;
    ;
}

bool Str8ts::solve()
{
    return solve(&map);
}

bool Str8ts::solve(SMap* st_instance)
{
    StartStop x = StartStop("Str8ts::solve");
    
    if(!st_instance->solved()) {
        // apply rules until no changes occur
        // returns true if it's violation free
        const bool no_violation = st_instance->apply_rules();

        if (!no_violation) {
            StartStop x = StartStop("violation");
            return false;
        }

        // if not solved yet call this function recursively with guessed numbers
        if(!st_instance->solved()) {
            extern bool opt_verbose;
            SMap * st_inst_save = NULL;
            
            if (opt_verbose) {
                st_instance->print();
                st_inst_save = new SMap(* st_instance);
                cout << "\nGuessing..." << endl;
            }
            // returns list with differently guessed numbers
            list<SMap*> sub_t_instances = st_instance->choose();
            bool solved = false;
            for (auto st_inst : sub_t_instances) {
                if (opt_verbose) {
                    string guess = " trying ";
                    for (int i = 0; i < 81; i++) {
                        
                        if (st_inst->fields[i] == st_inst_save->fields[i]) continue;
                        guess += '0' + ffs(st_inst->fields[i]);
                        guess += " at " + coordinates(i);
                        cout << guess << endl;
                        break;
                    }
                }
                if (solve(st_inst)) {
                    solved = true;
                    if (!solve_extensively)
                        break;
                    if ( solution_tables.size() > 16 ) {
                        break;
                    }
                }
            }
            // free resources
            for (auto st_inst : sub_t_instances) {
                delete st_inst;
            }
            if (st_inst_save) delete st_inst_save;
            return solved;
        }
        else {
            // saves a solution table to the list of solutions
            solution_tables.push_back(st_instance->get_table());
            return true;
        }
    }
    return false;
}

/*
 * Printing puzzle and solutions
 */

void Str8ts::print_problem()
{
    table.print();
}

void Str8ts::print_solutions()
{
    int count = 1;
    for (auto solution : solution_tables) {
        cout << "Solution " << count << ":" << endl;
        solution.print();
        count++;
    }
}
