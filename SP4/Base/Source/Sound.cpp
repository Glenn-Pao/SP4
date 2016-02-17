#include "Sound.h"
#include "Application.h"

CSound::CSound()
	: MainMenu(NULL)
	, volume(1)
{
	//create the new sound system
	TheSound = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);
}


CSound::~CSound()
{
}

void CSound::Init()
{
	//load the main menu background sound
	if (MainMenu == NULL)
	{
		MainMenu = TheSound->play2D("../irrKlang/media/Undertale - Bonetrousle.ogg", true, true);
	}
}
//gradually fade in the music
void CSound::FadeIn()
{
	//gradually fade in the music
	if (volume < 1.f)
	{
		volume += 0.01;
	}
	if (volume > 1.f)
	{
		volume = 1.f;
	}
	TheSound->setSoundVolume(volume);
}
//gradually fade out the music
void CSound::FadeOut()
{
	//gradually fade out the music
	if (volume > 0.f)
	{
		volume -= 0.01;
	}
	TheSound->setSoundVolume(volume);
}
//play the main menu bgm
void CSound::PlayMainMenu()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (MainMenu->getIsPaused())
	{
		MainMenu->setIsPaused(false);
	}
}
//temporarily stop playing the main menu. the soundtrack will be paused instead of dropped altogether
void CSound::StopMainMenu()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		MainMenu->setIsPaused(true);
	}
}
//adjust the volume during options. will need to have another variable to help remember this number
void CSound::AdjustVol()
{
	if (Application::IsKeyPressed(VK_UP) && volume < 1.f)
	{ 
		volume += 0.01f;
	}
	if (Application::IsKeyPressed(VK_DOWN) && volume > 0.f)
	{
		volume -= 0.01f;
	}
	TheSound->setSoundVolume(volume);
}
//dump the sound once done
void CSound::Exit()
{
	//drop the sound system when resetting/exiting
	TheSound->drop();

	if (MainMenu)
	{
		MainMenu->drop();
	}
}