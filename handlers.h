#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "digital_structs.h"
#include "parser.h"
#include <iostream>
#include <list>
#include <string>

using namespace rapidjson;
using namespace std;

// list of the parsers
list<parser *> parsers;

// the parser that will be pushed to the list for each iteration
parser *  curr_parser;

// the state that will be pushed for this iteration
state * curr_state;

// the transition that will be pushed to the list for this iteration
transition * curr_transit;

//will count the array starts and ends for the overall parser so we know when this
//entire section is complete
int arrcount_flag = 0;

//global variable for whether we are on the "Parsers" portion of the JSON
//and we should start putting things into structures
int in_parser_flag = 0;

// there will be more flags here that turn on as we pass imporant "keys"
// that mark places where we need to store data
int name_flag = 0;

int parse_states_flag = 0;

int state_flag = 0; //turn off after each transition_key to indicate next state

int transition_key_flag = 0;

int value_flag = 0;

int arr_flag = 0;

int transitions_flag = 0;

int next_state_flag= 0;

// sets all flags equal to 0
void reset_flags(){
	arrcount_flag = 0;
	in_parser_flag = 0;
	name_flag = 0;
	parse_states_flag = 0;
	state_flag = 0;
	transition_key_flag = 0;
	value_flag = 0;
	arr_flag = 0;
	transitions_flag = 0;
	next_state_flag = 0;
}


// maps the states in the transitions 
// necessary because, at first pass, all states have not been initialized in structs 
void second_pass(parser * _parser){

    list<parser*>::iterator parsers_it = parsers.begin();
    while(parsers_it!=parsers.end()){
            
        list<state*>::iterator states_it = (*parsers_it)->states.begin();
        while(states_it!=(*parsers_it)->states.end()){
            
			list<transition*>::iterator transit_it = (*states_it)->transitions.begin();
            while(transit_it != (*states_it)->transitions.end()){
    			
				string lookup_str = (*transit_it)->str_to_state;
				map<string, state*>::iterator map_it = (*parsers_it)->state_map.find(lookup_str);
				
				if(lookup_str.compare("\0")==0){
					;
				}
				else if(map_it == (*parsers_it)->state_map.end()){
					cout << "Invalid state found: " << lookup_str << endl;
				}
				else{
					(*transit_it)->to_state = map_it->second;
				}

                transit_it++;
            }   
                
            states_it++;
        }   
            
        parsers_it++;
    }   
}

struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
	bool Null() {
		//cout << "Null()" << endl;
	if (in_parser_flag == 1 && name_flag == 0 && parse_states_flag == 1 && state_flag == 0
              && transition_key_flag == 0 && value_flag == 0 && next_state_flag == 1 && transitions_flag == 1){
                  curr_transit->str_to_state = "\0";
				  curr_transit->to_state = NULL;
                  curr_transit->from_state = curr_state;
                  next_state_flag = 0;
	}

		return true;
	}
	bool Bool(bool b) {
		//cout << "Bool(" << boolalpha << b << ")" << endl;
		return true;
	}
	bool Int(int i) {
		//cout << "Int(" << i << ")" << endl;
		return true;
	}
	bool Uint(unsigned u) {
		//cout << "Uint(" << u << ")" << endl;
		return true;
	}
	bool Int64(int64_t i) {
		//cout << "Int64(" << i << ")" << endl;
		return true;
	}
	bool Uint64(uint64_t u) {
		//cout << "Uint64(" << u << ")" << endl;
		return true;
	}
	bool Double(double d) {
		//cout << "Double(" << d << ")" << endl;
		return true;
	}
	bool String(const char* str, SizeType length, bool copy) {
		//cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;

		// condition for if a new parser is beginning
		if(in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 0 && state_flag == 0 && transition_key_flag == 0 &&
		   value_flag == 0 && next_state_flag== 0){
			curr_parser = new parser;
			parsers.push_back(curr_parser);
			curr_parser->name = str;
			name_flag = 0;
		}
		// condition for grabbing the state name_flag
		else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 1 && state_flag == 0 && transition_key_flag == 0
			 && value_flag == 0 && next_state_flag== 0){
			// make a new state and put it in the curr_parser's list
			curr_state = new state;
			curr_state->name = str;
			
			cout << "curr state name" << curr_state->name << endl;

			curr_parser->add_state(curr_state);
		

			// reset flag
			name_flag = 0;
		}
		// pushing the array that represents the array of fields of the transition key
		else if (in_parser_flag == 1 && name_flag == 0 && transition_key_flag == 1 && value_flag == 1 && arr_flag == 1){
			// this is going to need to change because now
			// this only adds the value type to the last transition
			// need an example of a JSON if a transition depends on a different value
			curr_transit->add_value_type(str);
			
			// not resetting value_flag beecause we are pushing an array so we 
			// need to come back to this condition multiple times in a row 


		}	// condition for pushing the transition value to the list
		else if (in_parser_flag == 1 && name_flag == 0 && parse_states_flag == 1 && state_flag == 0
			 && transition_key_flag == 0 && value_flag == 1 && next_state_flag== 0 && transitions_flag == 1){
				curr_transit = new transition;
				curr_state->add_transit(curr_transit);
				curr_transit->value = str;
				curr_transit->from_state = curr_state;
				 // reset flag
				 value_flag = 0;
		}
		// condition for the next_state field of the transition
		else if (in_parser_flag == 1 && name_flag == 0 && parse_states_flag == 1 && state_flag == 0
			 && transition_key_flag == 0 && value_flag == 0 && next_state_flag == 1 && transitions_flag == 1){
				 curr_transit->str_to_state = str;
				 
				 cout << "str to state" << str << endl;
				 
				 curr_transit->from_state = curr_state;
				 next_state_flag = 0;
		}

		return true;
	}
	bool StartObject() {
		//cout << "StartObject()" << endl;
		return true;
	}
	bool Key(const char* str, SizeType length, bool copy) {
		//cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		//if the key is "parsers" -> set a flag and start parsing into digital logic classes
		
		//find the length of the key so we can 
		//make the char* to a string
		size_t str_length = 0;

		while(str[str_length] != '\0'){
			str_length++;
		}

		string string_str(str, str_length);
		
		if(string_str.compare("parsers") == 0){
			//turn flag on and parse things into classes
			in_parser_flag = 1;
		} else if ( in_parser_flag == 1 ){
			if(string_str.compare("name") == 0){
				name_flag = 1;
			}
			else if(string_str.compare("parse_states") == 0 ){
				parse_states_flag = 1;
			}
			else if(string_str.compare("transitions") == 0){
				transitions_flag = 1;
			}
			else if(string_str.compare("state") == 0){
				state_flag = 1;
			}
			else if(string_str.compare("transition_key") == 0){
				transitions_flag = 0;
				transition_key_flag = 1;
			}
			else if((transitions_flag == 1 || transition_key_flag == 1) && string_str.compare("value") == 0){
				value_flag = 1;
			}
			else if(string_str.compare("next_state") == 0){
				next_state_flag= 1;
			}
		}
		return true;
	}
	bool EndObject(SizeType memberCount) {
		//cout << "EndObject(" << memberCount << ")" << endl;
		return true;
	}
	bool StartArray() {
		//cout << "StartArray()" << endl;
		if(in_parser_flag == 1){
			arrcount_flag++;
		}

		if(transition_key_flag == 1){
			arr_flag = 1;
		}
		return true;
	}
	bool EndArray(SizeType elementCount) {
		//cout << "EndArray(" << elementCount << ")" << endl;
		if(in_parser_flag == 1){
			arrcount_flag--;
			if(arrcount_flag == 0){
				in_parser_flag = 0;
				//turn the rest of flags off too
				reset_flags();

				//call a function here that will instigate the second pass
				//to connect everything 
				second_pass(curr_parser);
			}
		}
		if(transition_key_flag == 1){
			arr_flag = 0;
			// turn off this flag because we are at the end of this section of the JSON
			transition_key_flag = 0;
			value_flag = 0;
		}

		return true;
	}
};
