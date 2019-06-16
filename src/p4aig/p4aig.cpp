#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "p4aig/p4aig.h"
#include "p4parser/handlers.h"

using namespace p4aig;

P4AigParser::P4AigParser(std::string f)
{
	in_file.open(f, std::ifstream::in);
	if (!in_file.is_open()) {
		std::cerr << "FATAL ERROR: Cannot open file " << f << std::endl;
		exit(0);
	}
}

P4AigParser::P4AigParser(const char *f)
{
	in_file.open(f, std::ifstream::in);
	if (!in_file.is_open()) {
		std::cerr << "FATAL ERROR: Cannot open file " << f << std::endl;
		exit(0);
	}
}

// the main parsing routine
std::list<P4Parser *>& P4AigParser::parse_p4_program()
{
	P4Handler handler;
	std::ostringstream sstream;
	rapidjson::Reader reader;

	if (!in_file.is_open()) {
		std::cerr << "[FATAL] Something weird must have happened"
			<< std::endl;
		exit(0);
	}

	// need to put the file into a rapidjson stringstream
	// TODO: there must be a better way to handle this if the file
	// is too big
	sstream << in_file.rdbuf();
	rapidjson::StringStream r_ss(sstream.str().c_str());

	// try to parse the file
	reader.Parse(r_ss, handler);

	// save the parsers by copying them and return a reference
	p4parsers = handler.copy_parsers();
	return p4parsers;
}
