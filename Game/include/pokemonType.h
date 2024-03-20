

#ifndef POKEMON_TYPE
#define POKEMON_TYPE

enum PokemonType
{
	NORMAL = 0,
	FIRE,
	WATER,
	ELECTRIC,
	GRASS,
	ICE,
	FIGHTING,
	POISON,
	GROUND,
	FLY,
	PSYCHIC,
	BUG,
	ROCK,
	GHOST,
	DRAGON,

	TYPE_MAX
};

char getTypeMultiplier(PokemonType attackType, PokemonType targetPokemonType);

const char* ptos(PokemonType pType);



#endif



