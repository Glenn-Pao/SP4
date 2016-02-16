#pragma once

//sound engine
#include <irrKlang.h>
#include <iostream>

#pragma comment (lib, "irrKlang.lib")
using namespace irrklang; 

class CSound
{
public:
	CSound();
	~CSound();

	void Init();
	void UpdateSound(double &dt);

	void PlayMainMenu();			//plays the main menu
	void StopMainMenu();			//stop the main menu

	void AdjustVol();					//adjust the volume

private:
	//Handles to play different sounds
	ISoundEngine *TheSound;	//main sound system

	ISound *MainMenu;		//background music for main menu

	float volume;
};

