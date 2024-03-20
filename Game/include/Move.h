#pragma once
#include "pokemonType.h"
#define MOVE_COUNT (46)

class Move
{

public:

	enum Category
	{
		STATUS,
		PHYSICAL,
		SPECIAL,

		NULL_MOVE,

		CAT_MAX
	};

	Move();
	Move(const char* name ,PokemonType type, Category cat, int power, int accuracy, int pp);
	bool FindMove(const char* name);
	static Move GetMove(const char* name);

	const char* GetName()	{ return name; }
	PokemonType GetType()	{ return type; }
	Category GetCategory()	{ return category; }
	int GetPower()			{ return power; }
	int GetAccuracy()		{ return accuracy; }
	int GetPP()				{ return pp; }

	bool MoveExists();

private:

	const char* name;
	PokemonType type;
	Category category;
	int power;
	int accuracy;
	int pp;

};

//Container for holding a pokemons move reference and the actual pp the pokemon has
typedef struct MoveT
{
	Move move;
	int currPP;

}MoveT;



