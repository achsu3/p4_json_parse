#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "handlers.h"
#include "digital.h"

using namespace std;
using namespace rapidjson;

int main() {
	ifstream inFile ("basic.json");
	if(!inFile){
		printf("/n couldn't open file /n");
		return -1;
	}

	MyHandler handler;
	Reader reader;

	//parse file normally and put into an array, test handler on it
	char _json[8000];
	int i=0;
	while(!inFile.eof()&&i<8000){
		_json[i] = inFile.get();
		//std::cout<<_json[i]<<std::endl;
		i++;
	}

	StringStream s2(_json);
	reader.Parse(s2, handler);

	// putting logic for testing here because i couldn't get
	// anything to compile :-/
	// but basically this just prints everything that was parsed into the structs
	list<Parser *>& parsers = handler.get_parsers();
	list<Parser*>::iterator parsers_it = parsers.begin();
	while(parsers_it!=parsers.end()) {
		cout<< "Parser name:" << (*parsers_it)->get_name() << endl;

		list<State*>::iterator states_it = (*parsers_it)->get_states().begin();
		while(states_it!=(*parsers_it)->get_states().end()) {
			cout<< "State name: " << (*states_it)->get_name() << endl;

			list<Transition*>::iterator transit_it =
				(*states_it)->get_transitions().begin();
			while (transit_it != (*states_it)->get_transitions().end()){
				cout<<"Transition from_state: " <<
					(*transit_it)->from_state->get_name() << endl;

				if ((*transit_it)->to_state == NULL){
					cout<<"Transition to_state: NULL" << endl;
				} else
					cout<<"Transition to_state: " <<
						(*transit_it)->to_state->get_name() << endl;

				cout<<"Transition value_type: " << endl;

				// print all the value_type elements
				list<string>::iterator value_t_it = (*transit_it)->value_type.begin();
				while(value_t_it!=(*transit_it)->value_type.end()) {
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

	// now translate to digital things
	list<Circuit*> circuit_components = parse_digital(handler.get_parsers());

	// test for the digital version by traversing the components pushed
	list<Circuit*>::iterator components_it = circuit_components.begin();
	while(components_it != circuit_components.end()){
		// print all the things this is attached to and the wires
		enum COMPONENT_TYPE type = (*components_it)->getType();
		if(type == OOPS){
			cout<< "something went wrong"<< endl;
		}
		else if(type == MUX){
			cout<< "Mux:"<< endl;
			Mux * mux = (Mux*) (*components_it);
			cout<< "Inputs:"<< endl;
			for(int i = 0; i < mux->inputs.size(); i++){
				for(int j = 0; j<mux->inputs[i]->to.size(); j++){
					cout << print_type(mux->inputs[i]->to[j]->getType())<<endl;
				}
			}
			cout<<"Select Input: "<<endl;
			for(int j = 0; j<mux->select_input->to.size(); j++){
				cout << print_type(mux->select_input->to[i]->getType())<<endl;
			}
		}
		else if(type == AND){
			cout<< "And:"<< endl;
		}
		else if(type == NOT){
			cout<< "Not:"<< endl;
		}
		else if(type == NOR){
			cout<< "Nor:"<< endl;
		}
		else if(type == XOR){
			cout<< "Xor:"<< endl;
		}
		else if(type == XNOR){
			cout<< "Xnor:"<< endl;
		}
		else if(type == CONSTANT_VALUE){
			cout<< "Constant_value:"<< endl;

			Constant_Value * constand_val = (Constant_Value*)(*components_it);
			cout<< "Value: "<< (constand_val) ->value<<endl;

			// show what this is connected to
			for(int i = 0; i < constand_val->output->to.size(); i++){
				cout<< "Output wire connected to: "<< print_type(constand_val->output->to[i]->getType())<<endl;
			}
		}
		else if(type == CONTROL_FLOW){
			cout<< "Control_flow:"<< endl;
		}

		components_it++;
	}

	return 0;
}
