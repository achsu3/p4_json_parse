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

typedef struct _transition{
  public:
    // "transitions"->"next_state"
    state * to_state;
    // the value that triggers this next state
    string * value; //from;

    // the value type that determines the transition
    // probably based off of "parser_ops"
    list<string> value_type;

    void add_value_type(string _val){
      value_type.push_back(_val);
    }

} transition;

typedef struct _state{
  public:
    string name; // parse_states -> name
    list<transition* > transitions;

    void add_transit(transition * _transit){
      transitions.push_back(_transit);
    }

} state;

typedef struct _parser{
  public:
    string name; //first "name" after finding "parsers"
    list<state *> states;
    map<string,state *> state_map;

    void add_state(state * _state){
      states.push_back(_state);
      //map it also

    }
} parser;
