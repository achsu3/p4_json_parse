#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "p4aig/p4aig.h"
#include "p4parser/handlers.h"

#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"

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

static inline size_t get_number_characters(std::ifstream& in)
{
	size_t count = 0;
	while (!in.eof()) {
		in.get();
		count++;
	}

	// reset the file stream to the beginning
	in.clear();
	in.seekg(0, ios::beg);
	return count;
}

// the main parsing routine
std::list<P4Parser *>& P4AigParser::parse_p4_program()
{
	P4Handler handler;
	std::ostringstream sstream;
	rapidjson::Reader reader;
	size_t num_chars;
	char *json_chars, *json_p;

	if (!in_file.is_open()) {
		std::cerr << "[FATAL] Something weird must have happened"
			<< std::endl;
		exit(0);
	}

	// need to put the file into a rapidjson stringstream
	num_chars = get_number_characters(in_file);
	json_chars = new char[num_chars + 1];
	json_chars[num_chars] = '\0';

	// Need to do this since for some reason the parser always failed when
	// using the std library's c_str() function. So will do a dumb copy for
	// now until we can find a better solution
	json_p = json_chars;
	while (!in_file.eof()) {
		*json_p++ = in_file.get();
	}
	rapidjson::StringStream r_ss(json_chars);

	// using the kParseStopWhenDoneFlag becase for some reason we end up
	// with a const char * with a bunch of empty spaces at the end and
	// rapidjson does not like that at all.
	rapidjson::ParseResult ok =
		reader.Parse<rapidjson::kParseStopWhenDoneFlag>(r_ss, handler);
	if (!ok) {
		std::cerr << "[FATAL] RapidJson Parsing Error:"
			<< rapidjson::GetParseError_En(ok.Code())
			<< " at " << ok.Offset() << std::endl;
		free(json_chars);
		exit(0);
	}
	std::cout << "[P4AIG] RapidJson parsed successfully..." << std::endl;

	// save the parsers by copying them and return a reference
	p4parsers = handler.copy_parsers();
	free(json_chars);
	return p4parsers;
}
