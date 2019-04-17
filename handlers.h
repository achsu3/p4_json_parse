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
list<parser> parsers;

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

int startArray = 0;

int endArray = 0;

int next_state;

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
				if(parsers == 1 && name == 0 && parse_states == 0 && state == 0 && transition_key == 0 &&
					value == 0 && next_state == 0){
							parser *  curr_parser = new parser();
							parsers.push_back(curr_parser)
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
          parser = 1;
        }
				else if(parser == 1){
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
      if(parser == 1){
	  		arrcount++;
	  	}
	  	return true;
    }
    bool EndArray(SizeType elementCount) {
      //cout << "EndArray(" << elementCount << ")" << endl;
      if(parser == 1){
	  		arrcount--;
				if(arrcount == 0){
					parser = 0;
				}
	  	}

	  return true;
    }
};
