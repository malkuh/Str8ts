#include "runstats.hpp"

#include <iostream>
#include <iomanip>

extern bool opt_debug;
namespace {
// Runstats soll auf diese Weise ein Singleton sein
Runstats runstats;
}

Node::~Node() {
    if (opt_debug) {
        std::cerr << "deleting Node at " << this << " - " << name  << '\n';
    }
}


void Runstats::start(const std::string & name) {
    // Search in current's child for name
    Node * previous = nullptr;
    for ( auto child = runstats.current->first.get();
         true ; child = child->next.get()) {
        if ( !child ) {
            // notfound
            std::unique_ptr<Node> new_child =
                    std::make_unique<Node>( name, runstats.current);
            auto new_current = new_child.get();
            if ( previous ) {
                previous->next = std::move(new_child);
            } else {
                runstats.current->first = std::move(new_child);
            }
            runstats.current = new_current;
            
            break;
        }
        if ( child->name == name ) {
            // found
            child->count += 1;
            runstats.current = child;
            break;
        }
        previous = child;
        
    }
    
}

void Runstats::stop() {
    runstats.current = runstats.current->parent;
    
}
void Runstats::print_stats() {
    
    Node * stack[64];
    stack[0] = nullptr;
    Node * node = & runstats.root;
    
    for (int i_stack = 0;;) {
        if ( !node ) {
            node = stack[i_stack];
            i_stack -= 1;
        } else {
            std::string indent = "";
            indent.resize(
                i_stack > 0 ? 2 * i_stack - 1 : 0, '.'
            );
            std::string name = indent + " " + node->name + " ";
            name.resize(64, '.');
            std::cout
                << name
                << std::setw(7) << node->count
                << '\n';
            i_stack += 1;
            stack[i_stack] = node->next.get();
            node = node->first.get();
        }
        if (i_stack == 0 ) break;
    }
   
}
