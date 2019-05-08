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
	parse_digital(handler.get_parsers());

	return 0;
}
