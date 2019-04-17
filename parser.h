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

class transition{
  public:
    // "transitions"->"next_state"
    string to;
    // the value that triggers this next state
    string value; //from;
};

class state{
  public:
    string name; // parse_states -> name
    list<transition> transitions;
    // the value type that determines the transition
    // probably based off of "parser_ops"
    list<string> value;
};

class parser{
  public:
    string name; //first "name" after finding "parsers"
    list<state> states;
    map<string,state> state_map;
};
