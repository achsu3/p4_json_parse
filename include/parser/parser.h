#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <list>

// defined so transition and state can use each other as members
class State;

class Transition {
public:
	//pointer to the state this transition is coming from
	State * from_state;
	// "transitions"->"next_state"
	// hold for the name of the next state
	// that will determine what will be put in
	// the pointer, since the next state hasn't
	// been created yet
	std::string str_to_state;
	State * to_state;
	// the value that triggers this next state
	std::string value; //from;

	// the value type that determines the transition
	// probably based off of "parser_ops"
	std::list<std::string> value_type;

	void add_value_type(std::string v);
};

class State {
private:
	std::string name; // parse_states -> name
	std::list<Transition* > transitions;
public:
	/** Constructor
	 *
	 * @n: the name for this state. This will be the key for this state
	 * going forward
	 *
	 * @return nothing
	 */
	State(std::string n);

	/** add_transition - Add a transition out of this state
	 *
	 * @t: the transtition to add. must be going out of this state
	 *
	 * @return nothing
	 */
	void add_transition(Transition * t);

	/* setters and getters */
	std::string get_name() { return name; }
	std::list<Transition *>& get_transitions() { return transitions; }
};

class P4Parser {
private:
	//first "name" after finding "parsers"
	std::string name;
	// the list of all states in the parser
	std::list<State *> states;
	// maps all the states and the namespace
	// needed for use in second pass of parse
	std::map<std::string,State *> state_map;
public:
	/** constructor
	 *
	 * @s: the name of the parser
	 *
	 */
	P4Parser(std::string s) { name = s; }

	/** add_state - add a state to the parser
	 *
	 * @s: the state to add to the parser
	 *
	 * @return nothing
	 */
	void add_state(State *s);

	/** lookup_state - Lookup a state by name
	 *
	 * @name: The name of the state to lookup
	 *
	 * @return the appropriate state if found, NULL otherwise
	 */
	State *lookup_state(std::string name);

	/* setters and getters */
	std::string get_name() { return name; }
	std::list <State *>& get_states() { return states; }
};

#endif /* _PARSER_H */

