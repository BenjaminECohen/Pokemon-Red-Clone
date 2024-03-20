#define BATTLE_UI_MANAGER
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"
#include <assert.h>
#include <string.h>


#include "BattleUIManager.h"
#include "TextPrinter.h"


#include "SOIL.h"

#define UISheetRows		(3)
#define UISheetCols		(3)

#define MenuSheetRows	(3)
#define MenuSheetCols	(3)

#define ACTION_INDEX_MAX (3)
#define SYMBOL_SIZE (24)

#define TEXT_BOX_HEIGHT (144)

#define PKMN_SPRITESHEET_SIZE (9)
#define FRONT_SIZE (168)
#define BACK_SIZE (192)

#define TRAINER_SHEET_SIZE (3)

#define BALL_COUNTER_LENGTH (6)
#define ITEM_HOLD_MAX (99)
#define TYPE_LENGTH_MAX (8)
#define POWER_POINT_MAX (2)
#define HEALTH_POINT_MAX (3)

#define HP_BAR_WIDTH	(144)
#define HP_BAR_HEIGHT	(24)

#define ICON_SHEET_HEIGHT	(13)
#define LEVEL_ICON_INDEX	(1)
#define DOWN_ARROW_INDEX	(2)
#define RIGHT_SELAR_INDEX	(3)
#define RIGHT_ARROW_INDEX	(4)

#define PKMN_SLOTF_INDEX	(12)
#define PKMN_SLOTE_INDEX	(10)
#define PKMN_SLOTD_INDEX	(6)

#pragma region BattleTexture UI Indexes
#define CHOOSE_PKMN			(0)
#define PKMN_LIST_SELECT	(1)
#define PKMN_LIST_SWITCH    (2)
#define PKMN_MOVE_LIST		(3)
#define BAG_DISPLAY			(4)
#define STATUS_DISP_MOVES	(5)
#define BAT_INTRO_TEX		(6)
#define BASE_DISPLAY		(7)
#define STATUS_DISP_STAT	(8)

#pragma endregion


BattleUIManager* BattleUIManager::sInstance = NULL;				//Set the instance to NULL


BattleUIManager* BattleUIManager::CreateInstance()
{
	if (sInstance != NULL)
	{
		return sInstance;
	}
	else
	{
		sInstance = new BattleUIManager();
		sInstance->mCurrentState = TEXT;

		return sInstance;
	}
}


