#ifndef _DIGITAL_H
#define _DIGITAL_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "parser.h"

using namespace rapidjson;
using namespace std;

// types of gates
enum COMPONENT_TYPE{OOPS, MUX, AND, NOT, NOR, XOR,
  XNOR, CONSTANT_VALUE, CONTROL_FLOW};

class Wire;

// base class for all hardware components
class Circuit{
public:
  COMPONENT_TYPE type;
  virtual COMPONENT_TYPE getType() = 0;
};

class Constant_Value: public Circuit{
public:
  Constant_Value(){
    value = "";
    output = nullptr;
  };
  Constant_Value(string _value, Wire * _output){
    output = _output;
    value = _value;
  };
  string value;
  Wire * output;

  COMPONENT_TYPE getType(){
    return CONSTANT_VALUE;
  };
};

// control flow
class Control_Flow: public Circuit{
public:
  Control_Flow(){
    return;
  }
  vector<Wire*> inputs;
  vector<Wire*> outputs;

  void addInput(Wire* _wire){
    inputs.push_back(_wire);
  }

  void addOutput(Wire* _wire){
    outputs.push_back(_wire);
  }
  COMPONENT_TYPE getType(){
    return CONTROL_FLOW;
  };
};

class Mux: public Circuit{
public:
  Mux(){
    select_input = NULL;
    output = NULL;
  };
  vector<Wire*> inputs;
  Wire * select_input;
  Wire * output;

  void addInput(Wire* _wire){
    inputs.push_back(_wire);
  }
  COMPONENT_TYPE getType(){
    return MUX;
  };

};

class Gate: public Circuit{
public:
  Wire* input1;
  Wire* input2;
  Wire* output;
  COMPONENT_TYPE getType(){
    return OOPS;
  };
};

class And: public Gate{
public:
  And(){
    input1 = NULL;
    input2 = NULL;
    output = NULL;
  }
  And(Wire* _input1, Wire* _input2){
    input1 = _input1;
    input2 = _input2;
  };
  COMPONENT_TYPE getType(){
    return AND;
  }
};

class Not: public Gate{
public:
  Not(){
    input1 = NULL;
    input2 = NULL;
    output = NULL;
  }
  Not(Wire* _input1, Wire* _input2){
    input1 = _input1;
    input2 = _input2;
  };
  COMPONENT_TYPE getType(){
    return NOT;
  }
};

class Nor: public Gate{
public:
  Nor(){
    input1 = NULL;
    input2 = NULL;
    output = NULL;
  }
  Nor(Wire* _input1, Wire* _input2){
    input1 = _input1;
    input2 = _input2;
  };
  COMPONENT_TYPE getType(){
    return NOR;
  }
};

class Xor: public Gate{
public:
  Xor(){
    input1 = NULL;
    input2 = NULL;
    output = NULL;
  }
  Xor(Wire* _input1, Wire* _input2){
    input1 = _input1;
    input2 = _input2;
  };
  COMPONENT_TYPE getType(){
    return XOR;
  }
};

class Xnor: public Gate{
public:
  Xnor(){
    input1 = NULL;
    input2 = NULL;
    output = NULL;
  }
  Xnor(Wire* _input1, Wire* _input2){
    input1 = _input1;
    input2 = _input2;
  };
  COMPONENT_TYPE getType(){
    return XNOR;
  }
};

// for connecting different components
class Wire{
public:
  Wire(){
    from = NULL;
  };
  Wire(Circuit * _to, Circuit * _from){
    to.push_back(_to);
    from = _from;
  }
  vector<Circuit *> to;
  Circuit * from;

  void add_to(Circuit * _to){
    to.push_back(_to);
  }
};

// actual function that parses the FSM into digital structs
list<Circuit*> parse_digital(list<Parser *>& parsers);
string print_type(enum COMPONENT_TYPE type);

#endif /* _DIGITAL_H */
