#define TEXT_PRINTER
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
#include "SOIL.h"
#include "string.h"

#include "TextPrinter.h"

#pragma region DEFINES FOR LOCATIONS

#define LETTER_SIZE (24)
#define FSHEET_ROWS (10)
#define FSHEET_COLUMNS (16)

// remember, chars are just numbers!
#define UPPERCASE_START_OFFSET ('A')	//A
#define UPPERCASE_END_OFFSET ('Z')		//Z

#define LOWERCASE_START_OFFSET (97)		//a
#define LOWERCASE_END_OFFSET (122)		//z

#define NUMBER_START_OFFSET (48)		//0
#define NUMBER_END_OFFSET (57)			//9
#define NUMBER_LETTER_OFFSET (6)
#define NUMBER_ROW (2)

#define EMPTY_SPACE_ROW (0)
#define EMPTY_SPACE_COLUMN (15)

#pragma region BORDER DEFINES
#define PLAYER_PKMN_BORDER_XPOS (24)
#define PLAYER_PKMN_BORDER_YPOS (120)
#define PLAYER_PKMN_BORDER_LENGTH (8)

#define BORDER_ROW				(0)
#define BORDER_LT_CORNER_COL	(9)
#define BORDER_MID_COL			(10)
#define BORDER_RT_CORNER_COL	(11)
#define BORDER_V_COL			(12)
#define BORDER_LB_CORNER_COL	(13)
#define BORDER_RB_CORNER_COL	(14)
#pragma endregion

#define DASH_ROW (3)
#define DASH_COL (3)
#define NULL_MOVE_LENGTH (12)

#pragma region SPECIAL CHARACTERS
#define ASCII_AP_S		(37)		// % = 's
#define ASCII_AP_T		(64)		// @ = 't
#define ASCII_EXCLA		(33)		
#define ASCII_QUEST		(63)		
// same w/ other visible ascii chars...
#define ASCII_SPACE		(' ')
#define ASCII_PERIOD	('.')


#pragma endregion


GLfloat uOffset = 1.0 / FSHEET_COLUMNS;											//Set u/x value offset
GLfloat vOffset = 1.0 / FSHEET_ROWS;											//Set v/y value offset


TextPrinter* TextPrinter::instance = NULL;


TextPrinter* TextPrinter::CreateInstance()
{
	if (instance != NULL)
	{
		return instance;
	}

	instance = new TextPrinter();
	return instance;
	
}


void TextPrinter::init()
{
	mFontMap = SOIL_load_OGL_texture("asset/fontSheet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}


void TextPrinter::PrintTextAt(const char* string, GLfloat xPosLeft, GLfloat yPosBot, char spaceLength)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, instance->mFontMap);
	glBegin(GL_QUADS);

	GLfloat sheetRow = 0;														
	GLfloat sheetColumn = 0;												

	

	size_t length = strlen(string);													//Get how long the string actually is
	
	//glColor4ub(0, 0xFF, 0, 0xFF); //Dye Green for testing
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	for (int i = 0; i < spaceLength; i++, xPosLeft += LETTER_SIZE)
	{
		if (i < (int)length)
		{
			char c = string[i];																//Get character at position

			char row;
			char column;

			instance->GetRowColumn(c, &row, &column);

			GLfloat uPos = (GLfloat)column * uOffset;										// Get the left U position
			GLfloat vPos = (GLfloat)row * vOffset;											//Get the bottom V position
			

			glTexCoord2f(uPos, vPos);
			glVertex3f(xPosLeft, yPosBot, 0.0);
			glTexCoord2f(uPos, vPos + vOffset);
			glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos + vOffset);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);
		}
		else																				//Fill in remainder space with empty spaces
		{

			GLfloat uPos = EMPTY_SPACE_COLUMN * uOffset;						// Get the left U position
			GLfloat vPos = EMPTY_SPACE_ROW * vOffset;							//Get the bottom V position

			glTexCoord2f(uPos, vPos);
			glVertex3f(xPosLeft, yPosBot, 0.0);
			glTexCoord2f(uPos, vPos + vOffset);
			glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos + vOffset);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);
		}

		
	}	

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnd();


}

