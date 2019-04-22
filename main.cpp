#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "handlers.h"
//#include "test.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

int main(){
	ifstream inFile ("basic.json");
	if(!inFile){
		printf("/n couldn't open file /n");
		return -1;
	}
	printf("\n opened file \n");

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

//	test_parser(parsers);

// putting logic for testing here because i couldn't get
// anything to compile :-/ 
// but basically this just prints everything that was parsed into the structs 
	list<parser*>::iterator parsers_it = parsers.begin();
    while(parsers_it!=parsers.end()){
    	cout<< "Parser name:" << (*parsers_it)->name << endl;
		
        list<state*>::iterator states_it = (*parsers_it)->states.begin();
        while(states_it!=(*parsers_it)->states.end()){
    		cout<< "State name: " << (*states_it)->name << endl;
			
            list<transition*>::iterator transit_it = (*states_it)->transitions.begin();
            while(transit_it != (*states_it)->transitions.end()){
    			cout<<"Transition from_state: " << (*transit_it)->from_state->name << endl;
				cout<<"Transition to_state: " << (*transit_it)->from_state->name << endl;
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

	return 0;
}
