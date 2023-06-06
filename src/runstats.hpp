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
    Node * next = 0;
    Node * parent = 0;
    int count = 1;
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
    static void start(const std::string &);
    static void stop(void);
    static void print_stats();
};

struct StartStop {
    StartStop(const std::string name) {
        Runstats::start(name);
    }
    ~StartStop() {
        // This code will run when object goes out of scope
        Runstats::stop();
    }
};

#endif /* runstats_hpp */
