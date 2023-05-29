//
//  runstats.hpp
//  Str8tsSolver
//

#ifndef runstats_hpp
#define runstats_hpp

#include <stdio.h>
#include <string>

struct Node {
    std::string name;
    Node * first = 0;
    Node * last = 0;
    Node * next = 0;
    Node * parent = 0;
    int count = 1;
    int successes = 0;
    Node(std::string name) {
        this->name = name;
    };
    
};

class Runstats {
private:
    Node * root;
    Node * current;
public:
    Runstats();
    void start(const std::string &);
    void stop(const int successes);
    void print_stats();
};

struct StartStop {
    StartStop(const std::string name) {
        extern Runstats * runstats;
        runstats->start(name);
    }
    ~StartStop() {
        // This code will run when object goes out of scope
        extern Runstats * runstats;
        runstats->stop(0);
    }
};

#endif /* runstats_hpp */
