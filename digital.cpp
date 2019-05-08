#include "digital.h"

void parse_digital(list<Parser *>& parsers){

  list<Parser*>::iterator parsers_it = parsers.begin();
	while(parsers_it!=parsers.end()) {
		//cout<< "Parser name:" << (*parsers_it)->get_name() << endl;

		list<State*>::iterator states_it = (*parsers_it)->get_states().begin();
		while(states_it!=(*parsers_it)->get_states().end()) {
			//cout<< "State name: " << (*states_it)->get_name() << endl;
      // create a component for this state and set it in a XNOR that will
      // be used to determine the select bit
      Wire * state_wire = new Wire();
      Constant_Value * _state = new Constant_Value((*states_it)->get_name(),state_wire);
    /*  // set members for this value
      _state->value = (*states_it)->get_name();
      _state->output = state_wire;*/
      // set members for the wire
      state_wire->from = _state;
      Xnor* state_xnor = new Xnor;
      state_wire->to = state_xnor;
      state_xnor->input1 = state_wire;
      // set the control flow
      Wire * ctrl_fl_wire = new Wire;
      state_xnor->input2 = ctrl_fl_wire;
      control_flow->addOutput(ctrl_fl_wire);

			list<Transition*>::iterator transit_it =
				(*states_it)->get_transitions().begin();
			while (transit_it != (*states_it)->get_transitions().end()){
				cout<<"Transition from_state: " <<
					(*transit_it)->from_state->get_name() << endl;

				if ((*transit_it)->to_state == NULL){
					cout<<"Transition to_state: NULL" << endl;
				} else
					cout<<"Transition to_state: " <<
						(*transit_it)->to_state->get_name() << endl;

				cout<<"Transition value_type: " << endl;

				// print all the value_type elements
				list<string>::iterator value_t_it = (*transit_it)->value_type.begin();
				while(value_t_it!=(*transit_it)->value_type.end()) {
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

  return;
}