void BattleUIManager::init(int width, int height)
{
	maxWidth			= width;
	maxHeight			= height;

	maxXCoord			= (float)width;
	maxYCoord			= (float)height;

	mBattleTextureMap	= SOIL_load_OGL_texture("asset/BattleUITexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	mSymbolMap			= SOIL_load_OGL_texture("asset/BattleUISymbols.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	mPokemonFronts		= SOIL_load_OGL_texture("asset/PkmnFrontSprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	mPokemonBacks		= SOIL_load_OGL_texture("asset/PkmnBackSprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	mMenuSheet			= SOIL_load_OGL_texture("asset/MenuSheet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	mMenuSprites		= SOIL_load_OGL_texture("asset/MenuSprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	mTrainers			= SOIL_load_OGL_texture("asset/TrainersSheet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	mPlayer				= SOIL_load_OGL_texture("asset/Player.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	mCurrentState		= MENU;


}

void BattleUIManager::render(Trainer player, Trainer opponent)
{
	Pokemon* pPokemon = player.GetCurrPokemon();
	Pokemon* oPokemon = opponent.GetCurrPokemon();

	if (renderPlayerPokemon)
	{
		DrawPokemon(*pPokemon, 24, 120, true);
	}
	if (renderOpponentPokemon)
	{
		DrawPokemon(*oPokemon, 288, 264, false);
	}
	
	if (mCurrentState == MENU)
	{
		DrawPokemon(PokemonManager::GetPokemon(6), 120, 24, false);
		DrawMenuUI(8);
	}
	else if (mCurrentState == TEXT)
	{
		if (mTextState == INTRO_TEXT)
		{

			if (renderPlayerSprite)
			{
				DrawCharacterSprite(player, 24, 120);
			}
			if (renderOpponentSprite)
			{
				DrawCharacterSprite(opponent, 288, 264);
			}

			//Draw Player Team Icons
			GLfloat xPos = 264;
			GLfloat yPos = 168;

			for (int i = 0; i < MAX_TEAM_SIZE; i++, xPos += SYMBOL_SIZE)
			{
				Pokemon* p = player.GetPokemon(i);

				if (p->GetIndex() != 0)
				{
					if (p->GetCurrHealth() != 0)
					{
						DrawSpecialIcon(xPos, yPos, PKMN_SLOTF_INDEX);
					}
					else
					{
						DrawSpecialIcon(xPos, yPos, PKMN_SLOTD_INDEX);
					}
				}
				else
				{
					DrawSpecialIcon(xPos, yPos, PKMN_SLOTE_INDEX);
				}

			}


			//Draw Enemy Team Icons
			xPos = 72;
			yPos = 360;
			for (int i = MAX_TEAM_SIZE - 1; i >= 0; i--, xPos += SYMBOL_SIZE)
			{
				Pokemon* p = opponent.GetPokemon(i);

				if (p->GetIndex() != 0)
				{
					if (p->GetCurrHealth() != 0)
					{
						DrawSpecialIcon(xPos, yPos, PKMN_SLOTF_INDEX);
					}
					else
					{
						DrawSpecialIcon(xPos, yPos, PKMN_SLOTD_INDEX);
					}
				}
				else
				{
					DrawSpecialIcon(xPos, yPos, PKMN_SLOTE_INDEX);
				}

			}

			TextPrinter::PrintSpecial(TextPrinter::PLAYER_PKMN_TEXTBOX, 0, 0);
			DrawBattleUI(mCurrentState);

			TextPrinter::PrintTextAt(line1, 24, 72, 18);
			TextPrinter::PrintTextAt(line2, 24, 24, 18);

		}
		else if (mTextState == BATTLE_TEXT)
		{
			
			
			//Print names of pokemon to screen
			TextPrinter::PrintTextAt(pPokemon->GetName(), 240, 240, PKMN_NAME_MAX);				//Player Pokemon Name
			TextPrinter::PrintTextAt(oPokemon->GetName(), 24, 408, PKMN_NAME_MAX);				//Opponent Pokemon Name

			

			//Draw Health Bars of both active pokemon
			if (tempHealthActive)						//If temporary health is on (animation is playing)
			{
				if (isPlayerTempHealth)
				{
					TextPrinter::PrintNumber((uint16_t)playerTempHealth, 264, 168, 3, false);
					TextPrinter::PrintNumber(pPokemon->GetMaxHealth(), 360, 168, 3, false);

					DrawTempHealthBar(playerTempHealth, *pPokemon, 288, 192);
					DrawHealthBar(*oPokemon, 96, 360);
				}
				else
				{
					TextPrinter::PrintNumber(pPokemon->GetCurrHealth(), 264, 168, 3, false);
					TextPrinter::PrintNumber(pPokemon->GetMaxHealth(), 360, 168, 3, false);

					DrawHealthBar(*pPokemon, 288, 192);
					DrawTempHealthBar(opponentTempHealth, *oPokemon, 96, 360);
					
				}
				
			}
			else
			{
				//Print Player Health Values
				TextPrinter::PrintNumber(pPokemon->GetCurrHealth(), 264, 168, 3, false);
				TextPrinter::PrintNumber(pPokemon->GetMaxHealth(), 360, 168, 3, false);

				DrawHealthBar(*pPokemon, 288, 192);
				DrawHealthBar(*oPokemon, 96, 360);
			}
			

			//Draw Player Level
			DrawSpecialIcon(336, 216, LEVEL_ICON_INDEX);
			TextPrinter::PrintNumber(pPokemon->GetLevel(), 360, 216, 2, false, true);


			//Draw Enemy Level
			DrawSpecialIcon(96, 384, LEVEL_ICON_INDEX);
			TextPrinter::PrintNumber(oPokemon->GetLevel(), 120, 384, 2, false, true);

			//Draw Special UI
			DrawTextUI(mTextState);
			TextPrinter::PrintSpecial(TextPrinter::PLAYER_PKMN_TEXTBOX, 0, 0);

			//Draw words to screen
			TextPrinter::PrintTextAt(line1, 24, 72, 18);
			TextPrinter::PrintTextAt(line2, 24, 24, 18);

		}
		else if (mTextState == END_TEXT)
		{

		}


	}	
	else if (mCurrentState == BASE)
	{	
		
		//Print names of pokemon to screen
		TextPrinter::PrintTextAt(pPokemon->GetName(), 240, 240, PKMN_NAME_MAX);				//Player Pokemon Name
		TextPrinter::PrintTextAt(oPokemon->GetName(), 24, 408, PKMN_NAME_MAX);				//Opponent Pokemon Name

		//Print Player Health Values
		TextPrinter::PrintNumber(pPokemon->GetCurrHealth(), 264, 168, 3, false);
		TextPrinter::PrintNumber(pPokemon->GetMaxHealth(), 360, 168, 3, false);

		//Draw Health Bars of both active pokemon
		DrawHealthBar(*pPokemon, 288, 192);
		DrawHealthBar(*oPokemon, 96, 360);

		//Draw Player Level
		DrawSpecialIcon(336, 216, LEVEL_ICON_INDEX);
		TextPrinter::PrintNumber(pPokemon->GetLevel(), 360, 216, 2, false, true);


		//Draw Enemy Level
		DrawSpecialIcon(96, 384, LEVEL_ICON_INDEX);
		TextPrinter::PrintNumber(oPokemon->GetLevel(), 120, 384, 2, false, true);

		//Draw NavigationPointer
		DrawPointers(mCurrentState);

		//Draw the rest of the static UI
		DrawBattleUI(mCurrentState);

		//Special Print to cover this missing part of the ui section
		TextPrinter::PrintSpecial(TextPrinter::PLAYER_PKMN_BASE, 0, 0);
	}
	else if (mCurrentState == MOVELIST)
	{
		//TextPrinter::PrintSpecial(TextPrinter::PLAYER_PKMN_BASE, 0, 0);
		TextPrinter::PrintTextAt(pPokemon->GetName(), 240, 240, PKMN_NAME_MAX);				//Player Pokemon Name
		TextPrinter::PrintTextAt(oPokemon->GetName(), 24, 408, PKMN_NAME_MAX);				//Opponent Pokemon Name

		//glColor4ub(0, 0xFF, 0xFF, 0xFF);
		TextPrinter::PrintNumber(pPokemon->GetCurrHealth(), 264, 168, 3, false);
		//glColor4ub(0, 0xFF, 0, 0xFF);
		TextPrinter::PrintNumber(pPokemon->GetMaxHealth(), 360, 168, 3, false);

		DrawHealthBar(*pPokemon, 288, 192);
		DrawHealthBar(*oPokemon, 96, 360);

		//Draw Player Level
		DrawSpecialIcon(336, 216, LEVEL_ICON_INDEX);
		TextPrinter::PrintNumber(pPokemon->GetLevel(), 360, 216, 2, false, true);


		//Draw Enemy Level
		DrawSpecialIcon(96, 384, LEVEL_ICON_INDEX);
		TextPrinter::PrintNumber(oPokemon->GetLevel(), 120, 384, 2, false, true);


		DrawBattleUI(mCurrentState);
		

		GLfloat yPos = 96;
		for (int i = 0; i <= MOVE_INDEX_MAX; i++)
		{
			
			MoveT move = *pPokemon->GetMove(i);
			if (move.move.MoveExists())
			{
				TextPrinter::PrintTextAt(pPokemon->GetMove(i)->move.GetName(), 144, yPos - (i * 24), 12);

				if (i == moveIndex)
				{
					TextPrinter::PrintNumber(pPokemon->GetMove(i)->currPP, 120, 144, 2, true);
					TextPrinter::PrintNumber(pPokemon->GetMove(i)->move.GetPP(), 192, 144, 2, true);
					TextPrinter::PrintTextAt(ptos(pPokemon->GetMove(i)->move.GetType()), 48, 168, TYPE_LENGTH_MAX);
				}
				
			}
			else
			{
				TextPrinter::PrintSpecial(TextPrinter::MOVE_NULL, 144, yPos - (i * 24));
			}
		}

		DrawPointers(mCurrentState);


	}
	else if (mCurrentState == BAG)
	{
		//Print Opponent name
		TextPrinter::PrintTextAt(oPokemon->GetName(), 24, 408, PKMN_NAME_MAX);

		//Draw Health Bars of opponent active pokemon
		DrawHealthBar(*oPokemon, 96, 360);

		//Draw Enemy Level
		DrawSpecialIcon(96, 384, LEVEL_ICON_INDEX);
		TextPrinter::PrintNumber(oPokemon->GetLevel(), 120, 384, 2, false, true);

		TextPrinter::PrintSpecial(TextPrinter::PLAYER_PKMN_TEXTBOX, 0, 0);
		DrawBattleUI(mCurrentState);

		TextPrinter::PrintTextAt("CANCEL\0", 144, 312, 13);
		DrawPointers(mCurrentState);
		DrawRect(120, 168, 432, 312);

	}

	else if (mCurrentState == POKEMONLIST)
	{

		int partySize = 0;
		GLfloat xPos = 72;
		GLfloat yPos = 408;

		for (int i = 0;
			i < MAX_TEAM_SIZE;
			i++, yPos -= 48)
		{
			Pokemon* p = player.GetPokemon(i);
			//If null, we are at the end of the list
			if (p->GetIndex() == 0)
			{
				partySize = i;
				break;
			}
			else if (i == MAX_TEAM_SIZE - 1)
			{
				partySize = 6;
			}


			
			//Otherwise, print relevant data
			TextPrinter::PrintTextAt(p->GetName(), xPos, yPos, PKMN_NAME_MAX);	

			DrawSpecialIcon(xPos + (SYMBOL_SIZE * 4) + HP_BAR_WIDTH, yPos, LEVEL_ICON_INDEX);
			TextPrinter::PrintNumber(p->GetLevel(), xPos + (SYMBOL_SIZE * 5) + HP_BAR_WIDTH, yPos, 2, false, true);

			TextPrinter::PrintNumber(p->GetCurrHealth(), xPos + (SYMBOL_SIZE * 4) + HP_BAR_WIDTH, yPos - SYMBOL_SIZE, 3, false);
			TextPrinter::PrintNumber(p->GetMaxHealth(), xPos + (SYMBOL_SIZE * 8) + HP_BAR_WIDTH, yPos - SYMBOL_SIZE, 3, false);

			DrawHealthBar(*p, xPos + (SYMBOL_SIZE * 3), yPos - SYMBOL_SIZE);
			
			DrawRect(0, 144, 72, 480);
			


		}

		DrawBattleUI(mCurrentState);
		//FILL ZONE
		
		
		DrawRect(0, 144, 480, 144 + ((48 * (MAX_TEAM_SIZE - partySize))));

		DrawPointers(mCurrentState);
		
		
	}
	

	
	

}

void BattleUIManager::shutdown()
{

}



uint32_t BattleUIManager::getState()
{
	return mCurrentState;
}

uint32_t BattleUIManager::setState(uint32_t mode)
{
	assert(mode < MODE_MAX && mode >= 0);
	mLastState = mCurrentState;
	mCurrentState = mode;
	return mCurrentState;
}



void BattleUIManager::DrawCharacterSprite(Trainer trainer, GLfloat xPosLeft, GLfloat yPosBot)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	GLfloat spriteSize;
	GLfloat uvOffset;
	uint16_t pkmnIndex;

	GLfloat row;
	GLfloat column;

	GLfloat uPos;
	GLfloat vPos;

	


	if (trainer.GetImageIndex() == 0)
	{
		glBindTexture(GL_TEXTURE_2D, mPlayer);
		spriteSize = BACK_SIZE;

		uvOffset = 1.0;
		uPos = 0;
		vPos = 0;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, mTrainers);
		spriteSize = FRONT_SIZE;

		uvOffset = 1.0 / TRAINER_SHEET_SIZE;
		pkmnIndex = trainer.GetImageIndex();
		row = pkmnIndex / TRAINER_SHEET_SIZE;
		column = pkmnIndex % TRAINER_SHEET_SIZE;
		uPos = column * uvOffset;
		vPos = row * uvOffset;
	}


	glBegin(GL_QUADS);



	//glColor4ub(0xFF, 0, 0, 0xFF); //Dye red for testing
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glTexCoord2f(uPos, vPos);
	glVertex3f(xPosLeft, yPosBot, 0.0);
	glTexCoord2f(uPos, vPos + uvOffset);
	glVertex3f(xPosLeft, yPosBot + spriteSize, 0.0);
	glTexCoord2f(uPos + uvOffset, vPos + uvOffset);
	glVertex3f(xPosLeft + spriteSize, yPosBot + spriteSize, 0.0);
	glTexCoord2f(uPos + uvOffset, vPos);
	glVertex3f(xPosLeft + spriteSize, yPosBot, 0.0);

	glEnd();
}

void BattleUIManager::DrawPokemon(Pokemon pokemon, GLfloat xPosLeft, GLfloat yPosBot, bool isPlayer)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	GLfloat spriteSize;
	GLfloat uvOffset;
	uint16_t pkmnIndex;

	GLfloat row;
	GLfloat column;

	GLfloat uPos;
	GLfloat vPos;


	if (isPlayer)
	{
		glBindTexture(GL_TEXTURE_2D, mPokemonBacks);
		spriteSize = BACK_SIZE;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, mPokemonFronts);
		spriteSize = FRONT_SIZE;
	}

	uvOffset = 1.0 / PKMN_SPRITESHEET_SIZE;
	pkmnIndex = pokemon.GetIndex() - 1;
	row = pkmnIndex / PKMN_SPRITESHEET_SIZE;
	column = pkmnIndex % PKMN_SPRITESHEET_SIZE;

	uPos = column * uvOffset;
	vPos = row * uvOffset;

	
	
	glBegin(GL_QUADS);

	

	//glColor4ub(0xFF, 0, 0, 0xFF); //Dye red for testing
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glTexCoord2f(uPos, vPos);
	glVertex3f(xPosLeft, yPosBot, 0.0);
	glTexCoord2f(uPos, vPos + uvOffset);
	glVertex3f(xPosLeft, yPosBot + spriteSize, 0.0);
	glTexCoord2f(uPos + uvOffset, vPos + uvOffset);
	glVertex3f(xPosLeft + spriteSize, yPosBot + spriteSize, 0.0);
	glTexCoord2f(uPos + uvOffset, vPos);
	glVertex3f(xPosLeft + spriteSize, yPosBot, 0.0);

	glEnd();
}


