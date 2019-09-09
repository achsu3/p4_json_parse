#ifndef _CIRCUIT_COMPONENTS_H
#define _CIRCUIT_COMPONENTS_H

#include <string>
#include <sstream>
#include <map>
#include <list>
#include <vector>

namespace p4aig
{

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
	}								\
	std::vector<T>& get_inputs()					\
	{								\
		return m_inputs;					\
	}

#define ADD_OUTPUTS(T)							\
	size_t add_outputs(std::initializer_list<T> l)			\
	{								\
		m_outputs.insert(m_outputs.end(), l.begin(), l.end());	\
		return m_outputs.size();				\
	}								\
	size_t get_num_outputs() const					\
	{								\
		return m_outputs.size();				\
	}								\
	std::vector<T>& get_outputs()					\
	{								\
		return m_outputs;					\
	}

	// base class for all hardware components
	class CComponent {
	protected:
		// The type of the circuit component
		int m_type;
	public:
		CComponent() { m_type = tUndefined; }

		int get_type() const { return m_type; }

		/** to_str - Build a string representation of the component
		 *
		 * @param:ss: A string stream to keep manipulating
		 *  :type: ostringstream
		 *
		 * @return: an reference to the same output stream
		 */
		virtual std::ostringstream& to_str(std::ostringstream& ss) const = 0;
	};

	/** CGate - A circuit gate
	 *
	 * This extends the circuit component for all circuit gates
	 */
	class CGate : public CComponent {
	private:
		std::vector<CComponent *> m_inputs;
		std::vector<CComponent *> m_outputs;
	public:
		CGate() : CComponent ()
		{
			m_type |= tGate;
		}

		CGate(std::initializer_list<CComponent *> t_list) : CComponent()
		{
			m_inputs.insert(m_inputs.end(), t_list.begin(), t_list.end());
		}

		std::ostringstream& to_str(std::ostringstream& ss) const;

		ADD_INPUTS(CComponent *)
		ADD_OUTPUTS(CComponent *)
	};

	class Constant : public CComponent {
	private:
		std::string m_value;
		std::vector<CComponent *> m_outputs;
	public:
		/* Constructor for the constant component
		 *
		 * @t_value: string: The value of the constant
		 */
		Constant(std::string& t_value)
			: CComponent(), m_value(t_value)
		{
			m_type |= tConst;
		}

		std::ostringstream& to_str(std::ostringstream& ss) const;

		ADD_OUTPUTS(CComponent *)
	};

	// control flow
	class Register: public CComponent {
	public:
		Register() : CComponent()
		{
			m_type |= tRegister;
		}

		Register(std::initializer_list<CComponent *> t_inputs) : CComponent()
		{
			m_type |= tRegister;

			m_inputs.insert(m_inputs.end(),
					t_inputs.begin(), t_inputs.end());
		}

		std::ostringstream& to_str(std::ostringstream& ss) const;

		ADD_INPUTS(CComponent *)
		ADD_OUTPUTS(CComponent *)
	private:
		std::vector<CComponent *> m_inputs;
		std::vector<CComponent *> m_outputs;
	};

	class Mux: public CComponent {
	public:
		Mux() : CComponent(), m_select(nullptr)
		{
			m_type |= tMux;
		}

		Mux(CComponent *t_select)
			: CComponent(), m_select(t_select)
		{
			m_type |= tMux;
		}

		CComponent *get_select() const { return m_select; }

		ADD_INPUTS(CComponent *)
		ADD_OUTPUTS(CComponent *)
	private:
		CComponent *m_select;
		std::vector<CComponent *> m_outputs;
		std::vector<CComponent *> m_inputs;
	};

	// helper to create new gates
#define CREATE_GATE(NAME, TYPE)							\
	class NAME : public CGate {						\
	public:									\
		NAME () : CGate ()						\
		{								\
			m_type |= TYPE;						\
		}								\
		NAME(std::initializer_list<CComponent *> t_list) : CGate(t_list)\
		{								\
			m_type |= TYPE;						\
		}								\
	};

	CREATE_GATE(AndGate, tAnd)
	CREATE_GATE(Inverter, tNot)
	CREATE_GATE(NorGate, tNor)
	CREATE_GATE(XorGate, tXor)
	CREATE_GATE(XnorGate, tXnor)

}

// actual function that parses the FSM into digital structs
/* std::list<CComponent*> parse_digital(std::list<Parser *>& parsers); */

#endif /* CIRCUIT_COMPONENTS */
