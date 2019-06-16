#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "p4aig/p4aig.h"

using namespace p4aig;
using namespace std;

int main() {
	P4AigParser *parser = new P4AigParser("basic.json");
	list<P4Parser *>& p4parsers = parser->parse_p4_program();

	list<P4Parser*>::iterator parsers_it = p4parsers.begin();
	cout << "Parsed " << p4parsers.size() << " P4 parsers." << endl;
	while(parsers_it!=p4parsers.end()) {
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
#if 0
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
	list<P4Parser *>& parsers = handler.get_parsers();

	// now translate to digital things
	list<CComponent *> circuit_components; // = parse_digital(handler.get_parsers());

	// test for the digital version by traversing the components pushed
	list<CComponent *>::iterator components_it = circuit_components.begin();
	while(components_it != circuit_components.end()){
		// print all the things this is attached to and the wires
		ostringstream os;
		(*components_it)->to_str(os);
		std::cout << os.str();
		components_it++;
	}

	return 0;
#endif 
}
