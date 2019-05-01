#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"

#include "parser.h"

#include <string>
#include <iostream>
#include <map>
#include <list>
 
using namespace rapidjson;
using namespace std;

#ifndef _DIGITAL_H
#define _DIGITAL_H 

#define CTRL_FLOW	0
#define STATE		1
#define MUX			2
#define AND			3
#define NOT			4
#define NOR			5
#define XOR			6
#define XNOR		7

typedef struct Digital{
	public:
		list<Digital*> input;
		Digital* output;
		int struct_type;
		// the state if the type is a state
		state * _state;
} Digital_t;

// ugh i know it's bad to put this in the header but 
// i can't spend any more time debugging redefinition issues 
list<Digital_t *> parse_digital(){
	// a list of every struct
	list<Digital_t*> digital_structs;

	//initialize the control_flow 
	Digital_t * control_flow = new Digital_t;
	digital_structs.push_back(control_flow);
	control_flow->struct_type = CTRL_FLOW;
	control_flow->_state = NULL;

	list<parser*>::iterator parsers_it = parsers.begin();
    while(parsers_it!=parsers.end()){
        //cout<< "Parser name:" << (*parsers_it)->name << endl;
    
        list<state*>::iterator states_it = (*parsers_it)->states.begin();
        while(states_it!=(*parsers_it)->states.end()){
            //cout<< "State name: " << (*states_it)->name << endl;
   			Digital_t * curr_state = new Digital_t;
			digital_structs.push_back(curr_state);
			curr_state->type = STATE;
			curr_state->_state = *states_it;

            list<transition*>::iterator transit_it = (*states_it)->transitions.begin();
            while(transit_it != (*states_it)->transitions.end()){
                cout<<"Transition from_state: " << (*transit_it)->from_state->name << endl;
                if((*transit_it)->to_state == NULL){
                    cout<<"Transition to_state: NULL" << endl;
                }   
                else cout<<"Transition to_state: " << (*transit_it)->to_state->name << endl;
                cout<<"Transition value_type: " << endl;
                // print all the value_type elements
                list<string>::iterator value_t_it = (*transit_it)->value_type.begin();
                while(value_t_it!=(*transit_it)->value_type.end()){
                    cout << *value_t_it<<endl;
                    value_t_it++;
                }   
                cout<<"Transition value: " << (*transit_it)->value << endl;

                transit_it++;
            }   
    
            states_it++;
        }   
    
        parsers_it++;
    }   

	return digital_structs;
}


#endif /* _DIGITAL_H */

