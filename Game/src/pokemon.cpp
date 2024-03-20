#define POKEMON_CPP
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>											// Header File For Windows
#include <math.h>
#include <string.h>
#include <assert.h>


#include "pokemon.h"

#define POKEMON_COUNT (19)

PokemonManager* PokemonManager::sInstance = NULL;

PokemonManager* PokemonManager::CreateInstance()
{
	if (sInstance != NULL)
	{
		return sInstance;
	}
	else
	{
		sInstance = new PokemonManager();
		return sInstance;
	}
}


void PokemonManager::init()
{
	/*
	pokemonList = (Pokemon*)malloc(sizeof(Pokemon) * NUMBER_OF_POKEMON);
	pokemonNameList = (char**)malloc(sizeof(char*) * NUMBER_OF_POKEMON);
	
	for (int i = 0; i < NUMBER_OF_POKEMON; i++)
	{
		pokemonNameList[i] = (char*)malloc(sizeof(char) * (PKMN_NAME_MAX + 1));
	}
	*/
	
	//CREATE ALL POKEMON HERE
	uint16_t index = 0;
	
	/*
	pokemonList[6] = Pokemon(7, 1, PokemonType::WATER, 44, 48, 65, 50, 64, 43);
	strcpy(pokemonNameList[6], "SQUIRTLE\0");
	pokemonList[6].GiveMove(Move::GetMove("TACKLE\0"), 0);
	pokemonList[6].GiveMove(Move::GetMove("WATER GUN\0"), 0);

	pokemonList[7] = Pokemon(8, 1, PokemonType::WATER, 44, 48, 65, 50, 64, 43);
	strcpy(pokemonNameList[7], "WARTORTLE\0");

	*/

}

void PokemonManager::shutdown()
{
	//free(sInstance->pokemonList);
	
	for (int i = 0; i < NUMBER_OF_POKEMON; i++)
	{
		//free(sInstance->pokemonNameList[i]);
	}
	//free(sInstance->pokemonNameList);
}



MoveT* Pokemon::GetMove(int index)
{
	assert(index >= 0 && index < MOVES_PER_PKMN);
	return &moveList[index];
}

char Pokemon::DealDamage(Move attackMove, Pokemon attackingPokemon)
{
	int pStatPow		= 0;
	int attackerLevel	= 0;
	int defenderDef		= 0;

	if (attackMove.GetCategory() == Move::Category::PHYSICAL)
	{
		pStatPow = attackingPokemon.GetAttack();
		defenderDef = defense;
	}
	else if (attackMove.GetCategory() == Move::Category::SPECIAL)
	{
		pStatPow = attackingPokemon.GetSpAttack();
		defenderDef = sdefense;
	}
	else //Status type
	{
		pStatPow = attackingPokemon.GetAttack(); ///REPLACE LATER
	}

	//---
	//BASE DAMAGE
	//---
	int baseDamage	= attackingPokemon.GetLevel() * 2;
	baseDamage		= (int)floor((float)baseDamage / 5.0) + 2;
	baseDamage		*= attackMove.GetPower() * pStatPow;
	baseDamage		= (int)floor((float)baseDamage / (float)defenderDef);
	baseDamage		= (int)floor((float)baseDamage / 50.0f);

	if (baseDamage > 997)
	{
		baseDamage = 997 + 2;
	}

	//---
	//MODIFIED DAMAGE
	//---
	int modifiedDamage = baseDamage;

	//STAB (same-type attack bonus)
	if (attackMove.GetType() == attackingPokemon.GetType())
	{
		modifiedDamage += (int)floor((float)baseDamage / 2.0);
	}

	//Calculate Type Effectiveness
	char typeMultiplier = 0;
	typeMultiplier = getTypeMultiplier(attackMove.GetType(), type);

	if (typeMultiplier == 0)					//Move does not affect
	{
		return -2;
	}

	modifiedDamage = (int)floor((modifiedDamage * typeMultiplier) / 10.0);


	if (modifiedDamage == 0)				//Deal No damage = attack misses
	{
		return -1;
	}

	//---
	//Random Factor
	//---

	if (modifiedDamage != 1)
	{
		float randomVal = ((float)rand() / RAND_MAX) * (255.0f - 217.0f);
		randomVal += 217;

		modifiedDamage = (int)floor(((float)modifiedDamage * randomVal) / 255.0f);
	}


	if (currHealth < modifiedDamage) 
	{
		currHealth = 0;
		return typeMultiplier;
	}

	currHealth -= modifiedDamage;
	//if (currHealth > maxHealth) { currHealth = maxHealth; }

	return typeMultiplier;
}

