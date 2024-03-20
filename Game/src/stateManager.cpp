#define STATE_MANAGER_CPP
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

#include "stateManager.h"

#include "BattleUIManager.h"

StateManagerC* StateManagerC::sInstance=NULL;


StateManagerC *StateManagerC::CreateInstance()
{
	if(sInstance!=NULL)return sInstance;
	else

	sInstance = new StateManagerC();
	return sInstance;
}

void StateManagerC::init()
{

	mCurrentState = MENU;
	
}

int32_t StateManagerC::getState()
{
	return mCurrentState;
}


int32_t StateManagerC::progressState()					//Progress to the next state
{

	if (mCurrentState == ACTION)
	{
		mCurrentState = OPPONENT;
	}
	else if (mCurrentState == OPPONENT)
	{
		mCurrentState = ACTION;
	}
	else
	{
		mCurrentState = ACTION;
	}
	
	return mCurrentState;
}

int32_t StateManagerC::startState()						//Start the state
{
	return 0;
}

int32_t StateManagerC::exitState()						//Exit the State
{
	return 0;
}

void StateManagerC::setState(int32_t state)
{
	mCurrentState = state;
}

//Set all base values
int32_t StateManagerC::executeTurn(TurnActor actor)
{

	currActor = actor;

	mCurrentState = BATTLE_TEXT;
	mCurrTextState = CALCULATE_DAMAGE;
	firstActorDone = false;

	return mCurrentState;

}

int32_t	StateManagerC::executeTurn(TurnActor actor, B_Text_State startTextState)
{
	currActor = actor;

	mCurrentState = BATTLE_TEXT;
	mCurrTextState = startTextState;
	firstActorDone = false;

	return mCurrentState;
}

int32_t StateManagerC::progressBattleText()
{
	int newState = (int)mCurrTextState + 1;
	mCurrTextState = (B_Text_State)newState;

	return mCurrTextState;
	
}

int StateManagerC::getBattleTextState()
{
	return mCurrTextState;
}

bool StateManagerC::allActorsTookTurn()
{
	if (firstActorDone)
	{
		return true;
	}

	return false;
}


StateManagerC::TurnActor StateManagerC::nextActorTurn()
{
	mCurrTextState = B_Text_State::CALCULATE_DAMAGE;
	firstActorDone = true;

	if (currActor == TurnActor::T_PLAYER)				//Player was first? Now opponent turn
	{
		currActor = TurnActor::T_OPPONENT;
		return currActor;
	}
	else
	{
		currActor = TurnActor::T_PLAYER;
		return currActor;
	}

}


void StateManagerC::shutdown()
{

}