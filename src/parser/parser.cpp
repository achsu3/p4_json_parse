#include "parser/parser.h"

using namespace std;

void
Transition::add_value_type(string v)
{
	value_type.push_back(v);
}

/* State constructor */
State::State(string n)
{
	this->name = n;
}

/* Add a transition out from a state */
void
State::add_transition(Transition *t)
{
	transitions.push_back(t);
}

/* Add a state to the parser */
void
Parser::add_state(State *s)
{
	states.push_back(s);
	// map it
	state_map.insert(pair<string, State*>(s->get_name(), s));
}

/* lookup a state by name */
State *
Parser::lookup_state(string name)
{
	map<string, State*>::iterator it = state_map.find(name);

	if (it == state_map.end())
		return NULL;

	return it->second;
}
