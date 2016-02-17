#include "Sound.h"
#include "Application.h"

CSound::CSound()
	: MainMenu(NULL)
	, MazeEscaper(NULL)
	, MCQPower(NULL)
	, PuzzleParty(NULL)
	, Stress(NULL)
	, Lobby(NULL)
	, volume(1)
	, maxVol(1)
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
		MainMenu = TheSound->play2D("../irrKlang/media/Bonetrousle.ogg", true, true);
	}
	//load the game 1 background sound
	if (MazeEscaper == NULL)
	{
		MazeEscaper = TheSound->play2D("../irrKlang/media/Dummy.ogg", true, true);
	}
	//load the game 2 background sound
	if (PuzzleParty == NULL)
	{
		PuzzleParty = TheSound->play2D("../irrKlang/media/Death By Glamour.ogg", true, true);
	}
	//load the game 3 background sound
	if (MCQPower == NULL)
	{
		MCQPower = TheSound->play2D("../irrKlang/media/Metal Crusher.ogg", true, true);
	}
	//load the game 4 background sound
	if (Stress == NULL)
	{
		Stress = TheSound->play2D("../irrKlang/media/Heartache.ogg", true, true);
	}
	//load the game 4 background sound
	if (Lobby == NULL)
	{
		Lobby = TheSound->play2D("../irrKlang/media/sans.ogg", true, true);
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
//play the maze escaper bgm
void CSound::PlayMazeEscaper()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (MazeEscaper->getIsPaused())
	{
		MazeEscaper->setIsPaused(false);
	}
}
//temporarily stop playing the maze escaper. the soundtrack will be paused instead of dropped altogether
void CSound::StopMazeEscaper()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		MazeEscaper->setIsPaused(true);
	}
}
//play the puzzle party bgm
void CSound::PlayPuzzleParty()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (PuzzleParty->getIsPaused())
	{
		PuzzleParty->setIsPaused(false);
	}
}
//temporarily stop playing the puzzle party music. the soundtrack will be paused instead of dropped altogether
void CSound::StopPuzzleParty()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		PuzzleParty->setIsPaused(true);
	}
}
//play the MCQ bgm
void CSound::PlayMCQPower()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (MCQPower->getIsPaused())
	{
		MCQPower->setIsPaused(false);
	}
}
//temporarily stop playing the MCQ music. the soundtrack will be paused instead of dropped altogether
void CSound::StopMCQPower()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		MCQPower->setIsPaused(true);
	}
}
//play the Stress bgm
void CSound::PlayStress()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (Stress->getIsPaused())
	{
		Stress->setIsPaused(false);
	}
}
//temporarily stop playing the Stress music. the soundtrack will be paused instead of dropped altogether
void CSound::StopStress()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		Stress->setIsPaused(true);
	}
}
//play the Lobby bgm
void CSound::PlayLobby()
{
	//this only works if there was a fade out previously.
	FadeIn();

	//after initialising, play the song
	if (Lobby->getIsPaused())
	{
		Lobby->setIsPaused(false);
	}
}
//temporarily stop playing the Lobby music. the soundtrack will be paused instead of dropped altogether
void CSound::StopLobby()
{
	FadeOut();
	if (volume < 0.f)
	{
		volume = 0.f;
		Lobby->setIsPaused(true);
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

	//drop all the sounds loaded to avoid mem leak
	if (MainMenu)
	{
		MainMenu->drop();
	}
	if (MazeEscaper)
	{
		MazeEscaper->drop();
	}
	if (PuzzleParty)
	{
		PuzzleParty->drop();
	}
	if (MCQPower)
	{
		MCQPower->drop();
	}
	if (Stress)
	{
		Stress->drop();
	}
	if (Lobby)
	{
		Lobby->drop();
	}
}