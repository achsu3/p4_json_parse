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
list<parser*> parsers;

// the parser that will be pushed to the list for each iteration
parser *  curr_parser

// the state that will be pushed for this iteration
state * curr_state;

// the transition that will be pushed to the list for this iteration
transition * curr_transit;


//will count the array starts and ends for the overall parser so we know when this
//entire section is complete 
int arrcount = 0;

//global variable for whether we are on the "Parsers" portion of the JSON
//and we should start putting things into structures
int parsers = 0;

// there will be more flags here that turn on as we pass imporant "keys"
// that mark places where we need to store data
int name = 0;

int parse_states = 0;

int state = 0; //turn off after each transition_key to indicate next state

int transition_key = 0;

int value = 0;

int arr = 0;

int transitions = 0;

int next_state = 0;

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
				if(parsers == 1 && name == 0 && parse_states == 0 && state == 0 && transition_key == 0 &&
					value == 0 && next_state == 0){
							curr_parser = new parser();
							parsers.push_back(curr_parser)
				}
				// condition for grabbing the name of the parser
				else if (parsers == 1 && name == 1 && parse_states == 0 && state == 0 && transition_key == 0
					&& value == 0 && next_state == 0){
						curr_parser->name = str;
				}
				// condition for grabbing the state name
				else if (parsers == 1 && name == 1 && parse_states == 1 && state == 0 && transition_key == 0
					&& value == 0 && next_state == 0){
						// make a new state and put it in the curr_parser's list
						curr_state = new state();
						parsers->states.push_back(curr_state);
						curr_state->name = str;
				}
				// pushing the array that represents the array of fields of the transition key
				else if (parsers == 1 && name == 1 && transition_key == 1 && value == 1 && arr == 1){
					curr_state->value.push_back(str);
				}
				// condition for pushing the transition value to the list
				else if (parsers == 1 && name == 1 && parse_states == 0 && state == 0 && transition_key == 0
					&& value == 0 && next_state == 0){

				}
				else if (parsers == 1 && name == 1 && parse_states == 0 && state == 0 && transition_key == 0
					&& value == 0 && next_state == 0){

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
        if(str == "parsers"){
          //turn flag on and parse things into classes
          parsers = 1;
        }
				else if(parsers == 1){
					else if(str == "name"){
						name = 1;
					}
					else if(str == "parse_states"){
						parse_states = 1;
					}
					else if(str == "state"){
						state = 1;
					}
					else if(str == "transition_key"){
						transition_key = 1;
					}
					else if(str == "value"){
						value = 1;
					}
					else if(str == "next_state"){
						next_state = 1;
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
      if(parsers == 1){
	  		arrcount++;
	  	}

			if(transition_key == 1){
				arr = 1;
			}
	  	return true;
    }
    bool EndArray(SizeType elementCount) {
      //cout << "EndArray(" << elementCount << ")" << endl;
      if(parsers == 1){
	  		arrcount--;
				if(arrcount == 0){
					parsers = 0;

					//turn the rest of flags off too 

				}
	  	}
			if(transition_key == 1){
				arr = 0;
			}

	  return true;
    }
};