void BattleUIManager::DrawPointers(int32_t mode)
{
	
	if (mode == BASE)
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, mSymbolMap);
		glBegin(GL_QUADS);

		//0,0 bot left

		GLfloat xPosLeft = 216;
		GLfloat xPosRight = xPosLeft + SYMBOL_SIZE;
		GLfloat yPosBot = 24;
		GLfloat yPosTop = yPosBot + SYMBOL_SIZE;


		GLfloat indexOffsetX = 144;
		GLfloat indexOffsetY = 48;

		char index = getActionIndex();
		switch (index)
		{
			case (0):
				yPosBot += indexOffsetY;
				yPosTop += indexOffsetY;
				break;
			case (1):
				yPosBot += indexOffsetY;
				yPosTop += indexOffsetY;
				xPosLeft += indexOffsetX;
				xPosRight += indexOffsetX;
				break;
			case (2):
				//Do nothing
				break;
			case (3):
				xPosLeft += indexOffsetX;
				xPosRight += indexOffsetX;
				break;
			default:
				printf("Shouldn't be here\n");
				break;
		}

		
		GLfloat vOffset = (1.0 / 13.0);							//There are 13 sections of the image
		GLfloat vPos = vOffset * RIGHT_ARROW_INDEX;

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF); //Dye red for testing
		glTexCoord2f(0, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(0, vPos + vOffset);
		glVertex3f(xPosLeft, yPosTop, 0.0);
		glTexCoord2f(1.0, vPos + vOffset);
		glVertex3f(xPosRight, yPosTop, 0.0);
		glTexCoord2f(1.0, vPos);
		glVertex3f(xPosRight, yPosBot, 0.0);
		
		glEnd();

	}

	if (mode == MOVELIST)
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, mSymbolMap);
		glBegin(GL_QUADS);

		//0,0 bot left

		GLfloat xPosLeft = 120;
		GLfloat yPosBot = 96;
		GLfloat letterSize = 24;

		yPosBot -= letterSize * (GLfloat)moveIndex;


		GLfloat vOffset = (1.0 / ICON_SHEET_HEIGHT);							//There are 13 sections of the image
		GLfloat vPos = vOffset * RIGHT_ARROW_INDEX;

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF); //Dye red for testing
		glTexCoord2f(0, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(0, vPos + vOffset);
		glVertex3f(xPosLeft, yPosBot + letterSize, 0.0);
		glTexCoord2f(1.0, vPos + vOffset);
		glVertex3f(xPosLeft + letterSize, yPosBot + letterSize, 0.0);
		glTexCoord2f(1.0, vPos);
		glVertex3f(xPosLeft + letterSize, yPosBot, 0.0);

		glEnd();


	}

	if (mode == POKEMONLIST)
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, mSymbolMap);
		glBegin(GL_QUADS);

		//0,0 bot left

		GLfloat xPosLeft = 0;
		GLfloat yPosBot = 384;

		yPosBot -= SYMBOL_SIZE * (GLfloat)teamIndex * 2.0;


		GLfloat vOffset = (1.0 / ICON_SHEET_HEIGHT);							//There are 13 sections of the image
		GLfloat vPos = vOffset * RIGHT_ARROW_INDEX;
		
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		//glColor4ub(0xFF, 0, 0xFF, 0xFF); //Dye red for testing
		glTexCoord2f(0, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(0, vPos + vOffset);
		glVertex3f(xPosLeft, yPosBot + SYMBOL_SIZE, 0.0);
		glTexCoord2f(1.0, vPos + vOffset);
		glVertex3f(xPosLeft + SYMBOL_SIZE, yPosBot + SYMBOL_SIZE, 0.0);
		glTexCoord2f(1.0, vPos);
		glVertex3f(xPosLeft + SYMBOL_SIZE, yPosBot, 0.0);

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

		glEnd();
	}

	if (mode == BAG)
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, mSymbolMap);
		glBegin(GL_QUADS);

		//0,0 bot left

		GLfloat xPosLeft = 120;
		GLfloat yPosBot = 312;


		GLfloat vOffset = (1.0 / ICON_SHEET_HEIGHT);							//There are 13 sections of the image
		GLfloat vPos = vOffset * RIGHT_ARROW_INDEX;

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		//glColor4ub(0xFF, 0, 0xFF, 0xFF); //Dye red for testing
		glTexCoord2f(0, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(0, vPos + vOffset);
		glVertex3f(xPosLeft, yPosBot + SYMBOL_SIZE, 0.0);
		glTexCoord2f(1.0, vPos + vOffset);
		glVertex3f(xPosLeft + SYMBOL_SIZE, yPosBot + SYMBOL_SIZE, 0.0);
		glTexCoord2f(1.0, vPos);
		glVertex3f(xPosLeft + SYMBOL_SIZE, yPosBot, 0.0);

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

		glEnd();
	}

}


