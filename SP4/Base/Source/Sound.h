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

	void Exit();

	void PlayMainMenu();			//plays the main menu
	void StopMainMenu();			//stop the main menu

	void FadeOut();					//fade out the bgm
	void FadeIn();					//fade in the bgm
	void AdjustVol();					//adjust the volume in options

private:
	//Handles to play different sounds
	ISoundEngine *TheSound;	//main sound system

	ISound *MainMenu;		//background music for main menu

	float volume;
};

