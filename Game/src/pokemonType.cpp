
#include <assert.h>
#include "pokemonType.h"

const char* PokemonTypeStrings[TYPE_MAX] =
{
	"NORMAL\0",
	"FIRE\0",
	"WATER\0",
	"ELECTRIC\0",
	"GRASS\0",
	"ICE\0",
	"FIGHTING\0",
	"POISON\0",
	"GROUND\0",
	"FLYING\0",
	"PSYCHIC\0",
	"BUG\0",
	"ROCK\0",
	"GHOST\0",
	"DRAGON\0",

};

char multiplierChart[TYPE_MAX][TYPE_MAX] =
{
	//N  Fi  Wa  El  Gr  Ic  Fg  Po  Go  Fl  Ps  Bu  Ro  Gh  Dr
	{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  5,  0, 10},	//Normal
	{10,  5,  5, 10, 20, 20, 10, 10, 10, 10, 10, 20,  5, 10, 5},	//Fire
	{10, 20,  5, 10,  5, 10, 10, 10, 20, 10, 10, 10, 20, 10, 5},	//Water
	{10, 10, 20,  5,  5, 10, 10, 10,  0, 20, 10, 10, 10, 10, 5},	//Electric
	{10,  5, 20, 10,  5, 10, 10,  5, 20,  5, 10,  5, 20, 10, 5},	//Grass
	{10, 10,  5, 10, 20,  5, 10, 10, 10, 20, 10, 10, 10, 10, 20},	//Ice
	{20, 10, 10, 10, 10, 20, 10,  5, 10,  5,  5,  5, 20,  0, 10},	//Fighting
	{10, 10, 10, 10, 20, 10, 10,  5,  5, 10, 10, 20,  5,  5, 10},	//Poison
	{10, 20, 10, 20,  5, 10, 10, 20, 10,  0, 10,  5, 20, 10, 10},	//Ground
	{10, 10, 10,  5, 20, 10, 20, 10, 10, 10, 10, 20,  5, 10, 10},	//Flying
	{10, 10, 10, 10, 10, 10, 20, 20, 10, 10,  5, 10, 10, 10, 10},	//Psychic
	{10,  5, 10, 10, 20, 10,  5, 20, 10,  5, 20, 10, 10,  5, 10},	//Bug
	{10, 20, 10, 10, 10, 20,  5, 10,  5, 20, 10, 20, 10, 10, 10},	//Rock
	{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 10},	//Ghost
	{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20}	//Dragon
};






char getTypeMultiplier(PokemonType attackType, PokemonType targetPokemonType)
{
	assert(attackType < TYPE_MAX);
	assert(targetPokemonType < TYPE_MAX);

	char multiplier = multiplierChart[attackType][targetPokemonType];
	return multiplier;
}

const char* ptos(PokemonType pType)
{
	assert(pType < TYPE_MAX);

	const char* typeString = PokemonTypeStrings[pType];
	return typeString;
}