void BattleUIManager::UseTempHealth(float tempHP, bool isPlayer)
{
	tempHealthActive = true;
	if (isPlayer)
	{
		isPlayerTempHealth = true;
		playerTempHealth = tempHP;
	}
	else
	{
		isPlayerTempHealth = false;
		opponentTempHealth = tempHP;
	}
}

void BattleUIManager::UpdateTempHealth(float tempHP, bool isPlayer)
{
	if (isPlayer)
	{
		playerTempHealth = tempHP;
	}
	else
	{
		opponentTempHealth = tempHP;
	}
}

void BattleUIManager::DisableTempHealth()
{
	tempHealthActive = false;
}

void BattleUIManager::DrawTempHealthBar(float tempHP, Pokemon pokemon, GLfloat xPos, GLfloat yPos)
{
	float hpFraction = tempHP / (float)pokemon.GetMaxHealth();

	GLfloat fractionLength = hpFraction * HP_BAR_WIDTH;


	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mBattleTextureMap);
	glBegin(GL_QUADS);

	GLfloat uOffset = 1.0 / (480.0 * 3.0);
	GLfloat vOffset = 1.0 / (432.0 * 3.0);

	//glColor4ub(0xFF, 0, 0xFF, 0xFF);
	glColor4ub(0x60, 0x60, 0x60, 0xFF);

	glTexCoord2f(uOffset, vOffset);
	glVertex3f(xPos, yPos, 0.0);
	glTexCoord2f(uOffset, vOffset + vOffset);
	glVertex3f(xPos, yPos + HP_BAR_HEIGHT, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset + vOffset);
	glVertex3f(xPos + fractionLength, yPos + HP_BAR_HEIGHT, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset);
	glVertex3f(xPos + fractionLength, yPos, 0.0);


	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnd();
}

