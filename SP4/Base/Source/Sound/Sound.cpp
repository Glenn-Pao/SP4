#include "Sound.h"
#include "..\Application.h"

CSound::CSound()
	: MainMenu(NULL)
	, MazeEscaper(NULL)
	, MCQPower(NULL)
	, PuzzleParty(NULL)
	, Stress(NULL)
	, Lobby(NULL)
	, volMM(0)
	, volLob(0)
	, volME(0)
	, volMP(0)
	, volPP(0)
	, volS(0)
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
//play the main menu bgm
void CSound::PlayMainMenu()
{
	//after initialising, play the song
	if (MainMenu->getIsPaused())
	{
		MainMenu->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volMM < maxVol)
	{
		volMM += 0.003;
	}
	if (volMM > maxVol)
	{
		volMM = maxVol;
	}
	MainMenu->setVolume(volMM);
}
//play the maze escaper bgm
void CSound::PlayMazeEscaper()
{
	//after initialising, play the song
	if (MazeEscaper->getIsPaused())
	{
		MazeEscaper->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volME < maxVol)
	{
		volME += 0.003;
	}
	if (volME > maxVol)
	{
		volME = maxVol;
	}
	MazeEscaper->setVolume(volME);
}
//play the puzzle party bgm
void CSound::PlayPuzzleParty()
{
	//after initialising, play the song
	if (PuzzleParty->getIsPaused())
	{
		PuzzleParty->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volPP < maxVol)
	{
		volPP += 0.003;
	}
	if (volPP > maxVol)
	{
		volPP = maxVol;
	}
	PuzzleParty->setVolume(volPP);
}
//play the MCQ bgm
void CSound::PlayMCQPower()
{
	//after initialising, play the song
	if (MCQPower->getIsPaused())
	{
		MCQPower->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volMP < maxVol)
	{
		volMP += 0.003;
	}
	if (volMP > maxVol)
	{
		volMP = maxVol;
	}
	MCQPower->setVolume(volMP);
}
//play the Stress bgm
void CSound::PlayStress()
{
	//after initialising, play the song
	if (Stress->getIsPaused())
	{
		Stress->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volS < maxVol)
	{
		volS += 0.003;
	}
	if (volS > maxVol)
	{
		volS = maxVol;
	}
	Stress->setVolume(volS);
}
//play the Lobby bgm
void CSound::PlayLobby()
{
	//after initialising, play the song
	if (Lobby->getIsPaused())
	{
		Lobby->setIsPaused(false);
	}
	//this only works if there was a fade out previously.
	//gradually fade in the music
	if (volLob <maxVol)
	{
		volLob += 0.003;
	}
	if (volLob > maxVol)
	{
		volLob = 1.f;
	}
	Lobby->setVolume(volLob);
}
//temporarily stop playing the main menu. the soundtrack will be paused instead of dropped altogether
void CSound::StopMainMenu()
{
	//gradually fade out the music
	if (volMM > 0.f)
	{
		volMM -= 0.01;
	}
	MainMenu->setVolume(volMM);
	if (volMM < 0.f)
	{
		volMM = 0.f;
		MainMenu->setIsPaused(true);
	}
}
//temporarily stop playing the maze escaper. the soundtrack will be paused instead of dropped altogether
void CSound::StopMazeEscaper()
{
	//gradually fade out the music
	if (volME > 0.f)
	{
		volME -= 0.01;
	}
	MazeEscaper->setVolume(volME);
	if (volME < 0.f)
	{
		volME = 0.f;
		MazeEscaper->setIsPaused(true);
	}
}
//temporarily stop playing the puzzle party music. the soundtrack will be paused instead of dropped altogether
void CSound::StopPuzzleParty()
{
	//gradually fade out the music
	if (volPP > 0.f)
	{
		volPP -= 0.01;
	}
	PuzzleParty->setVolume(volPP);
	if (volPP < 0.f)
	{
		volPP = 0.f;
		PuzzleParty->setIsPaused(true);
	}
}
//temporarily stop playing the MCQ music. the soundtrack will be paused instead of dropped altogether
void CSound::StopMCQPower()
{
	//gradually fade out the music
	if (volMP > 0.f)
	{
		volMP -= 0.01;
	}
	MCQPower->setVolume(volMP);
	if (volMP < 0.f)
	{
		volMP = 0.f;
		MCQPower->setIsPaused(true);
	}
}
//temporarily stop playing the Stress music. the soundtrack will be paused instead of dropped altogether
void CSound::StopStress()
{
	//gradually fade out the music
	if (volS > 0.f)
	{
		volS -= 0.01;
	}
	Stress->setVolume(volS);
	if (volS < 0.f)
	{
		volS = 0.f;
		Stress->setIsPaused(true);
	}
}
//temporarily stop playing the Lobby music. the soundtrack will be paused instead of dropped altogether
void CSound::StopLobby()
{
	//gradually fade out the music
	if (volLob > 0.f)
	{
		volLob -= 0.01;
	}
	Lobby->setVolume(volLob);
	if (volLob < 0.f)
	{
		volLob = 0.f;
		Lobby->setIsPaused(true);
	}
	
}
//adjust the volume during options. will need to have another variable to help remember this number
void CSound::AdjustVol()
{
	/*if (Application::IsKeyPressed(VK_UP) && volume < 1.f)
	{ 
		volume += 0.01f;
	}
	if (Application::IsKeyPressed(VK_DOWN) && volume > 0.f)
	{
		volume -= 0.01f;
	}
	TheSound->setSoundVolume(volume);*/
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