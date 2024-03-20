#define GAME_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "collInfo.h"
#include "object.h"
#include "inputmapper.h"
#include "ball.h"
#include "field.h"
#include "random.h"
#include "gameObjects.h"
#include "openGLStuff.h"
#include "game.h"
#include "StateManager.h"
#include "BallManager.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "SpriteDemoManager.h"
#include "igame.h"

#include "BattleUIManager.h"
#include "InputHandler.h"
#include "TextPrinter.h"
#include "sound.h"
#include <time.h>


IGame* CreateGame() {
	return new CGame();
}

void DestroyGame(IGame* pGame) {
	delete pGame;
}

// Declarations
const char8_t CGame::mGameTitle[]="Pkmn Red";

CGame::CGame() {
	StateManagerC::CreateInstance();
	PokemonManager::CreateInstance();
	BattleUIManager::CreateInstance();
	TextPrinter::CreateInstance();
	
	//player->SetName("PLAYER\0");
	//opponent->SetName("BLUE\0");

}

CGame::~CGame()
{
	delete TextPrinter::GetInstance();
	delete StateManagerC::GetInstance();
	delete PokemonManager::GetInstance();
	delete BattleUIManager::GetInstance();
	
}

bool CGame::Initialize (GL_Window* window, Keys* keys)
{
	//initOpenGLDrawing(window,keys, 0.0f, 0.0f, 0.0f);
	initOpenGLDrawing(window, keys, 0.973, 0.973, 0.973);										//White Background

	TextPrinter::GetInstance()->init();
	StateManagerC::GetInstance()->init();
	PokemonManager::GetInstance()->init();

	Sound::Init();

	InputHandler::init();

	
	srand(time(NULL));

	player =  new Trainer("PLAYER\0", 0);

	int enemyTrainer = rand() % (TRAINER_IMAGES - 1);
	enemyTrainer++;

	opponent = new Trainer(getNameByImage(enemyTrainer), enemyTrainer);

	//Generate random team for player
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		int index = ((float)rand() / RAND_MAX) * NUMBER_OF_POKEMON;
		if (index == 0) { index++; }
		player->CatchPokemon(PokemonManager::GetPokemon(index));

	}

	//Generate random team for opponent
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		int index = ((float)rand() / RAND_MAX) * NUMBER_OF_POKEMON;
		if (index == 0) { index++; }
		opponent->CatchPokemon(PokemonManager::GetPokemon(index));

	}
	

	BattleUIManager::GetInstance()->init(CGame::GetScreenWidth(), CGame::GetScreenHeight());

	//BattleUIManager::GetInstance()->render(player, opponent);

	return true;
}

void CGame::Deinitialize() {
	StateManagerC::GetInstance()->shutdown();
	TextPrinter::GetInstance()->shutdown();

	BattleUIManager::GetInstance()->shutdown();
	InputHandler::shutdown();
	Sound::Shutdown();

	player->shutdown();
	opponent->shutdown();

	delete player;
	player = nullptr;
	delete opponent;
	opponent = nullptr;

}

void CGame::UpdateFrame(uint32_t milliseconds)			
{
	if (g_keys->keyDown[VK_ESCAPE])								// Is ESC Being Pressed?
	{
		TerminateApplication(g_window);						// Terminate The Program
	}

	if (g_keys->keyDown[VK_F1])									// Is F1 Being Pressed?
	{
		ToggleFullscreen(g_window);							// Toggle Fullscreen Mode
	}

	InputHandler::Update(milliseconds, player, opponent);
	

}

void CGame::DrawScene(void)											
{
	startOpenGLDrawing();

	// this is copying player & opponent every frame... likely undesirable
	BattleUIManager::GetInstance()->render(*player, *opponent);


}