void BattleUIManager::DrawHealthBar(Pokemon pokemon, GLfloat xPos, GLfloat yPos)
{
	
	float hpFraction = (float)pokemon.GetCurrHealth() / (float)pokemon.GetMaxHealth();

	GLfloat fractionLength = hpFraction * HP_BAR_WIDTH;

	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mBattleTextureMap);
	glBegin(GL_QUADS);

	GLfloat uOffset = 1.0 / (480.0 * 3.0);
	GLfloat vOffset = 1.0 / (432.0 * 3.0);
	
	//glColor4ub(0xFF, 0, 0xFF, 0xFF);
	glColor4ub(0x60, 0x60, 0x60, 0xFF);

	glTexCoord2f(uOffset, vOffset);
	glVertex3f(xPos, yPos, 0.0);
	glTexCoord2f(uOffset, vOffset + vOffset);
	glVertex3f(xPos, yPos + HP_BAR_HEIGHT, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset + vOffset);
	glVertex3f(xPos + fractionLength, yPos + HP_BAR_HEIGHT, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset);
	glVertex3f(xPos + fractionLength, yPos, 0.0);


	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnd();
}

void BattleUIManager::DrawPokemonIcon(int index, GLfloat xPos, GLfloat yPos)
{

}

void BattleUIManager::DrawRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mBattleTextureMap);
	glBegin(GL_QUADS);

	GLfloat uOffset = 1.0 / (480.0 * 3.0);
	GLfloat vOffset = 1.0 / (432.0 * 3.0);


	//glColor4ub(0xf8, 0xf8, 0xf8, 0xFF);
	//glColor4ub(0, 0, 0xFF, 0xFF);

	glTexCoord2f(uOffset, vOffset);
	glVertex3f(x1, y1, 0.0);
	glTexCoord2f(uOffset, vOffset + vOffset);
	glVertex3f(x1, y2, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset + vOffset);
	glVertex3f(x2, y2, 0.0);
	glTexCoord2f(uOffset + uOffset, vOffset);
	glVertex3f(x2, y1, 0.0);

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glEnd();


}

