#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "p4aig/p4aig.h"

using namespace p4aig;
using namespace std;

int main() {
	P4AigParser *parser = new P4AigParser("basic.json");
	list<P4Parser *>& p4parsers = parser->parse_p4_program();

	list<P4Parser*>::iterator parsers_it = p4parsers.begin();
	cout << "Parsed " << p4parsers.size() << " P4 parsers." << endl;
#if 0
	// now translate to digital things
	list<CComponent *> circuit_components; // = parse_digital(handler.get_parsers());

	// test for the digital version by traversing the components pushed
	list<CComponent *>::iterator components_it = circuit_components.begin();
	while(components_it != circuit_components.end()){
		// print all the things this is attached to and the wires
		ostringstream os;
		(*components_it)->to_str(os);
		std::cout << os.str();
		components_it++;
	}

	return 0;
#endif 
}
