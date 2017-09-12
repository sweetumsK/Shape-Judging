#include"Header.h"
#include<json\json.h>

using namespace Json;

void GetResult(const bool judging, string& result) {
	if (judging) result = "{\"result\":\"yes\"}";
	else result = "{\"result\":\"no\"}";
}

int main(int argc, char* argv[]) {
	string s, itemPath, result;
	Reader reader;
	Value root, product;
	s = argv[argc - 1];
	//Mat image = imread("4.jpg");
	//Item item(image);
	if (reader.parse(s, root)) {
		product = root["product"];
		itemPath = product["file"].asString();
		Mat image = imread(itemPath);
		Item item(image);
		if (product["type"].asString() == "shirt") GetResult(item.IsShirtFolded(), result);
		else GetResult(item.IsTrousersFolded(), result);
		item.~Item();
	}
	// how to return the result? can json receive the string from printf?
   	return 0;
}
