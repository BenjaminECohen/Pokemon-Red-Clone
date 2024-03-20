#pragma once
#include "Trainer.h"


#define MOVE_INDEX_MAX (3)

class BattleUIManager
{
public:
	static BattleUIManager* CreateInstance();
	static BattleUIManager* GetInstance() { return sInstance;};
	~BattleUIManager() {};

	enum UIState
	{
		//BattleStates
		TEXT				= 0,
		BASE,
		STATS_STATS,
		MOVELIST,
		BAG,
		STATS_MOVES,
		POKEMONLIST,
		POKEMONLIST_SELECT,
		POKEMONLIST_SWITCH,
		RUN,

		//Menu States
		MENU,

		MODE_MAX
	};

	int getModeTextureIndex(int mode);

	enum UIText_SubState
	{
		INTRO_TEXT			= 0,
		BATTLE_TEXT,
		END_TEXT,

		STATE_MAX
	};

	uint32_t		getState();
	uint32_t		setState(uint32_t mode);
	uint32_t		cancelMode();

	int		getTextState() { return mTextState; }
	int		setTextState(int index) { mTextState = index; return mTextState; }
	void	SetTextLine(const char* string, int line);

	char	setActionIndex(char index) { actionIndex = index; return actionIndex; }
	char	moveActionIndex(char indexChange);
	char	getActionIndex() { return actionIndex; }

	char	setMoveIndex(char index) { moveIndex = index; return moveIndex; }
	char	setMoveIndexMax(Pokemon playerCurrPokemon);
	char	moveMoveIndex(char indexChange);
	char	getMoveIndex() { return moveIndex; }

	char	setTeamIndex(char index) { teamIndex = index; return teamIndex; }
	char	setTeamIndexMax(Trainer trainer);
	char	moveTeamIndex(char indexChange);
	char	getTeamIndex() { return teamIndex; }

	void init(int width, int height);
	void render(Trainer player, Trainer opponent);
	void shutdown();

	void UseTempHealth(float tempHP, bool isPlayer);
	void UpdateTempHealth(float tempHP, bool isPlayer);
	void DisableTempHealth();

	//Pokemon Render
	bool	renderPlayerPokemon = false;
	bool	renderOpponentPokemon = false;
	bool	renderPlayerSprite = false;
	bool	renderOpponentSprite = false;



private:
	static BattleUIManager* sInstance;
	BattleUIManager() {};

	GLuint mBattleTextureMap;
	GLuint mSymbolMap;
	GLuint mPokemonFronts;
	GLuint mPokemonBacks;
	GLuint mMenuSheet;
	GLuint mMenuSprites;
	GLuint mTrainers;
	GLuint mPlayer;

	int maxWidth;
	int maxHeight;

	float maxXCoord;
	float maxYCoord;

	void DrawCharacterSprite(Trainer trainer, GLfloat xPosLeft, GLfloat yPosBot);
	void DrawPokemon(Pokemon pokemon, GLfloat xPosLeft, GLfloat yPosBot, bool isPlayer);

	//Draw Battle UI
	void DrawBattleUI(int32_t state);
	
	//Draw Text when needed, acts as as ubstate to battle ui
	void DrawTextUI(int textSubState);

	void DrawMenuUI(int screenIndex);

	
	//Draw character pointer
	void DrawPointers(int32_t mode);

	
	void DrawHealthBar(Pokemon pokemon, GLfloat xPos, GLfloat yPos);						//Draw the pkmn current health
	void DrawTempHealthBar(float tempHP, Pokemon pokemon, GLfloat xPos, GLfloat yPos);		//Draws using a temp health (used during animation)

	void DrawPokemonIcon(int index, GLfloat xPos, GLfloat yPos);

	void DrawRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

	void DrawSpecialIcon(GLfloat xPos, GLfloat yPos, GLfloat iconIndex);

	int32_t mLastState;
	int32_t mCurrentState;

	int		mTextState = 0;

	char line1[255];
	char line2[255];
	
	//Indexes
	char	actionIndex = 0;
	char	moveIndex = 0;
	char	moveIndexMax = MOVE_INDEX_MAX;
	char	teamIndex = 0;
	char	teamIndexMax = MAX_TEAM_SIZE;
	//char	bagIndex;

	//Temp Health
	bool	tempHealthActive = false;
	bool	isPlayerTempHealth = false;
	float	playerTempHealth = 0.f;
	float	opponentTempHealth = 0.f;

	


};

