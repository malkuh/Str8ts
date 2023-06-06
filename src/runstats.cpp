#include "runstats.hpp"

#include <iostream>
#include <iomanip>

namespace {
// Runstats soll auf diese Weise ein Singleton sein
Runstats runstats;
}

Runstats::Runstats() {
    this->root = new Node("**MAIN**");
    this->current = this->root;
}

void Runstats::start(const std::string & name) {
    // Search in current's child for name
    Node * previous = NULL;
    for ( Node * child = runstats.current->first; true ; child = child->next) {
        if ( child == NULL ) {
            // notfound
            child = new Node(name);
            child->parent = runstats.current;
            if ( previous ) {
                previous->next = child;
            } else {
                runstats.current->first = child;
            }
            runstats.current = child;
            
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
    stack[0] = NULL;
    Node * node = runstats.root;
    
    for (int i_stack = 0;;) {
        if ( node == NULL) {
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
            stack[i_stack] = node->next;
            node = node->first;
        }
        if (i_stack == 0 ) break;
    }
}
