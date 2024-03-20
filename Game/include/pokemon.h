#pragma once
#include "baseTypes.h"
#include "Move.h"
#include "pokemonType.h"


#define PKMN_NAME_MAX (10)
#define PKMN_SPRITE_SIZE (168)

#define NUMBER_OF_POKEMON (19)
#define MOVES_PER_PKMN (4)




class Pokemon
{
private:
	
	
	uint16_t index;						//Entry number helps find location in spritesheet
	
	const char* name;

	PokemonType type;
	uint16_t level;
	uint32_t exp;

	//Stats
	uint16_t maxHealth;
	uint16_t currHealth;
	uint16_t attack;
	uint16_t defense;
	uint16_t sattack;
	uint16_t sdefense;
	uint16_t speed;

	//List
	MoveT moveList[4];

	//Training
	
	


public:
	
	Pokemon();
	Pokemon(uint16_t index, const char* name, uint16_t level, PokemonType type, uint16_t health, uint16_t attack, uint16_t defense, uint16_t sattack, uint16_t sdefense, uint16_t speed);
	Pokemon(uint16_t index, const char* name, uint16_t level, PokemonType type, uint16_t health, uint16_t attack, uint16_t defense, uint16_t sattack, uint16_t sdefense, uint16_t speed, Move move1, Move move2, Move move3, Move move4);

	const char* GetName()		{ return name; }
	uint16_t GetLevel()			{ return level; }
	PokemonType GetType()		{ return type; }
	uint16_t GetMaxHealth()		{ return maxHealth; }
	uint16_t GetCurrHealth()	{ return currHealth; }
	uint16_t GetIndex()			{ return index; }
	uint16_t GetAttack()		{ return attack; }
	uint16_t GetDefense()		{ return defense; }
	uint16_t GetSpAttack()		{ return sattack; }
	uint16_t GetSpDefense()		{ return sdefense; }
	uint16_t GetSpeed()			{ return speed; }

	MoveT* GetMove(int index);

	char DealDamage(Move attackMove, Pokemon attackingPokemon);
	bool UseMove(MoveT* move);
	bool GiveMove(Move move, int index);

	void Restore();
	
	

	static void CopyPokemon(Pokemon* dest, Pokemon* source);				//Copy stats of one pokemon to another (used for catching)
	//GainExperience(uint32_t expGain);

};

class PokemonManager												//Holds a list of all the pokemon available
{
public:

	static PokemonManager* CreateInstance();
	static PokemonManager* GetInstance() { return sInstance; };

	static Pokemon GetPokemon(uint16_t pkmnIndex);

	void init();
	void shutdown();

private:
	static PokemonManager* sInstance;
	//Pokemon* pokemonList;
	//char** pokemonNameList;




};





