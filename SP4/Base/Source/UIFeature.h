#pragma once
#include <string>
#include "Vector3.h"
using std::string;

class UIFeature
{

public:
	enum UI_TYPE
	{
		UT_START = 0,
		UT_BUTTON,
		UT_TOTAL
	};
	virtual void Update(float dt);

	void setID(string ID);
	void setDefaultPos(Vector3 DefaultPos);
	void setCurrentPos(Vector3 CurrentPos);
	void setScale(Vector3 Scale);
	void setUI_Type(UI_TYPE type);


	int getUI_Type();
	Vector3 getScale();
	Vector3 getCurrentPos();
	Vector3 getDefaultPos();
	string getID();


	UIFeature();
	virtual~UIFeature();

private:
	
	string ID;

	Vector3 DefaultPos, CurrentPos;

	Vector3 Scale;

	UI_TYPE type;
};

