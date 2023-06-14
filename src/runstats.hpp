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
    Node(std::string name, Node * parent) : name(name), parent(parent) { };
    ~Node();
private:
    std::string name;
    std::unique_ptr<Node> first = nullptr;
    std::unique_ptr<Node> next = nullptr;
    Node * parent;
    int count = 1;
    
};

class Runstats {
private:
    Node  root = Node("**main**", nullptr);
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
