#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include <iostream>
#include <vector>
#include <string>

using namespace rapidjson;
using namespace std;

class mux{
	public:
		vector<string> inputs;

};

class nand{
	public:
		vector<string> inputs;

};

class _not{
	public:
		vector<string> inputs;
};