void Pokemon::CopyPokemon(Pokemon* dest, Pokemon* source)
{

	dest->name = source->name;
	dest->index = source->index;

	dest->level = source->level;

	dest->type = source->type;


	dest->maxHealth = source->maxHealth;
	dest->currHealth = source->currHealth;
	dest->attack = source->attack;
	dest->defense = source->defense;
	dest->sattack = source->sattack;
	dest->sdefense = source->sdefense;
	dest->speed = source->speed;

	for (int i = 0; i < MOVES_PER_PKMN; i++)
	{
		dest->moveList[i].move = source->moveList[i].move;
		dest->moveList[i].currPP = source->moveList[i].move.GetPP();
	}
}

Pokemon::Pokemon()
{
	this->index = 0;			//Means NO pokemon
	this->name = 0;
	this->level = 0;
	this->exp = 0;

	this->type = (PokemonType)0;
	

	this->maxHealth = 0;
	this->currHealth = 0;
	this->attack = 0;
	this->defense = 0;
	this->sattack = 0;
	this->sdefense = 0;
	this->speed = 0;

}

Pokemon::Pokemon(uint16_t index, const char* name, uint16_t level, PokemonType type, uint16_t health, uint16_t attack, uint16_t defense, uint16_t sattack, uint16_t sdefense, uint16_t speed)
{
	this->index = index;
	this->name = name;
	this->level = level;
	this->exp = 0;

	this->type = type;



	this->maxHealth = health;
	this->currHealth = health;
	this->attack = attack;
	this->defense = defense;
	this->sattack = sattack;
	this->sdefense = sdefense;
	this->speed = speed;

	for (int i = 0; i < MOVES_PER_PKMN; i++)
	{
		this->moveList[i].move = Move::GetMove("NULL\0");
		this->moveList[i].currPP = Move::GetMove("NULL\0").GetPP();
	}

}

Pokemon::Pokemon(uint16_t index, const char* name, uint16_t level, PokemonType type, uint16_t health, uint16_t attack, uint16_t defense, uint16_t sattack, uint16_t sdefense, uint16_t speed, Move move1, Move move2, Move move3, Move move4)
{
	this->index = index;
	this->name = name;
	this->level = level;
	this->exp = 0;

	this->type = type;



	this->maxHealth = health;
	this->currHealth = health;
	this->attack = attack;
	this->defense = defense;
	this->sattack = sattack;
	this->sdefense = sdefense;
	this->speed = speed;

	this->moveList[0].move = move1;
	this->moveList[0].currPP = move1.GetPP();

	this->moveList[1].move = move2;
	this->moveList[1].currPP = move2.GetPP();

	this->moveList[2].move = move3;
	this->moveList[2].currPP = move3.GetPP();

	this->moveList[3].move = move4;
	this->moveList[3].currPP = move4.GetPP();

}

