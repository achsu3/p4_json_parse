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
    string to;
    string from;
};

class state{
  public:
    string name;
    list<transition> transitions;
};

class parser{
  public:
    string name;
    list<state> states;
    
	map<string,state> state_map;
};
