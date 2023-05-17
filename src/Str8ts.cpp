#include "Str8ts.hpp"

using namespace std;

Str8ts::Str8ts(string filename) : table(filename), map(table)
{
    ;
}

bool Str8ts::solve()
{
    return solve(&map);
}

bool Str8ts::solve(SMap* st_instance)
{
    if(!st_instance->solved()) {
        // apply rules until no changes occur
        st_instance->apply_rules();

        // check if it's violation free
        if (!st_instance->violation_free()) {
            return false;
        }

        // if not solved yet call this function recursively with guessed numbers
        if(!st_instance->solved()) {
            // returns list with differently guessed numbers
            list<SMap*> sub_t_instances = st_instance->choose();
            bool solved = false;
            for (auto st_inst : sub_t_instances) {
                if (solve(st_inst)) {
                    solved = true;
                    if (!solve_extensively)
                        break;
                }
            }
            // free resources
            for (auto st_inst : sub_t_instances) {
                delete st_inst;
            }
            return solved;
        }
        else {
            // saves a solution table to the list of solutions
            solution_tables.push_back(st_instance->get_table());
            return true;
        }
    }
    else {
        return false;
    }
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
