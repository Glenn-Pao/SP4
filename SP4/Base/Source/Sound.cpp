#include "Sound.h"


CSound::CSound()
	: MainMenu(NULL)
{
	//create the new sound system
	TheSound = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);
}


CSound::~CSound()
{
	//drop the sound system when resetting/exiting
	TheSound->drop();

	if (MainMenu)
	{
		delete MainMenu;
		MainMenu = NULL;
	}
}

void CSound::Init()
{
	//load the main menu background sound
	if (MainMenu == NULL)
	{
		MainMenu = TheSound->play2D("../irrKlang/media/Undertale - Bonetrousle.ogg", true, true);
	}
}
void CSound::PlayMainMenu()
{
	if (MainMenu->getIsPaused())
	{
		MainMenu->setIsPaused(false);
	}
}
void CSound::PauseMainMenu()
{
	MainMenu->setIsPaused(true);
}