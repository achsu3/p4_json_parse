#include "p4parser/handlers.h"

using namespace p4aig;

bool
P4Handler::Null()
{
	//cout << "Null()" << endl;
	if (in_parser_flag == 1 && name_flag == 0 &&
	    parse_states_flag == 1 && state_flag == 0 &&
	    transition_key_flag == 0 && value_flag == 0 &&
	    next_state_flag == 1 && transitions_flag == 1) {
		curr_transit->str_to_state = "\0";
		curr_transit->to_state = NULL;
		curr_transit->from_state = curr_state;
		next_state_flag = 0;
	}

	return true;
}

void
P4Handler::second_pass()
{

	list<P4Parser*>::iterator parsers_it = parsers.begin();
	while(parsers_it!=parsers.end()){

		list<State*>::iterator states_it =
			(*parsers_it)->get_states().begin();
		while(states_it!=(*parsers_it)->get_states().end()){

			list<Transition*>::iterator transit_it =
				(*states_it)->get_transitions().begin();
			while(transit_it != (*states_it)->get_transitions().end()){
				string lookup_str = (*transit_it)->str_to_state;
				State *s =
					(*parsers_it)->lookup_state(lookup_str);
				if (s) {
					(*transit_it)->to_state = s;
				} else if ((lookup_str.compare("\0") != 0)){
					cout << "Invalid state found: " <<
						lookup_str << endl;
				}
				transit_it++;
			}

			states_it++;
		}

		parsers_it++;
	}
}

bool
P4Handler::String(const char* str, SizeType length, bool copy)
{
	// condition for if a new parser is beginning
	if(in_parser_flag == 1 && name_flag == 1 &&
	   parse_states_flag == 0 && state_flag == 0 &&
	   transition_key_flag == 0 && value_flag == 0 &&
	   next_state_flag== 0) {
		curr_parser = new P4Parser(str);
		parsers.push_back(curr_parser);
		name_flag = 0;
	} // condition for grabbing the state name_flag
	else if (in_parser_flag == 1 && name_flag == 1 &&
		 parse_states_flag == 1 && state_flag == 0 &&
		 transition_key_flag == 0 && value_flag == 0 &&
		 next_state_flag== 0) {
		// make a new state and put it in the curr_parser's list
		curr_state = new State(str);

		curr_parser->add_state(curr_state);

		// clear the transition value types list
		transit_value_type.clear();

		// reset flag
		name_flag = 0;
	}
	// pushing the array that represents the array of fields of the transition key
	else if (in_parser_flag == 1 && name_flag == 0 &&
		 transition_key_flag == 1 && value_flag == 1 && arr_flag == 1){
		// this is going to need to change because now
		// this only adds the value type to the last transition
		// need an example of a JSON if a transition depends on a different value

		//curr_transit->add_value_type(str);

		transit_value_type.push_back(str);

		// put all the transition types into the transitions for this state
		list<Transition*>::iterator transit_it = curr_state->get_transitions().begin();
		while(transit_it!=curr_state->get_transitions().end()){
			(*transit_it)->value_type = transit_value_type;
			transit_it++;
		}

		// not resetting value_flag beecause we are pushing an array so we
		// need to come back to this condition multiple times in a row


	}	// condition for pushing the transition value to the list
	else if (in_parser_flag == 1 && name_flag == 0 &&
		 parse_states_flag == 1 && state_flag == 0 &&
		 transition_key_flag == 0 && value_flag == 1 &&
		 next_state_flag== 0 && transitions_flag == 1) {
		curr_transit = new Transition();
		curr_state->add_transition(curr_transit);
		curr_transit->value = str;
		curr_transit->from_state = curr_state;
		// reset flag
		value_flag = 0;
	}
	// condition for the next_state field of the transition
	else if (in_parser_flag == 1 && name_flag == 0 &&
		 parse_states_flag == 1 && state_flag == 0 &&
		 transition_key_flag == 0 && value_flag == 0 &&
		 next_state_flag == 1 && transitions_flag == 1) {
		curr_transit->str_to_state = str;
		curr_transit->from_state = curr_state;
		next_state_flag = 0;
	}

	return true;
}

bool
P4Handler::Key(const char* str, SizeType length, bool copy)
{
	//cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
	//if the key is "parsers" -> set a flag and start parsing into digital logic classes

	//find the length of the key so we can
	//make the char* to a string
	size_t str_length = 0;

	while(str[str_length] != '\0'){
		str_length++;
	}

	string string_str(str, str_length);

	if(string_str.compare("parsers") == 0) {
		//turn flag on and parse things into classes
		in_parser_flag = 1;
	} else if (in_parser_flag == 1) {
		if(string_str.compare("name") == 0) {
			name_flag = 1;
		}
		else if(string_str.compare("parse_states") == 0) {
			parse_states_flag = 1;
		}
		else if(string_str.compare("transitions") == 0) {
			transitions_flag = 1;
		}
		else if(string_str.compare("state") == 0) {
			state_flag = 1;
		}
		else if(string_str.compare("transition_key") == 0) {
			transitions_flag = 0;
			transition_key_flag = 1;
		}
		else if((transitions_flag == 1 || transition_key_flag == 1) &&
			string_str.compare("value") == 0) {
			value_flag = 1;
		}
		else if(string_str.compare("next_state") == 0) {
			next_state_flag= 1;
		}
	}

	return true;
}

bool
P4Handler::StartArray()
{
	//cout << "StartArray()" << endl;
	if(in_parser_flag == 1){
		arrcount_flag++;
	}

	if(transition_key_flag == 1){
		arr_flag = 1;
	}
	return true;
}

bool
P4Handler::EndArray(SizeType elementCount)
{
	//cout << "EndArray(" << elementCount << ")" << endl;
	if(in_parser_flag == 1){
		arrcount_flag--;
		if(arrcount_flag == 0){
			in_parser_flag = 0;
			//turn the rest of flags off too
			reset_flags();

			//call a function here that will instigate the second pass
			//to connect everything
			second_pass();
		}
	}
	if(transition_key_flag == 1){
		arr_flag = 0;
		// turn off this flag because we are at the end of this section of the JSON
		transition_key_flag = 0;
		value_flag = 0;
	}

	return true;
}
