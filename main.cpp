#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API 
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include "handlers.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

int main(){
	ifstream inFile ("basic.json");
	if(!inFile){
		printf("/n couldn't open file /n");
		return -1;
	}
	printf("\n opened file \n");
	
	MyHandler handler;
	Reader reader;


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