void TextPrinter::PrintNumber(uint16_t stat, GLfloat xPosLeft, GLfloat yPosBot, char spaceLength, bool leadingZeros, bool leftAlign)
{
	GLfloat uPos = 0;
	GLfloat vPos = 0;
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, instance->mFontMap);
	glBegin(GL_QUADS);

	bool numberStarted = false;

	if (leadingZeros)
	{
		numberStarted = true;
	}

	char numBuffer[2];
	uint16_t numToPrint = 0;
	uint16_t val = stat;
	
	//glColor4ub(0xAB, 0xEC, 0xFF, 0xFF);			//For Testing
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	for (int i = 0; i < spaceLength; i++, xPosLeft += LETTER_SIZE)
	{
		//Change int to ascii char
		numToPrint = val / (uint16_t)pow(10, spaceLength - i - 1);


		if (numToPrint == 0 && !numberStarted && i != spaceLength - 1)	     //If we havent printed anything yet AND the number is 0 AND this isn't the final digit
		{
			uPos = EMPTY_SPACE_COLUMN * uOffset;							// Get the left U position
			vPos = EMPTY_SPACE_ROW * vOffset;								//Get the bottom V position
		}
		else																		//Print a number
		{			
			_itoa(numToPrint, &numBuffer[0], 10);

			char row;
			char column;

			instance->GetRowColumn(numBuffer[0], &row, &column);

			uPos = (GLfloat)column * uOffset;					// Get the left U position
			vPos = NUMBER_ROW * vOffset;												//Get the bottom V position		
			numberStarted = true;

		}

		glTexCoord2f(uPos, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(uPos, vPos + vOffset);
		glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
		glTexCoord2f(uPos + uOffset, vPos + vOffset);
		glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
		glTexCoord2f(uPos + uOffset, vPos);
		glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);

		//If nothing is printed yet, number is 0, not last digit, AND is left aligned, don't let xPosLeft increment
		if (numToPrint == 0 && !numberStarted && i != spaceLength - 1 && leftAlign)
		{
			xPosLeft -= LETTER_SIZE;

		}

		//Save remainder and work with that next loop
		val %= (uint16_t)pow(10, spaceLength - i - 1);

	}
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnd();

}

