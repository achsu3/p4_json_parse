#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API 
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include <iostream>
#include <fstream>

//using namespace rapidjson;

using namespace std;
using namespace rapidjson;

struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, SizeType length, bool copy) { 
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) { 
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

int main(){
	ifstream inFile ("basic.json");
	//inFile.open("basic.json");
	
	if(!inFile){
		printf("/n couldn't open file /n");
		return -1;
	}
	printf("\n opened file \n");

	//now we will try using the rapidjson FileReadStream
	FILE * fp = fopen("basic.json", "r"); //just open file
	char readBuffer[2048];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(is);
	
	//fclose(fp);
	
	//test above MyHandler with basic reader 
	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    MyHandler handler;
    Reader reader;
    StringStream ss(json);
    reader.Parse(ss, handler);
	
/*
	MyHandler handler;
	Reader reader;
	reader.Parse(is, handler);
*/
	
	//parse file normally and put into an array, test handler on it
	
	char _json[4096];
	int i=0;
	while(!inFile.eof()&&i<4096){
		_json[i] = inFile.get();	
		//std::cout<<_json[i]<<std::endl;
		i++;
	}

	StringStream s2(_json);
	reader.Parse(s2, handler);
	
	return 0;
}