Pokemon PokemonList[POKEMON_COUNT] =										
{
	Pokemon(0, "NULL\0", 1, PokemonType::NORMAL, 0, 0, 0, 0, 0, 0),									//StandIn For DOES NOT EXIST

	Pokemon(1, "DEWGONG\0",		32,	PokemonType::WATER, 90, 70, 80, 70, 95, 70,			Move::GetMove("AURORA BEAM\0"), Move::GetMove("ICE BEAM\0"), Move::GetMove("TAKE DOWN\0"), Move::GetMove("NULL\0")),
	Pokemon(2, "ALAKAZAM\0",	32,	PokemonType::PSYCHIC, 55, 50, 45, 135, 95, 120,		Move::GetMove("CONFUSION\0"), Move::GetMove("PSYBEAM\0"),Move::GetMove("PSYCHIC\0"), Move::GetMove("NULL\0")),
	Pokemon(3, "VENASAUR\0",	32,	PokemonType::GRASS, 80, 82, 83, 100, 100, 80,		Move::GetMove("TACKLE\0"), Move::GetMove("VINE WHIP\0"), Move::GetMove("RAZOR LEAF\0"), Move::GetMove("NULL\0")),

	Pokemon(4, "SANDSLASH\0",	32,	PokemonType::GROUND, 75, 100, 110, 45, 55, 65,		Move::GetMove("SCRATCH\0"), Move::GetMove("SLASH\0"), Move::GetMove("EARTHQUAKE\0"), Move::GetMove("NULL\0")),
	Pokemon(5, "MACHAMP\0",		32,	PokemonType::FIGHTING, 90, 130, 80, 65, 85, 55,		Move::GetMove("LOW KICK\0"), Move::GetMove("SUBMISSION\0"), Move::GetMove("EARTHQUAKE\0"), Move::GetMove("DOUBLE EDGE\0")),
	Pokemon(6, "CHARIZARD\0",	32,	PokemonType::FIRE, 78, 84, 78, 109, 85, 100,		Move::GetMove("EMBER\0"), Move::GetMove("FLAMETHROWER\0"), Move::GetMove("TAKE DOWN\0"), Move::GetMove("NULL\0")),

	Pokemon(7, "GENGAR\0",		32,	PokemonType::GHOST, 60, 65, 60, 130, 75, 110,		Move::GetMove("TAKE DOWN\0"), Move::GetMove("LICK\0"), Move::GetMove("THUNDERBOLT\0"), Move::GetMove("DREAM EATER\0")),
	Pokemon(8, "TENTACRUEL\0",	32,	PokemonType::WATER, 80, 70, 65, 80, 120, 100,		Move::GetMove("POISON STING\0"), Move::GetMove("WATER GUN\0"), Move::GetMove("WRAP\0"), Move::GetMove("ACID\0")),
	Pokemon(9, "BLASTOISE\0",	32,	PokemonType::WATER, 79, 83, 100, 85, 105, 78,		Move::GetMove("HYDRO PUMP\0"), Move::GetMove("BITE\0"), Move::GetMove("WATER GUN\0"), Move::GetMove("BUBBLE\0")),

	Pokemon(10, "RAPIDASH\0",	32,	PokemonType::FIRE, 65, 100, 70, 80, 80, 105,		Move::GetMove("EMBER\0"), Move::GetMove("TAKE DOWN\0"), Move::GetMove("SKULL BASH\0"), Move::GetMove("NULL\0")),
	Pokemon(11, "PIKACHU\0",	32,	PokemonType::ELECTRIC, 35, 55, 40, 50, 50, 90,		Move::GetMove("THUNDERBOLT\0"), Move::GetMove("DOUBLE EDGE\0"), Move::GetMove("SUBMISSION\0"), Move::GetMove("SKULL BASH\0")),
	Pokemon(12, "ARBOK\0",		32,	PokemonType::POISON, 60, 95, 69, 65, 79, 80,		Move::GetMove("POISON STING\0"), Move::GetMove("BITE\0"), Move::GetMove("ACID\0"), Move::GetMove("EARTHQUAKE\0")),
	
	Pokemon(13, "BEEDRILL\0",	32,	PokemonType::BUG, 65, 90, 40, 45, 80, 75,			Move::GetMove("TWINEEDLE\0"), Move::GetMove("TAKE DOWN\0"), Move::GetMove("POISON STING\0"), Move::GetMove("TWINEEDLE\0")),
	Pokemon(14, "PIGEOT\0",		32,	PokemonType::FLY, 83, 80, 75, 70, 70, 101,			Move::GetMove("GUST\0"), Move::GetMove("WING ATTACK\0"), Move::GetMove("SKY ATTACK\0"), Move::GetMove("TAKE DOWN\0")),
	Pokemon(15, "NIDOKING\0",	32,	PokemonType::POISON, 79, 83, 100, 85, 105, 78,		Move::GetMove("HORN ATTACK\0"), Move::GetMove("DOUBLE KICK\0"), Move::GetMove("SURF\0"), Move::GetMove("ICE BEAM\0")),
	
	Pokemon(16, "ONIX\0",		32,	PokemonType::ROCK, 35, 45, 160, 30, 45, 75,			Move::GetMove("TAKE DOWN\0"), Move::GetMove("ROCK THROW\0"), Move::GetMove("EARTHQUAKE\0"), Move::GetMove("ROCK SLIDE\0")),
	Pokemon(17, "SNORLAX\0",	32,	PokemonType::NORMAL, 160, 110, 65, 65, 110, 30,		Move::GetMove("DOUBLE EDGE\0"), Move::GetMove("BODY SLAM\0"), Move::GetMove("SURF\0"), Move::GetMove("THUNDERBOLT\0")),
	Pokemon(18, "DRAGONITE\0",	32,	PokemonType::DRAGON, 91, 134, 95, 100, 100, 80,		Move::GetMove("TAKE DOWN\0"), Move::GetMove("SURF\0"), Move::GetMove("THUNDERBOLT\0"), Move::GetMove("BLIZZARD\0")),
};


//Returns true if user has enough pp left
bool Pokemon::UseMove(MoveT* move)
{
	if (move->currPP == 0)
	{
		return false;
	}

	--move->currPP;						//Decrement amount left and return true
	return true;
}

bool Pokemon::GiveMove(Move move, int index)
{
	assert(index >= 0 && index < MOVE_COUNT);

	moveList[index].move = move;
	moveList[index].currPP = move.GetPP();

	return true;
}

void Pokemon::Restore()
{
	currHealth = maxHealth;
	
	for (int i = 0; i < MOVE_COUNT; i++)
	{
		moveList[index].currPP = moveList[index].move.GetPP();
	}

}




Pokemon PokemonManager::GetPokemon(uint16_t pkmnIndex)
{
	Pokemon mon = PokemonList[pkmnIndex];
	return mon;						//-1 to account for how data is stored. Pokemon number is +1 their index in the data structure
}
