#pragma once



class TextPrinter
{

public:

	static TextPrinter* CreateInstance();
	static TextPrinter* GetInstance() { return instance; };

	static void PrintTextAt(const char* string, GLfloat xPosLeft, GLfloat yPosBot, char spaceLength);

	enum SpecialMode
	{
		PLAYER_PKMN_TEXTBOX			= 0,
		PLAYER_PKMN_BASE,
		MOVE_NULL,


		SPECIAL_MODE_MAX
	};

	static void PrintSpecial(SpecialMode mode, GLfloat _xPosLeft, GLfloat _yPosBot);

	static void PrintNumber(uint16_t stat, GLfloat xPosLeft, GLfloat yPosBot, char spaceLength, bool leadingZeros, bool leftAlign = false);

	void init();
	// by convention, always pair init w/ shutdown
	void shutdown() {}




private:

	static TextPrinter* instance;
	GLuint mFontMap;

	void GetRowColumn(char c, char* row, char* col);


};