void BattleUIManager::DrawSpecialIcon(GLfloat xPos, GLfloat yPos, GLfloat iconIndex)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mSymbolMap);
	glBegin(GL_QUADS);

	GLfloat vOffset = 1.0 / ICON_SHEET_HEIGHT;
	GLfloat vPos = vOffset * iconIndex;

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glTexCoord2f(0, vPos);
	glVertex3f(xPos, yPos, 0.0);
	glTexCoord2f(0, vPos + vOffset);
	glVertex3f(xPos, yPos + SYMBOL_SIZE, 0.0);
	glTexCoord2f(1.0, vPos + vOffset);
	glVertex3f(xPos + SYMBOL_SIZE, yPos + SYMBOL_SIZE, 0.0);
	glTexCoord2f(1.0, vPos);
	glVertex3f(xPos + SYMBOL_SIZE, yPos, 0.0);

	glEnd();
}


void BattleUIManager::DrawBattleUI(int32_t mode)
{
	
	

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mBattleTextureMap);
	glBegin(GL_QUADS);

	GLfloat xPosLeft = 0;
	GLfloat xPosRight = maxXCoord;
	GLfloat yPosTop = maxYCoord;
	GLfloat yPosBot = 0;

	GLfloat uOffset = 1.0 / UISheetRows;
	GLfloat vOffset = 1.0 / UISheetCols;

	GLfloat uPos = 0;
	GLfloat vPos = 0;

	
	int index = getModeTextureIndex(mode);
	GLfloat row = index / UISheetRows;
	GLfloat col = index % UISheetCols;

	uPos = col / UISheetCols;
	vPos = row / UISheetRows;


	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	
	glTexCoord2f(uPos, vPos);
	glVertex3f(xPosLeft, yPosBot, 0.0);
	glTexCoord2f(uPos, vPos + vOffset);
	glVertex3f(xPosLeft, yPosTop, 0.0);
	glTexCoord2f(uPos + uOffset, vPos + vOffset);
	glVertex3f(xPosRight, yPosTop, 0.0);
	glTexCoord2f(uPos + uOffset, vPos);
	glVertex3f(xPosRight, yPosBot, 0.0);

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glEnd();




}

