#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "digital_structs.h"
#include "parser.h"
#include <iostream>
#include <list>
#include <string>

using name_flagspace rapidjson;
using name_flagspace std;

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
	transition_flag = 0;
	next_state_flag = 0;
}


struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
	bool Null() {
		//cout << "Null()" << endl;
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
		if(in_parser_flag == 1 && name_flag == 0 && parse_states_flag == 0 && state_flag == 0 && transition_key_flag == 0 &&
		   value_flag == 0 && next_state_flag== 0){
			curr_parser = new parser;
			parsers.push_back(curr_parser);
		}
		// condition for the name of the parser
		else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 0 && state_flag == 0 && transition_key_flag == 0
			 && value_flag == 0 && next_state_flag== 0){
			curr_parser->name = str;

			// reset flag
			name_flag = 0;
		}
		// condition for grabbing the state name_flag
		else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 1 && state_flag == 0 && transition_key_flag == 0
			 && value_flag == 0 && next_state_flag== 0){
			// make a new state and put it in the curr_parser's list
			curr_state = new state;
			curr_parser->add_state(curr_state);
			curr_state->name = str;

			// reset flag
			name_flag = 0;
		}
		// pushing the array that represents the array of fields of the transition key
		else if (in_parser_flag == 1 && name_flag == 1 && transition_key_flag == 1 && value_flag == 1 && arr_flag == 1){
			curr_transit->add_value_type(str);
			//curr_state->value.push_back(str);
		}
		// condition for pushing the transition value to the list
		else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 1 && state_flag == 0
			 && transition_key_flag == 0 && value_flag == 1 && next_state_flag== 0){
				 curr_transit->value = str;

				 // reset flag
				 value_flag = 0;
		}
		// condition for the next_state field of the transition
		else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 0 && state_flag == 0
			 && transition_key_flag == 0 && value_flag == 0 && next_state_flag == 1){
				 curr_transit->str_to_state = str;
				 curr_transit->from_state = curr_state;
		}
		// // conditions for
		// else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 0 && state_flag == 0
		// 	 && transition_key_flag == 0 && value_flag == 0 && next_state_flag== 0){
		//
		// }
		// else if (in_parser_flag == 1 && name_flag == 1 && parse_states_flag == 0 && state_flag == 0
		// 	 && transition_key_flag == 0 && value_flag == 0 && next_state_flag== 0){
		//
		// }

		return true;
	}
	bool StartObject() {
		//cout << "StartObject()" << endl;
		return true;
	}
	bool Key(const char* str, SizeType length, bool copy) {
		//cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		//if the key is "parsers" -> set a flag and start parsing into digital logic classes
		if(str == "parsers"){
			//turn flag on and parse things into classes
			in_parser_flag = 1;
		} else if ( in_parser_flag == 1 ){
			if(str == "name"){
				name_flag = 1;
			}
			else if(str == "parse_states"){
				parse_states_flag = 1;
			}
			else if(str == "state"){
				state_flag = 1;
			}
			else if(str == "transition_key"){
				transition_key_flag = 1;
			}
			else if(str == "value"){
				value_flag = 1;
			}
			else if(str == "next_state"){
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
			}
		}
		if(transition_key_flag == 1){
			arr_flag = 0;
			// turn off this flag because we are at the end of this section of the JSON
			transition_key_flag = 0;
		}

		return true;
	}
};
