#define TRAINER_CPP
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <assert.h>
#include <string.h>


#include "Trainer.h"


Trainer::Trainer()
{
	//name = (char*)malloc(PLAYER_MAX_NAME_LENGTH + 1);
	pokemonList = (Pokemon*)malloc(sizeof(Pokemon) * MAX_TEAM_SIZE);
	//memset(&name, '\0', sizeof(this->name));
	//strcpy(name, "NONAME\0");
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		// relying upon default copy construction here, which can be dangerous... in this case, I think you are safe, but you
		//  need to be aware this is happening... 
		pokemonList[i] = PokemonManager::GetPokemon(0);
	}
	listFull = false;
	currentPokemonIndex = 0;

}

Trainer::Trainer(const char* name)
{
	//name = (char*)malloc(PLAYER_MAX_NAME_LENGTH + 1);
	pokemonList = (Pokemon*)malloc(sizeof(Pokemon) * MAX_TEAM_SIZE);
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		pokemonList[i] = PokemonManager::GetPokemon(0);
	}

	//memset(&name, '\0', sizeof(this->name));
	//strncpy(this->name, name, PLAYER_MAX_NAME_LENGTH + 1);
	listFull = false;

	//memset(this->name, '\0', PLAYER_MAX_NAME_LENGTH);
	strcpy(this->name, name);

	currentPokemonIndex = 0;
}

// chain constructors, when possible, to reduce code duplication
Trainer::Trainer(const char* name, int imageIndex) : Trainer(name)
{
	this->imageIndex = imageIndex;
}

void Trainer::shutdown()
{
	//free(name);
	free(pokemonList);
}




char* Trainer::GetName()
{
	return name;
}

//Returns list index if a pokemon can be swapped, -1 if they can't
int Trainer::CanSwapToPokemon(int listIndex)
{
	if (pokemonList[listIndex].GetCurrHealth() != 0 && currentPokemonIndex != listIndex)				//Only switch out pokemon if it has health and is not the current active pokemon
	{
		return listIndex;				
	}
	return -1;
}

//Swaps the current pokemon at an index. Returns bool if successful
bool Trainer::SwitchCurrentPokemon(int listIndex)
{
	if (pokemonList[listIndex].GetCurrHealth() != 0 && currentPokemonIndex != listIndex)				//Only switch out pokemon if it has health and is not the current active pokemon
	{
		currentPokemonIndex = listIndex;
		return true;
	}
	return false;
}

Pokemon* Trainer::GetPokemon(char index)
{
	assert(index < MAX_TEAM_SIZE);
	return &pokemonList[index];
}

char Trainer::CatchPokemon(Pokemon target)
{
	/*CHECK IF IT IS CATCHABLE
	if (OWNED)
		return -3;
	*/

	if (listFull)
	{
		return -2;
	}

	
	//Implement actual catch rate later
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		if (pokemonList[i].GetIndex() == 0)					//Means there is a free space
		{
			Pokemon::CopyPokemon(&pokemonList[i], &target);
			return 1;
		}
	}
	
	//We are now full
	listFull = true;
	return -2;
}

void Trainer::SetName(const char* name)
{
	memset(this->name, '\0', sizeof(this->name));
	strcpy(this->name, name);
}

//Checks to see if the trainer is defeated
bool Trainer::IsDefeated()
{
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		if (pokemonList[i].GetIndex() != 0 && pokemonList[i].GetCurrHealth() != 0)				//Trainer has pokemon left if a pokemon is not NULL and has health left
		{
			return false;
		}
	}
	return true;
}

void Trainer::RefreshTeam()
{
	for (int i = 0; i < MAX_TEAM_SIZE; i++)
	{
		if (pokemonList[i].GetIndex() != 0)
		{
			pokemonList[i].Restore();
		}
	}
}


Pokemon* Trainer::GetCurrPokemon()
{
	Pokemon* p = &this->pokemonList[currentPokemonIndex];
	return p;
}

const char* trainerNames[TRAINER_IMAGES] =
{
	"PLAYER\0",
	"ENGINEER\0",
	"ACETRAINER\0",

	"BLACKBELT\0",
	"ERIKA\0",
	"SUPER NERD\0",

	"YOUNGSTER\0",
	"LASS\0",
	"JR TRAINER\0"

};

const char* getNameByImage(int imageIndex)
{
	return trainerNames[imageIndex];
}