int BattleUIManager::getModeTextureIndex(int mode)
{


	switch (mode)
	{
		case (TEXT):
			return BAT_INTRO_TEX;
		case (BASE):
			return BASE_DISPLAY;
		case (STATS_STATS):
			return STATUS_DISP_STAT;
		case (MOVELIST):
			return PKMN_MOVE_LIST;
		case (BAG):
			return BAG_DISPLAY;
		case (STATS_MOVES):
			return STATUS_DISP_MOVES;
		case (POKEMONLIST):
			return CHOOSE_PKMN;
		case (POKEMONLIST_SELECT):
			return PKMN_LIST_SELECT;
		case (POKEMONLIST_SWITCH):
			return PKMN_LIST_SWITCH;

		default:
			return BAT_INTRO_TEX;

	}

}


void BattleUIManager::DrawTextUI(int textSubState)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mBattleTextureMap);
	glBegin(GL_QUADS);


	GLfloat xPosLeft = 0;
	GLfloat xPosRight = maxXCoord;
	GLfloat yPosTop = maxYCoord;
	GLfloat yPosBot = 0;

	GLfloat uOffset = 1.0 / UISheetRows;
	GLfloat vOffset = 1.0 / UISheetCols;

	GLfloat uPos = 0;
	GLfloat vPos = 0;

	if (textSubState == BATTLE_TEXT)
	{
		GLfloat row = UISheetRows - 1;
		GLfloat col = 1;

		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		//glColor4ub(0xA6, 0x05, 0x44, 0xFF);
		//Print Battle UI above text box

		uPos = col / UISheetCols;
		vPos = row / UISheetRows;

		GLfloat vPosBotOffset = 1008.f / 1296.f;			//Textbox height + height under it divided by total height of sheet

		GLfloat yOffset = TEXT_BOX_HEIGHT;

		glTexCoord2f(uPos, vPosBotOffset);
		glVertex3f(xPosLeft, yPosBot + yOffset, 0.0);
		glTexCoord2f(uPos, vPos + vOffset);
		glVertex3f(xPosLeft, yPosTop, 0.0);
		glTexCoord2f(uPos + uOffset, vPos + vOffset);
		glVertex3f(xPosRight, yPosTop, 0.0);
		glTexCoord2f(uPos + uOffset, vPosBotOffset);
		glVertex3f(xPosRight, yPosBot + yOffset, 0.0);


		//Print Text Box
		//glColor4ub(0xFF, 0x52, 0x19, 0xFF);
		
		col = 0;

		uPos = col / UISheetCols;
		vPos = row / UISheetRows;

		glTexCoord2f(uPos, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(uPos, vPosBotOffset);
		glVertex3f(xPosLeft, yOffset, 0.0);
		glTexCoord2f(uPos + uOffset, vPosBotOffset);
		glVertex3f(xPosRight, yOffset, 0.0);
		glTexCoord2f(uPos + uOffset, vPos);
		glVertex3f(xPosRight, yPosBot, 0.0);

		

		//glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		
	}

	glEnd();
}

