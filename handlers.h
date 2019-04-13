#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "digital_structs.h"
#include "parser.h"
#include <iostream>
#include <list>
#include <string>

using namespace rapidjson;
using namespace std;

//global variable for resulting parser structure(s) 
parser _parser;

//global variable for whether we are on the "Parsers" portion of the JSON
//and we should start putting things into structures
int parser = 0;

//variables to track whether we are done with the parser
int arrcount = 0;

class current_key{
	string key; //the name of the key
	int count;	//the count of startArray/objs so we know when this key is over
	current_key * next_key;	//the next key - questionable!
};

struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
    bool Null() {
      cout << "Null()" << endl;
      return true;
    }
    bool Bool(bool b) {
      cout << "Bool(" << boolalpha << b << ")" << endl;
      return true;
    }
    bool Int(int i) {
      cout << "Int(" << i << ")" << endl;
      return true;
    }
    bool Uint(unsigned u) {
      cout << "Uint(" << u << ")" << endl;
      return true;
    }
    bool Int64(int64_t i) {
      cout << "Int64(" << i << ")" << endl;
      return true;
    }
    bool Uint64(uint64_t u) {
      cout << "Uint64(" << u << ")" << endl;
      return true;
    }
    bool Double(double d) {
      cout << "Double(" << d << ")" << endl;
      return true;
    }
    bool String(const char* str, SizeType length, bool copy) {
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() {
      cout << "StartObject()" << endl;
      return true;
    }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        //if the key is "parsers" -> set a flag and start parsing into digital logic classes
        if(str == "parsers"){
          //turn flag on and parse things into classes
          parser = 1;
        }
		else if(str == "name"){
		
		}

		return true;
    }
    bool EndObject(SizeType memberCount) {
      cout << "EndObject(" << memberCount << ")" << endl;
      return true;
    }
    bool StartArray() {
      cout << "StartArray()" << endl;
      if(parser == 1){
	  	arrcount++;
	  }
	  
	  return true;
    }
    bool EndArray(SizeType elementCount) {
      cout << "EndArray(" << elementCount << ")" << endl;
      if(parser == 1){
	  	arrcount--;
		if(arrcount == 0){
			parser = 0;
		}
	  }

	  return true;
    }
};
