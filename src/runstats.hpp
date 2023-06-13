//
//  runstats.hpp
//  Str8tsSolver
//

#ifndef runstats_hpp
#define runstats_hpp

#include <string>

extern bool opt_trace;

struct Node {
    friend class Runstats;
    Node(std::string name) : name(name) { };
private:
    std::string name;
    Node * first = 0;
    Node * next = 0;
    Node * parent = 0;
    int count = 1;
    
    ~Node();
    // No copy constructor nor assignment operator
    Node & operator=(const Node &);
    Node(const Node &);
    
    
};

class Runstats {
private:
    Node  root = Node("**main**");
    Node * current = & root;
public:
    static void start(const std::string &);
    static void stop(void);
    static void print_stats();
};

struct StartStop {
    StartStop(const std::string name) {
        if (opt_trace) Runstats::start(name);
    }
    ~StartStop() {
        // This code will run when object goes out of scope
        if (opt_trace) Runstats::stop();
    }
};

#endif /* runstats_hpp */
