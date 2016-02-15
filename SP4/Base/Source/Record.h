#pragma once
#include <string>
#include <sstream>
using namespace std;

class CRecord
{
public:
	CRecord(void);
	~CRecord(void);

    int getScore();
	void setScore(int score);

	string getName();
	void setName(string);
	
	friend std::string& operator>>(std::string& line, CRecord& record)
	{
		std::string temp;
		int scoreTemp;

		std::istringstream iss(line);

		iss >> temp;
		scoreTemp = std::stoi(temp);
		record.setScore(scoreTemp);
		temp.clear();

		std::getline(iss, temp);
		record.setName(temp);

		return line;
    };
	void reset();
private:
	int score;
	string playername;

};