void TextPrinter::PrintSpecial(SpecialMode mode, GLfloat _xPosLeft, GLfloat _yPosBot)
{
	GLfloat xPosLeft = 0;
	GLfloat yPosBot = 0;
	GLfloat uPos = 0;
	GLfloat vPos = 0;
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, instance->mFontMap);
	glBegin(GL_QUADS);

	if (mode == PLAYER_PKMN_TEXTBOX)
	{

		xPosLeft = PLAYER_PKMN_BORDER_XPOS;
		yPosBot = PLAYER_PKMN_BORDER_YPOS;

		for (int i = 0; i < PLAYER_PKMN_BORDER_LENGTH; i++, xPosLeft += LETTER_SIZE)
		{

			uPos = BORDER_MID_COL * uOffset;						// Get the left U position
			vPos = BORDER_ROW * vOffset;							//Get the bottom V position		

			glTexCoord2f(uPos, vPos);
			glVertex3f(xPosLeft, yPosBot, 0.0);
			glTexCoord2f(uPos, vPos + vOffset);
			glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos + vOffset);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);
		}
	}

	else if (mode == PLAYER_PKMN_BASE)
	{

		xPosLeft = PLAYER_PKMN_BORDER_XPOS;
		yPosBot = PLAYER_PKMN_BORDER_YPOS;

		for (int i = 0; i < PLAYER_PKMN_BORDER_LENGTH; i++, xPosLeft += LETTER_SIZE)
		{
			if (i == PLAYER_PKMN_BORDER_LENGTH - 1)						//We have a special letter at the end in this case
			{
				uPos = BORDER_LT_CORNER_COL * uOffset;						// Get the left U position
				vPos = BORDER_ROW * vOffset;							//Get the bottom V position
			}
			else
			{
				uPos = BORDER_MID_COL * uOffset;						// Get the left U position
				vPos = BORDER_ROW * vOffset;							//Get the bottom V position
			}

			glTexCoord2f(uPos, vPos);
			glVertex3f(xPosLeft, yPosBot, 0.0);
			glTexCoord2f(uPos, vPos + vOffset);
			glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos + vOffset);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);
		}

		yPosBot -= LETTER_SIZE;
		xPosLeft -= LETTER_SIZE;

		uPos = BORDER_V_COL * uOffset;						// Get the left U position
		vPos = BORDER_ROW * vOffset;							//Get the bottom V position

		glTexCoord2f(uPos, vPos);
		glVertex3f(xPosLeft, yPosBot, 0.0);
		glTexCoord2f(uPos, vPos + vOffset);
		glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
		glTexCoord2f(uPos + uOffset, vPos + vOffset);
		glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
		glTexCoord2f(uPos + uOffset, vPos);
		glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);

	}

	else if (mode == MOVE_NULL)
	{
		xPosLeft = _xPosLeft;
		yPosBot = _yPosBot;

		for (int i = 0; i < NULL_MOVE_LENGTH; i++, xPosLeft += LETTER_SIZE)
		{
			if (i == 0)
			{
				uPos = DASH_COL * uOffset;						// Get the left U position
				vPos = DASH_ROW * vOffset;							//Get the bottom V position
			}
			else
			{
				uPos = EMPTY_SPACE_COLUMN * uOffset;						// Get the left U position
				vPos = EMPTY_SPACE_ROW * vOffset;							//Get the bottom V position
			}
			glTexCoord2f(uPos, vPos);
			glVertex3f(xPosLeft, yPosBot, 0.0);
			glTexCoord2f(uPos, vPos + vOffset);
			glVertex3f(xPosLeft, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos + vOffset);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot + LETTER_SIZE, 0.0);
			glTexCoord2f(uPos + uOffset, vPos);
			glVertex3f(xPosLeft + LETTER_SIZE, yPosBot, 0.0);
		}
		

		
	}

	glEnd();

}



void TextPrinter::GetRowColumn(char c, char* row, char* col)
{
	
	*row = FSHEET_ROWS - 1;
	char rowOffset = 0;

	*col = 0;
	char columnOffset = 0;

	if (c >= UPPERCASE_START_OFFSET && c <= UPPERCASE_END_OFFSET)			//value A to Z
	{
		c -= UPPERCASE_START_OFFSET;
		rowOffset = floor((float)c / FSHEET_COLUMNS);
		*row -= rowOffset;

		columnOffset = c % FSHEET_COLUMNS;
		*col += columnOffset;

	}
	else if (c >= LOWERCASE_START_OFFSET && c <= LOWERCASE_END_OFFSET)		//Value a to z
	{
		c -= LOWERCASE_START_OFFSET;
		rowOffset = floor((float)c / FSHEET_COLUMNS) + 2;
		*row -= rowOffset;

		columnOffset = c % FSHEET_COLUMNS;
		*col += columnOffset;

	}
	else if (c >= NUMBER_START_OFFSET && c <= NUMBER_END_OFFSET)			//Number value
	{
		c -= NUMBER_START_OFFSET;
		rowOffset = floor((float)c / FSHEET_COLUMNS);
		rowOffset += NUMBER_LETTER_OFFSET;
		*row -= rowOffset;

		columnOffset = c % FSHEET_COLUMNS;
		columnOffset += NUMBER_LETTER_OFFSET;
		*col += columnOffset;

	}
	else if (c == ASCII_SPACE)												//Space
	{
		*row = EMPTY_SPACE_ROW;
		*col = EMPTY_SPACE_COLUMN;


	}
	else if (c == ASCII_EXCLA)												//Exclamation
	{
		*row = 3;
		*col = 7;
	}
	else if (c == ASCII_QUEST)
	{
		*row = 3;
		*col = 7;
	}
	else if (c == ASCII_PERIOD)
	{
		*row = 3;
		*col = 8;
	}
	else if (c == ASCII_AP_S)
	{
		*row = 6;
		*col = 13;
	}
	
	else if (c == ASCII_AP_T)
	{
		*row = 6;
		*col = 14;
	}
}



