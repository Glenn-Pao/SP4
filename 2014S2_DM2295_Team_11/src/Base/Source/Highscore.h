#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Record.h"

using namespace std;

class Highscore
{
public:
	Highscore();
	~Highscore();

	void newScore(int score);
	int getCount(void);

	void reset();

	bool ReadFromTextFile();
	CRecord record[5];            
	int writehighscore(string, CRecord);
    bool HighscoreCheck(CRecord recs);
    void storeNewRecord(CRecord recs);
private:		
	int counter;
};