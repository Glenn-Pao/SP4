#include "Highscore.h"

Highscore::Highscore()
{
	counter = 0;
}

Highscore::~Highscore()
{
	ofstream myFile("Highscore.txt");
	int i = 0;
	if(myFile.is_open())
	{
		while(i != 5)
		{
			myFile << record[i].getScore() <<  record[i].getName() << endl;
			i++;
		}
	}
}

void Highscore::newScore(int score)
{
	if(counter < 5)
	{
		record[counter].setScore(score);
		counter++;
	}
}

int Highscore::getCount(void)
{
	return counter;
}

void Highscore::reset()
{
	for(int i = 0; i < counter; ++i)
	{
		record[counter].setScore(0);
	}

}

bool Highscore::ReadFromTextFile()
{
	string line;
	std::ifstream myfile;
	myfile.open("Highscore.txt");
	

	if (myfile.is_open())
	{
		cout << "File opened" << endl;
		int i = 0;
		while(i != 5)
		{
			std::getline(myfile, line);
			line >> record[i];
			i += 1;
		}
	}
	else
	{
		cout << "File not opened" << endl;	
		return false;
	}
	
	myfile.close();

	return true;
}

int Highscore::writehighscore(string File, CRecord rec)
{
	ofstream myFile(File);
	int i = 0;
	if(myFile.is_open())
	{
		while(i < 5)
		{
			myFile << rec.getScore() << " " << rec.getName() << endl;
		}
	}
	return i;
}

bool Highscore::HighscoreCheck(CRecord recs)
{
	if (recs.getScore() > record[4].getScore())
	{
		return true;
	}
	else
		return false;
}

void Highscore::storeNewRecord(CRecord recs)
{
	for(int i = 0; i < 4; ++i)
	{
		if (recs.getScore() > record[i].getScore())
		{
			for(int k = 4; k > i + 1; --k)
			{
				record[k] = record[k - 1];
			}
			record[i] = recs;
			return;
		}
	}
}