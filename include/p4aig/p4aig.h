#ifndef __P4AIG_H
#define __P4AIG_H

#include <fstream>

#include "p4parser/p4parser.h"

namespace p4aig
{
	/**
	 * P4AigParser - The main p4aig parser that starts from the input json
	 *	file and maintains the list of parsers in the p4 program
	 */
	class P4AigParser {
	private:
		std::ifstream in_file;
		std::list<P4Parser *> p4parsers;
	public:
		/* P4AigParser - constructor
		 *
		 * @f: string: the name of the input file generated from the p4
		 * program
		 */
		P4AigParser(std::string f);

		/* P4AigParser - constructor
		 *
		 * @f: const char*: the name of the input file generated from
		 * the p4 program
		 */
		P4AigParser(const char *f);

		/* parse_file - Parse the file and get the p4 parsers
		 *
		 * @return: a list of p4parsers
		 */
		std::list<P4Parser *>& parse_p4_program();
	}; /* p4aigparser */
}

#endif /* p4aig.h */

