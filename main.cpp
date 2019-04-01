#include "stdlib.h"
#include "include/rapidjson/document.h" //DOM API 
#include "include/rapidjson/prettywriter.h" //stringify JSON
#include "include/rapidjson/filereadstream.h"
#include <iostream>
#include <fstream>

//using namespace rapidjson;

using namespace std;

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
	
	fclose(fp);
	return 0;
}
