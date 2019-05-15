#ifndef _DIGITAL_H
#define _DIGITAL_H

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "parser.h"

// types of gates
enum ComponentType {
	tUndefined = 0,
	tMux = 1,
	tAnd = 1 << 1,
	tNot = 1 << 2,
	tNor = 1 << 3,
	tXor = 1 << 4,
	tXnor = 1 << 5,
	tConst = 1 << 6,
	tRegister = 1 << 7,
	tGate = 1 << 8,
	LastOne /* always keep this at the end */
};

// helpers for type checks
#define IS_MUX(X) X->get_type() & tMux
#define IS_AND(X) X->get_type() & tAnd
#define IS_OR(X) X->get_type() & tOr
#define IS_NOT(X) X->get_type() & tNot
#define IS_NOR(X) X->get_type() & tNor
#define IS_XOR(X) X->get_type() & tXor
#define IS_XNOR(X) X->get_type() & tXnor
#define IS_CONSTANT(X) X->get_type() & tConst
#define IS_REGISTER(X) X->get_type() & tRegister
#define IS_GATE(X) X->get_type() & tGate

#define ADD_INPUTS(T)							\
	size_t add_inputs(std::initializer_list<T> l)			\
	{								\
		m_inputs.insert(m_inputs.end(), l.begin(), l.end());	\
		return m_inputs.size();					\
	}								\
	size_t get_num_inputs() const					\
	{								\
		return m_inputs.size();					\
	}


const char *ComponentNames[] = {
	"mux", "and", "not", "nor", "xor", "xnor", "constant", "register", "gate"
};

class CComponent;

// for connecting different components
class Wire {
private:
	CComponent *m_src;
	std::vector<CComponent *> m_dst;
public:
	Wire() : m_src(nullptr)
	{
	}

	Wire(CComponent *t_src) : m_src(t_src)
	{
	}

	Wire(CComponent *t_src, std::initializer_list<CComponent *> t_list) :
		m_src(t_src)
	{
		m_dst.insert(m_dst.end(), t_list.begin(), t_list.end());
	}

	size_t add_destination(std::initializer_list<CComponent *> t_list)
	{
		m_dst.insert(m_dst.end(), t_list.begin(), t_list.end());
	}

	std::vector<CComponent *>& get_destinations() { return m_dst; }
	CComponent * get_src() { return m_src; }
};

// base class for all hardware components
class CComponent {
protected:
	// The type of the circuit component
	int m_type;
public:
	CComponent() { m_type = tUndefined; }

	int get_type() const { return m_type; }
};

/** CGate - A circuit gate
 *
 * This extends the circuit component for all circuit gates
 */
class CGate : public CComponent {
private:
	std::vector<Wire *> m_inputs;
	Wire *m_output;
public:
	CGate() : CComponent ()
	{
		m_type |= tGate;
	}

	CGate(std::initializer_list<Wire *> t_list) : CComponent()
	{
		m_inputs.insert(m_inputs.end(), t_list.begin(), t_list.end());
	}

	ADD_INPUTS(Wire *)

	std::vector<Wire *>& get_inputs() { return m_inputs; }
	Wire *get_output() { return m_output; }
};

class Constant : public CComponent {
private:
	std::string m_value;
	Wire *m_output;
public:
	/* Constructor for the constant component
	 *
	 * @t_value: string: The value of the constant
	 */
	Constant(std::string& t_value)
		: CComponent(), m_value(t_value), m_output(new Wire(this))
	{
		m_type |= tConst;
	}
};

// control flow
class Register: public CComponent {
public:
	Register() : CComponent(), m_output(new Wire(this))
	{
		m_type |= tRegister;
	}

	Register(std::initializer_list<Wire *> t_inputs) : CComponent()
							   , m_output(new Wire(this))
	{
		m_type |= tRegister;

		m_inputs.insert(m_inputs.end(),
				t_inputs.begin(), t_inputs.end());
	}

	ADD_INPUTS(Wire *)

	std::vector<Wire *>& get_inputs() { return m_inputs; }
	Wire *get_output() { return m_output; }
private:
	std::vector<Wire *> m_inputs;
	Wire *m_output;
};

class Mux: public CComponent {
public:
	Mux() : CComponent(), m_select(nullptr), m_output(new Wire(this))
	{
		m_type |= tMux;
	}

	Mux(Wire *t_select)
		: CComponent(), m_select(t_select), m_output(new Wire(this))
	{
		m_type |= tMux;
	}

	ADD_INPUTS(Wire *)

	Wire *get_output() const { return m_output; }
	Wire *get_select() const { return m_select; }
	std::vector<Wire *>& get_inputs() { return m_inputs; }

private:
	Wire *m_select;
	Wire *m_output;
	std::vector<Wire *> m_inputs;
};

// helper to create new gates
#define CREATE_GATE(NAME, TYPE)							\
	class NAME : public CGate {						\
	public:									\
		NAME () : CGate ()						\
		{								\
			m_type |= TYPE;						\
		}								\
		NAME(std::initializer_list<Wire *> t_list) : CGate(t_list)	\
		{								\
			m_type |= TYPE;						\
		}								\
	};

CREATE_GATE(AndGate, tAnd)
CREATE_GATE(Inverter, tNot)
CREATE_GATE(NorGate, tNor)
CREATE_GATE(XorGate, tXor)
CREATE_GATE(XnorGate, tXnor)

// actual function that parses the FSM into digital structs
std::list<CComponent*> parse_digital(std::list<Parser *>& parsers);
std::string print_type(int type);

#endif /* _DIGITAL_H */
