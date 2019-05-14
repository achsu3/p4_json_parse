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
enum ComponentType {
	Mux = 0,
	And = 1,
	Not = 2,
	Nor = 3,
	Xor = 4,
	Xnor = 5,
	Const = 6,
	Register = 7,
	LastOne /* always keep this at the end */
};

const char *ComponentNames[] = {
	"mux", "and", "not", "nor", "xor", "xnor", "constant", "register"
};

class Wire;

// base class for all hardware components
class Circuit{
public:
	virtual ComponentType get_type() = 0;
};

class Constant : public Circuit{
private:
	string m_value;
	Wire *m_output;
public:
	/* Constructor for the constant component
	 *
	 * @t_value: string: The value of the constant
	 * @m_output: wite ptr: The output wirte, can be null!
	 */
	Constant(string t_value, Wire *t_output)
		: value(t_value), m_output(t_output)
	{
	}

	ComponentType get_type(){
		return Const;
	}
};

// control flow
class Register: public Circuit {
public:
	Control_Flow()
	{
	}

	void add_input(Wire *t_wire) {
		inputs.push_back(t_wire);
	}

	void add_output(Wire *t_wire) {
		outputs.push_back(_wire);
	}

	ComponentType get_type() {
		return Register;
	}

	vector<Wire *>& get_inputs() { return m_inputs; }
	vector<Wire *>& get_ouptputs() { return m_outputs; }
private:
	vector <Wire *> m_inputs;
	vector <Wire *> m_outputs;
};

class Mux: public Circuit {
public:
	Mux() : m_select(nullptr), m_output(nullptr)
	{
	}

	Mux(Wire *t_select, Wite *t_output)
		: m_select(t_select), m_output(t_output)
	{
	}

	void add_input(Wire *t_wire) {
		m_inputs.push_back(t_wire);
	}

	ComponentType get_type() {
		return MUX;
	}

	Wire * get_output const { return m_output; }
	Wire * get_select const { return m_select; }
	vector<Wire *>& get_inputs() { return m_inputs; }

private:
	Wire *m_select;
	Wire *m_output;
	vector<Wire *> m_inputs;
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
