



class StateManagerC
{
public:
	static StateManagerC	*CreateInstance();
	static StateManagerC	*GetInstance() {return sInstance;};

	~StateManagerC(){};
	enum
	{
		NO_BALLS_FILLED,
		HALF_BALLS_FILLED,
		ALL_BALLS_FILLED
	};

	enum State
	{
		MENU					= 0,
		START,
		TEXT,
		BATTLE_TEXT,
		ACTION,

		PLAYER_ATTACK,
		OPPONENT_ATTACK,
		PLAYER_PKMN_FAINT,
		OPPONENT_PKMN_FAINT,
		
		OPPONENT,	

		BATTLE_END,
		RETURN_TO_MENU,

		RUN,

		MAX_STATE

	};

	enum TurnActor
	{
		T_PLAYER					= 0,
		T_OPPONENT
	};

	enum B_Text_State
	{
		//Used for battle turn taking
		CALCULATE_DAMAGE			= 0,
		ADJUST_HP,
		STATE_EFFECTIVENESS,
		CHECK_IF_FAINTED,
		CHECK_IF_DEFEATED,
		CHECK_IF_DONE,

		//Used for swapping a pokemon instead of a move
		SWAP_OUT_PKMN,
		SWAP_OUT_PKMN_ANIM,
		SEND_OUT_NEW_PKMN,
		SWAP_IN_PKMN_ANIM,

		ENEMY_SWAP_PKMN,
		ENEMY_SWAP_ANIM,



		B_TEXT_STATE_MAX
	};

	int32_t		getState();										
	
	
	int32_t		startState();									//Things to do at the start of a state
	int32_t		exitState();									//Things to do at the exit of a state
	int32_t		progressState();



	//New functions
	int32_t		executeTurn(TurnActor actor);
	int32_t		executeTurn(TurnActor actor, B_Text_State startTextState);
	int32_t		progressBattleText();
	int			getBattleTextState();
	int			setBattleTextState(B_Text_State newState) { mCurrTextState = newState; return newState; }

	bool		allActorsTookTurn();
	TurnActor	nextActorTurn();
	TurnActor	getCurrActor() { return currActor; }

	void		setActorsDone() { firstActorDone = true; }
	


	void		init();

	void		setState(int32_t stateToSet);
	void		shutdown();

private:
	static StateManagerC *sInstance;
	StateManagerC(){};

	int32_t				mCurrentState;

	TurnActor			currActor			= TurnActor::T_PLAYER;
	B_Text_State		mCurrTextState		= (B_Text_State) 0;
	bool				firstActorDone		= false;				//Has the first mover in a turn gone?


};