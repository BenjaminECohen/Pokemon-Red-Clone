#define MOVE_CPP

#include <windows.h>											// Header File For Windows
#include <string.h>


#include "Move.h"



Move::Move()
{
}

Move::Move(const char* name, PokemonType type, Category cat, int power, int accuracy, int pp)
{
	this->name = name;
	this->type = type;
	this->category = cat;
	this->power = power;
	this->accuracy = accuracy;
	this->pp = pp;

}

Move MasterMoveList[MOVE_COUNT] =
{
	Move("NULL\0", PokemonType::NORMAL, Move::NULL_MOVE, 0, 0, 0),

	Move("ABSORB\0", PokemonType::GRASS, Move::SPECIAL, 20, 100, 25),
	Move("ACID\0", PokemonType::POISON, Move::SPECIAL, 40, 100, 30),
	Move("AURORA BEAM\0", PokemonType::ICE, Move::SPECIAL, 65, 100, 20),
	Move("BARRAGE\0", PokemonType::NORMAL, Move::PHYSICAL, 15, 85, 20),
	Move("BITE\0", PokemonType::NORMAL, Move::PHYSICAL, 60, 100, 25),

	Move("BLIZZARD\0", PokemonType::ICE, Move::SPECIAL, 110, 70, 5),
	Move("BODY SLAM\0", PokemonType::NORMAL, Move::PHYSICAL, 85, 100, 15),
	Move("BONE CLUB\0", PokemonType::GROUND, Move::PHYSICAL, 65, 85, 20),
	Move("BONEMERANG\0", PokemonType::GROUND, Move::PHYSICAL, 50, 90, 10),
	Move("BUBBLE\0", PokemonType::WATER, Move::SPECIAL, 20, 100, 30),

	Move("VINE WHIP\0", PokemonType::GRASS, Move::SPECIAL, 35, 100, 25),
	Move("RAZOR LEAF\0", PokemonType::GRASS, Move::SPECIAL, 55, 95, 25),
	Move("TACKLE\0", PokemonType::NORMAL, Move::PHYSICAL, 40, 95, 35),
	Move("WATER GUN\0", PokemonType::WATER, Move::SPECIAL, 40, 100, 25),
	Move("HYDRO PUMP\0", PokemonType::WATER, Move::SPECIAL, 110, 80, 5),
	
	Move("HEADBUTT\0", PokemonType::NORMAL, Move::PHYSICAL, 70, 100, 15),
	Move("ICE BEAM\0", PokemonType::ICE, Move::SPECIAL, 95, 100, 10),
	Move("TAKE DOWN\0", PokemonType::NORMAL, Move::PHYSICAL, 90, 85, 20),
	Move("CONFUSION\0", PokemonType::PSYCHIC, Move::SPECIAL, 50, 100, 25),
	Move("PSYBEAM\0", PokemonType::PSYCHIC, Move::SPECIAL, 65, 100, 20),

	Move("SCRATCH\0", PokemonType::NORMAL, Move::PHYSICAL, 40, 100, 35),
	Move("SLASH\0", PokemonType::NORMAL, Move::PHYSICAL, 70, 100, 20),
	Move("EARTHQUAKE\0", PokemonType::GROUND, Move::PHYSICAL, 100, 100, 10),
	Move("LOW KICK\0", PokemonType::FIGHTING, Move::PHYSICAL, 50, 90, 20),
	Move("SUBMISSION\0", PokemonType::FIGHTING, Move::PHYSICAL, 80, 80, 20),

	Move("DOUBLE EDGE\0", PokemonType::NORMAL, Move::PHYSICAL, 100, 100, 15),
	Move("FLAMETHROWER\0", PokemonType::FIRE, Move::SPECIAL, 95, 100, 15),
	Move("EMBER\0", PokemonType::FIRE, Move::SPECIAL, 40, 100, 25),
	Move("LICK\0", PokemonType::GHOST, Move::PHYSICAL, 20, 100, 30),
	Move("DREAM EATER\0", PokemonType::PSYCHIC, Move::SPECIAL, 100, 100, 15),

	Move("THUNDERBOLT\0", PokemonType::ELECTRIC, Move::SPECIAL, 95, 100, 15),
	Move("WRAP\0", PokemonType::NORMAL, Move::PHYSICAL, 15, 85, 20),
	Move("POISON STING\0", PokemonType::POISON, Move::PHYSICAL, 15, 100, 35),
	Move("SKULL BASH\0", PokemonType::NORMAL, Move::PHYSICAL, 100, 100, 10),
	Move("TWINEEDLE\0", PokemonType::BUG, Move::PHYSICAL, 50, 100, 20),

	Move("PIN MISSLE\0", PokemonType::BUG, Move::PHYSICAL, 75, 95, 20),
	Move("GUST\0", PokemonType::NORMAL, Move::PHYSICAL, 40, 100, 35),
	Move("WING ATTACK\0", PokemonType::FLY, Move::PHYSICAL, 35, 100, 35),
	Move("SKY ATTACK\0", PokemonType::FLY, Move::PHYSICAL, 140, 90, 5),
	Move("HORN ATTACK\0", PokemonType::NORMAL, Move::PHYSICAL, 65, 100, 25),

	Move("DOUBLE KICK\0", PokemonType::FIGHTING, Move::PHYSICAL, 60, 100, 30),
	Move("SURF\0", PokemonType::WATER, Move::SPECIAL, 95, 100, 15),
	Move("ROCK THROW\0", PokemonType::ROCK, Move::PHYSICAL, 50, 65, 15),
	Move("ROCK SLIDE\0", PokemonType::ROCK, Move::PHYSICAL, 75, 90, 10),
	Move("PSYCHIC\0", PokemonType::PSYCHIC, Move::SPECIAL, 90, 100, 10)


};


bool Move::FindMove(const char* name)
{
	for (int i = 0; i < MOVE_COUNT; i++)
	{
		if (strcmp(name, MasterMoveList[i].name) == 0)		//We have a match
		{
			return true;
		}
	}
	return false;
}

Move Move::GetMove(const char* name)
{
	for (int i = 0; i < MOVE_COUNT; i++)
	{
		if (strcmp(name, MasterMoveList[i].name) == 0)		//We have a match
		{
			return MasterMoveList[i];
		}
	}
	return MasterMoveList[0];
}

bool Move::MoveExists()
{
	if (category != NULL_MOVE)
	{
		return true;
	}
	return false;
}





