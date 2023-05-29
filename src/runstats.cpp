#include "runstats.hpp"

#include <iostream>
#include <iomanip>

Runstats * runstats = NULL;

Runstats::Runstats() {
    this->root = new Node("**MAIN**");
    this->current = this->root;
    runstats = this;
}

void Runstats::start(const std::string & name) {
    // Search in current's child for name
    Node * previous = NULL;
    for ( Node * child = this->current->first; ; child = child->next) {
        if ( child == NULL ) {
            // notfound
            child = new Node(name);
            child->parent = current;
            current->last = child;
            if ( previous ) {
                previous->next = child;
            } else {
                current->first = child;
            }
            current = child;
            
            break;
        }
        if ( child->name == name ) {
            // found
            child->count ++;
            current = child;
            break;
        }
        previous = child;
        
    }
    
}

void Runstats::stop(const int successes) {
    current->successes += successes;
    current = current->parent;
    
}
void Runstats::print_stats() {
    
    Node * stack[64];
    stack[0] = NULL;
    Node * node = this->root;
    
    for (int i_stack = 0;;) {
        if ( node == NULL) {
            node = stack[i_stack];
            i_stack --;
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
                << std::endl;
            i_stack ++;
            stack[i_stack] = node->next;
            node = node->first;
        }
        if (i_stack == 0 ) break;
    }
}
