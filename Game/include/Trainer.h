#include "pokemon.h"

#define PLAYER_MAX_NAME_LENGTH (11)
#define MAX_TEAM_SIZE (6)

#define TRAINER_IMAGES (9)


const char* getNameByImage(int imageIndex);

class Trainer
{
public:

	Trainer();
	Trainer(const char* name);
	Trainer(const char* name, int imageIndex);
	char* GetName();
	int GetImageIndex() { return imageIndex; }
	
	Pokemon* SendOutFirstPokemon() { this->currentPokemonIndex = 0; return GetPokemon(currentPokemonIndex); }
	int GetCurrentPokemonIndex() { return this->currentPokemonIndex; }

	int CanSwapToPokemon(int listIndex);
	bool SwitchCurrentPokemon(int listIndex);
	

	Pokemon* GetPokemon(char index);
	Pokemon* GetCurrPokemon();
	char CatchPokemon(Pokemon target);

	void SetName(const char* name);
	void SetTrainerIndex(int index) { imageIndex = index; }

	void shutdown();
	
	bool IsDefeated();

	void RefreshTeam();

	


private:

	int currentPokemonIndex;
	Pokemon* pokemonList;
	bool listFull;
	char name[PLAYER_MAX_NAME_LENGTH];
	int imageIndex = 6;
	


};

