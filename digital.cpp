#include "digital.h"

// global control flow
Control_Flow * control_flow = new Control_Flow();

list<Circuit*> parse_digital(list<Parser *>& parsers){

  //list of all the components in this - excluding the wires
  list<Circuit*> components;
  components.push_back(control_flow);

  list<Parser*>::iterator parsers_it = parsers.begin();
	while(parsers_it!=parsers.end()) {

    // keeps track of where we are in connecting all the muxs together
    Wire * prev_mux_wire = NULL;

		list<State*>::iterator states_it = (*parsers_it)->get_states().begin();
		while(states_it!=(*parsers_it)->get_states().end()) {

      Wire * prev_xnor_wire = NULL;

      // be used to determine the select bit
      Wire * state_wire = new Wire();
      Constant_Value * _state = new Constant_Value((*states_it)->get_name(),state_wire);
      components.push_back(_state);
      // set members for the wire
      state_wire->from = _state;
      Xnor* state_xnor = new Xnor;
      components.push_back(state_xnor);
      state_wire->add_to(state_xnor);
      state_xnor->input1 = state_wire;
      // set a control flow wire
      Wire * ctrl_fl_wire = new Wire;
      ctrl_fl_wire->from = control_flow;
      ctrl_fl_wire->add_to(state_xnor);
      state_xnor->input2 = ctrl_fl_wire;
      control_flow->addOutput(ctrl_fl_wire);

      //create a wire from the xnor
      Wire * state_xnor_out = new Wire();
      state_xnor_out->from = state_xnor;

      // start iterating through transitions
			list<Transition*>::iterator transit_it =
				(*states_it)->get_transitions().begin();

			while (transit_it != (*states_it)->get_transitions().end()){
				//from_state will always be the current state in the states iterator

        // make all the components
        Mux * transit_mux = new Mux();
        components.push_back(transit_mux);

        // if this is the only transition there is
        if((*transit_it)->to_state == NULL && prev_xnor_wire == NULL){
          // create simple XNOR situation for the mux that
          // only depends on whether the control flow
          // has reached this state
          state_xnor_out->add_to(transit_mux);
          transit_mux->select_input = state_xnor_out;

          // connect to the previous mux if there was one
          if(prev_mux_wire){
            prev_mux_wire->add_to(transit_mux);
            transit_mux->addInput(prev_mux_wire);
          }
          //connect to control flow
          else{
            Wire * control_flow_default = new Wire;
            control_flow_default->from = control_flow;
            control_flow_default->add_to(transit_mux);
            control_flow->addOutput(control_flow_default);
            transit_mux->addInput(control_flow_default);
          }

          // do the next_state option for the mux - default
          Constant_Value * next_state = new Constant_Value();
          components.push_back(next_state);
          next_state->value = "Accept";
          Wire * mux_select_wire = new Wire();
          next_state->output = mux_select_wire;
          mux_select_wire->from = next_state;
          mux_select_wire->add_to(transit_mux);

          Wire * mux_output_wire = new Wire();
          transit_mux->output = mux_output_wire;
          mux_output_wire->from = transit_mux;

          prev_mux_wire = mux_output_wire;
          transit_it ++;
          continue;

        }

        // connect the state_xnor to the and gate
        And * mux_select_and = new And();
        components.push_back(mux_select_and);
        mux_select_and->input1 = state_xnor_out;
        state_xnor_out->add_to(mux_select_and);

        //create wire from the and gate to the mux
        Wire * mux_select_wire = new Wire();
        mux_select_wire->from = mux_select_and;
        mux_select_wire->add_to(transit_mux);
        mux_select_and->output = mux_select_wire;
        transit_mux->select_input = mux_select_wire;

        Wire * mux_state_wire = new Wire();

        Constant_Value * next_state = new Constant_Value();
        components.push_back(next_state);
        next_state->output = mux_state_wire;

        // this is the default case
        if ((*transit_it)->to_state == NULL){
					next_state->value = "Accept";
				}
        else{
          next_state->value = (*transit_it)->to_state->get_name();
        }

        // set the wire connecting the state to the mux
        mux_state_wire->from = next_state;
        mux_state_wire->add_to(transit_mux);

        // push the 0 first so things are in order
        if(prev_mux_wire == NULL){
          // make another wire to the control flow
          Wire * mux_ctrl_fl_wire = new Wire;
          mux_ctrl_fl_wire->from = control_flow;
          mux_ctrl_fl_wire->add_to(transit_mux);

          control_flow->addOutput(mux_ctrl_fl_wire);
          transit_mux->addInput(mux_ctrl_fl_wire);
        }
        else{
          prev_mux_wire->add_to(transit_mux);
          transit_mux->addInput(prev_mux_wire);
        }
        // add the 1 input
        transit_mux->addInput(mux_state_wire);

        // create another XNOR situation for the other input to the AND gate that
        // controls the select bit for the mux
        if(prev_xnor_wire == NULL){
          Xnor * value_xnor = new Xnor();
          components.push_back(value_xnor);
          Wire * value_type_xnor_wire = new Wire();
          value_type_xnor_wire->add_to(value_xnor);
          value_xnor->input1 = value_type_xnor_wire;

          Constant_Value * value_type = new Constant_Value();
          components.push_back(value_type);
          value_type->value = "";

          value_type_xnor_wire->from = value_type;
          value_type->output = value_type_xnor_wire;

          // print all the value_type elements
          list<string>::iterator value_t_it = (*transit_it)->value_type.begin();
          while(value_t_it!=(*transit_it)->value_type.end()) {
            value_type->value+= *value_t_it;
            value_t_it++;
          }

          // set the other input of the XNOR - the actual value
          // you're comparing to
          Wire * value_xnor_wire = new Wire();
          value_xnor_wire->add_to(value_xnor);
          value_xnor->input2 = value_xnor_wire;

          Constant_Value * xnor_value = new Constant_Value();
          components.push_back(xnor_value);
          xnor_value->value = (*transit_it)->value;

          value_xnor_wire->from = xnor_value;
          xnor_value->output = value_xnor_wire;

          // connect the xnor and the AND gate
          Wire * value_xnor_to_and = new Wire();
          value_xnor->output = value_xnor_to_and;
          value_xnor_to_and->from = value_xnor;
          value_xnor_to_and->add_to(mux_select_and);

          prev_xnor_wire = value_xnor_to_and;
        }
        // if there already exists one for this part
        // then just add a NOT gate
        else{
          Not * invert_xnor = new Not();
          components.push_back(invert_xnor);
          prev_xnor_wire->add_to(invert_xnor);
          invert_xnor->input1 = prev_xnor_wire;
          Wire * invert_to_and = new Wire();
          invert_to_and->from = invert_xnor;
          invert_to_and->add_to(mux_select_and);
          invert_xnor->output = invert_to_and;
        }

        // make an output wire for this MUX
        Wire * mux_output_wire = new Wire();
        transit_mux->output = mux_output_wire;
        mux_output_wire->from = transit_mux;

        prev_mux_wire = mux_output_wire;

				transit_it++;
			}

      //check if the last mux output needs to be connected back to the control flow
      if(prev_mux_wire->to.size() == 0){
        // set to control flow
        prev_mux_wire->add_to(control_flow);
        control_flow->addInput(prev_mux_wire);
      }

			states_it++;
		}

		parsers_it++;
	}

  return components;
}