void BattleUIManager::DrawMenuUI(int screenIndex)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mMenuSheet);
	glBegin(GL_QUADS);

	GLfloat xPosLeft = 0;
	GLfloat xPosRight = maxXCoord;
	GLfloat yPosTop = maxYCoord;
	GLfloat yPosBot = 0;

	GLfloat uOffset = 1.0 / MenuSheetRows;
	GLfloat vOffset = 1.0 / MenuSheetCols;

	GLfloat uPos = 0;
	GLfloat vPos = 0;

	int offset = screenIndex / MenuSheetRows;
	GLfloat row = MenuSheetRows - offset - 1;
	GLfloat col = screenIndex % MenuSheetCols;

	uPos = col / MenuSheetCols;
	vPos = row / MenuSheetRows;


	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glTexCoord2f(uPos, vPos);
	glVertex3f(xPosLeft, yPosBot, 0.0);
	glTexCoord2f(uPos, vPos + vOffset);
	glVertex3f(xPosLeft, yPosTop, 0.0);
	glTexCoord2f(uPos + uOffset, vPos + vOffset);
	glVertex3f(xPosRight, yPosTop, 0.0);
	glTexCoord2f(uPos + uOffset, vPos);
	glVertex3f(xPosRight, yPosBot, 0.0);

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glEnd();
}


void BattleUIManager::SetTextLine(const char* string, int line)
{
	assert(line > 0 && line <= 2);

	if (line == 1)
	{
		strcpy(line1,string);
	}
	else
	{
		strcpy(line2, string);
	}

}

uint32_t BattleUIManager::cancelMode()
{
	switch (mCurrentState)
	{
	case (BASE):
		return 0;
		break;

		//FALL THROUGH FOR ALL CASES THAT GO BACK TO BASE
	case (MOVELIST):

	case (BAG):

	case (POKEMONLIST):
		setState(BASE);
		break;

	case (TEXT):
		return 0;			//Cannot go back
		break;

	default:
		printf("Shouldn't be here!\n");
		break;

	}
	return 1;
}



#pragma region Indexer Functions
char BattleUIManager::moveActionIndex(char indexChange)
{
	if (actionIndex + indexChange > ACTION_INDEX_MAX || actionIndex + indexChange < 0)		//Dont allow it to go out of range
	{
		return -1;
	}
	actionIndex += indexChange;

	return actionIndex;

}

char BattleUIManager::setMoveIndexMax(Pokemon playerCurrPokemon)
{
	int index = 0;
	for (int i = 0; i <= MOVE_INDEX_MAX; i++)
	{

		MoveT* move = playerCurrPokemon.GetMove(i);

		if (!move->move.MoveExists())									//If false, break we have our max moves we can use
		{
			index = --i;
			break;
		}
		if (i == MOVE_INDEX_MAX)
		{
			index = MOVE_INDEX_MAX;
		}
		
	}
	moveIndexMax = index;

	return index;
}

char BattleUIManager::moveMoveIndex(char indexChange)
{
	moveIndex += indexChange;

	if (moveIndex > moveIndexMax)
	{
		moveIndex = 0;
	}
	else if (moveIndex < 0)
	{
		moveIndex = moveIndexMax;
	}
	return moveIndex;
}


char BattleUIManager::setTeamIndexMax(Trainer trainer)
{
	int index = 0;

	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		if (trainer.GetPokemon(i)->GetIndex() == 0)												//If the pokemon is null, we have reached max index size
		{
			index = --i;
			break;
		}
		if (i == MAX_TEAM_SIZE - 1)
		{
			index = i;
		}
		
	}
	teamIndexMax = index;
	return index;
}

char BattleUIManager::moveTeamIndex(char indexChange)
{

	if (teamIndex + indexChange > teamIndexMax || teamIndex + indexChange < 0)
	{
		return -1;
	}

	teamIndex += indexChange;

	return teamIndex;
}
#pragma endregion
