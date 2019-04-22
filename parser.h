#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"

#include <string>
#include <iostream>
#include <map>
#include <list>

using namespace rapidjson;
using namespace std;

// defined so transition and state can use each other as members
struct state;

struct transition{
  public:
    //pointer to the state this transition is coming from
    state * from_state;
    // "transitions"->"next_state"
    // hold for the name of the next state
    // that will determine what will be put in
    // the pointer, since the next state hasn't
    // been created yet
    string str_to_state;
    state * to_state;
    // the value that triggers this next state
    string value; //from;

    // the value type that determines the transition
    // probably based off of "parser_ops"
    list<string> value_type;

    void add_value_type(string _val){
      value_type.push_back(_val);
    }

};

struct state{
  public:
    string name; // parse_states -> name
    list<transition* > transitions;

    void add_transit(transition * _transit){
      transitions.push_back(_transit);
    }

};

struct parser{
  public:
    string name; //first "name" after finding "parsers"
    list<state *> states;
    // maps all the states and the namespace
    // needed for use in second pass of parse 
    map<string,state *> state_map;

    void add_state(state * _state){
      states.push_back(_state);
      //map it also
	  state_map.insert(pair<string,state*>(_state->name, _state));

    }
};
