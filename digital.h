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
enum GATE_TYPE{OOPS, MUX, AND, NOT, NOR, XOR, XNOR};

class Wire;

// base class for all hardware components
class Circuit{
public:
  GATE_TYPE type;
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
};

class Mux: public Circuit{
public:
  Mux();
  vector<Wire*> inputs;
  Wire * select_input;
  Wire * output;

  void addInput(Wire* _wire){
    inputs.push_back(_wire);
  }

};

class Gate: public Circuit{
public:
  Wire* input1;
  Wire* input2;
  Wire* output;
  GATE_TYPE getType(){
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
  GATE_TYPE getType(){
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
  GATE_TYPE getType(){
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
  GATE_TYPE getType(){
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
  GATE_TYPE getType(){
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
  GATE_TYPE getType(){
    return XNOR;
  }
};

// for connecting different components
class Wire{
public:
  Wire();
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
void parse_digital(list<Parser *>& parsers);

#endif /* _DIGITAL_H */

/*
#define CTRL_FLOW	0
#define STATE		1
#define MUX			2
#define AND			3
#define NOT			4
#define NOR			5
#define XOR			6
#define XNOR		7*/

/*
typedef struct Digital{
	public:
		list<Digital*> input;
		Digital* output;
		int struct_type;
		// the state if the type is a state
		State * _state;
} Digital_t;

// ugh i know it's bad to put this in the header but
// i can't spend any more time debugging redefinition issues
list<Digital_t *> parse_digital(){
	// a list of every struct
	list<Digital_t*> digital_structs;

	//initialize the control_flow
	Digital_t * control_flow = new Digital_t;
	digital_structs.push_back(control_flow);
	control_flow->struct_type = CTRL_FLOW;
	control_flow->_state = NULL;

	list<parser*>::iterator parsers_it = parsers.begin();
    while(parsers_it!=parsers.end()){
        //cout<< "Parser name:" << (*parsers_it)->name << endl;

        list<State*>::iterator states_it = (*parsers_it)->states.begin();
        while(states_it!=(*parsers_it)->states.end()){
            //cout<< "State name: " << (*states_it)->name << endl;
   			Digital_t * curr_state = new Digital_t;
			digital_structs.push_back(curr_state);
			curr_state->type = STATE;
			curr_state->_state = *states_it;

            list<transition*>::iterator transit_it = (*states_it)->transitions.begin();
            while(transit_it != (*states_it)->transitions.end()){
                cout<<"Transition from_state: " << (*transit_it)->from_state->name << endl;
                if((*transit_it)->to_state == NULL){
                    cout<<"Transition to_state: NULL" << endl;
                }
                else cout<<"Transition to_state: " << (*transit_it)->to_state->name << endl;
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

	return digital_structs;
}
*/
