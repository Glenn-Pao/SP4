#pragma once
#include <string>

using std::string;

class UIFeature
{
private:
	
	string ID;

public:

	void setID(string ID);

	string getID();

	UIFeature();
	~UIFeature();
};

