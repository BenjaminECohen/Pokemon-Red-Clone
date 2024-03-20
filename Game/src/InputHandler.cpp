
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"
#include <assert.h>
#include <string.h>
#include "sound.h"

#include "stateManager.h"
#include "BattleUIManager.h"

#include "InputHandler.h"
#include "pokemon.h"

#define HP_BAR_CHANGE_RATE_S (5);

uint32_t menuTimer = 0;
uint32_t mCurrentTimer = 0;
double hpBarTarget = 0;
double currHPDisplay = 0;

MoveT* queuedPlayerMove = nullptr;
int	 playerSwapIndex;
char effectiveness = 0;
bool playerWin = false;

bool animationGoing = false;

bool opponentStartDone = false;

int defeatMessageIndex = 0;



//Always used lines
const char* Used		= "used \0";
const char* Fainted		= "fainted!\0";
const char* Enemy		= "Enemy \0";
const char* Wild		= "Wild \0";
const char* atkMiss		= "attack missed!\0";
const char* ap_s		= "%\0";
const char* ap_t		= "@\0";

//Menu stuff
int menuPokemonIndex = 0;
bool menuNeedStart = true;

//Sound stuff
int menuSoundID;
int battleSoundID;
int victorySoundID;
int AB_SoundID;

void InputHandler::Update(uint32_t milliseconds, Trainer* player, Trainer* opponent)
{
	StateManagerC* stateMachine = StateManagerC::GetInstance();
	BattleUIManager * uiManager = BattleUIManager::GetInstance();

	char c_Buffer_One[255] = "";
	char c_Buffer_Two[255] = "";


	switch (stateMachine->getState())
	{
		case (StateManagerC::MENU):

			if (menuNeedStart)
			{
				Sound::Playback(menuSoundID);
				menuNeedStart = false;
				//Generate random team
				
				
				//Free squirtles!
				
				//player.CatchPokemon(PokemonManager::GetPokemon(8));
				//player.CatchPokemon(PokemonManager::GetPokemon(2));

				//opponent->CatchPokemon(PokemonManager::GetPokemon(8));
				//opponent->CatchPokemon(PokemonManager::GetPokemon(7));
			}

			if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				stateMachine->setState(StateManagerC::START);
				stateMachine->setBattleTextState(StateManagerC::B_Text_State::ENEMY_SWAP_PKMN);
				uiManager->setState(BattleUIManager::TEXT);
				uiManager->setTextState(BattleUIManager::INTRO_TEXT);

				//uiManager->renderPlayerPokemon = true;

				strcat(c_Buffer_One, opponent->GetName());
				strcat(c_Buffer_One, " wants");
				uiManager->SetTextLine(c_Buffer_One, 1);
				uiManager->SetTextLine("to fight!", 2);

				uiManager->renderOpponentSprite = true;
				uiManager->renderPlayerSprite = true;

				Sound::Stop(menuSoundID);
				Sound::Playback(battleSoundID);

				g_keys->keyDown[VK_RETURN] = false;
			}

			break;
		
		case (StateManagerC::START):
			
			if (g_keys->keyDown[VK_RETURN] && !opponentStartDone)
			{
				Sound::Playback(AB_SoundID);
				if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::ENEMY_SWAP_PKMN)
				{
					uiManager->setTextState(BattleUIManager::INTRO_TEXT);
					strcat(c_Buffer_One, opponent->GetName());
					strcat(c_Buffer_One, " sent\0");
					uiManager->SetTextLine(c_Buffer_One, 1);

					strcat(c_Buffer_Two, "out ");
					strcat(c_Buffer_Two, opponent->GetCurrPokemon()->GetName());
					strcat(c_Buffer_Two, "!");
					uiManager->SetTextLine(c_Buffer_Two, 2);

					stateMachine->progressBattleText();

					uiManager->renderOpponentSprite = false;
					
					
				
				}	
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::ENEMY_SWAP_ANIM)
				{
					uiManager->SetTextLine(" ", 1);
					uiManager->SetTextLine(" ", 2);
					//uiManager->SetTextLine("Enemy PKMN\0", 1);
					//uiManager->SetTextLine("swap here!\0", 2);

					stateMachine->setBattleTextState(StateManagerC::B_Text_State::SEND_OUT_NEW_PKMN);

					uiManager->renderOpponentPokemon = true;
					opponentStartDone = true;

				}
				g_keys->keyDown[VK_RETURN] = false;
			}
			

			if (g_keys->keyDown[VK_RETURN] && opponentStartDone)
			{
				Sound::Playback(AB_SoundID);
				if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SEND_OUT_NEW_PKMN)			//No pokemon shown, will display GO! NAME! after click
				{
					strcat(c_Buffer_One, "Go! ");
					strcat(c_Buffer_One, player->GetCurrPokemon()->GetName());
					strcat(c_Buffer_One, "!\0");
					uiManager->SetTextLine(c_Buffer_One, 1);
					uiManager->SetTextLine(" \0", 2);

					stateMachine->progressBattleText();
					uiManager->renderPlayerSprite = false;

				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SWAP_IN_PKMN_ANIM)			//Animation for swapping in players
				{
					stateMachine->setState(StateManagerC::ACTION);
					uiManager->setState(BattleUIManager::BASE);
					uiManager->renderPlayerPokemon = true;
				}
				
				g_keys->keyDown[VK_RETURN] = false;
			}
			break;


		case (StateManagerC::BATTLE_TEXT):
			 
			//Automatic faint checker
			

			//Animation states
			if (animationGoing)
			{
				if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::ADJUST_HP)		//Used text is on display
				{
					if (currHPDisplay != hpBarTarget)																		//If the currentHP displayed does not equal actual
					{

						mCurrentTimer += milliseconds;

						//current -= HP_PER_SECOND * (double)mCurrentTimer
						currHPDisplay -= ((double)mCurrentTimer / 1000.0) * HP_BAR_CHANGE_RATE_S;

						//Update health display
						if (stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)
						{
							uiManager->UpdateTempHealth(currHPDisplay, true);
						}
						else
						{
							uiManager->UpdateTempHealth(currHPDisplay, false);
						}

						if (currHPDisplay < hpBarTarget)
						{
							currHPDisplay = hpBarTarget;
							uiManager->DisableTempHealth();
							stateMachine->progressBattleText();
							currHPDisplay = 0;
							hpBarTarget = 0;
							animationGoing = false;

							if (effectiveness == 10)					//If the effectiveness was 10, skip the next state
							{
								stateMachine->progressBattleText();
							}

						}

					}
					else if (currHPDisplay == hpBarTarget)
					{
						currHPDisplay = hpBarTarget;
						uiManager->DisableTempHealth();
						stateMachine->progressBattleText();
						currHPDisplay = 0;
						hpBarTarget = 0;
						animationGoing = false;

						if (effectiveness == 10)					//If the effectiveness was 10, skip the next state
						{
							stateMachine->progressBattleText();
						}
					}
					
				}
			}

			//Controlled states that need user input to proceed
			else if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				//Main Battle Flow
				if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::CALCULATE_DAMAGE)				//Used text is on display
				{
					stateMachine->progressBattleText();  //ADJUST HP NEXT

					Pokemon* attacker;

					//Actor move fires off
					if (stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)									//Is the enemy the current actor? Then attack
					{
						attacker = opponent->GetCurrPokemon();
						MoveT* opMove = attacker->GetMove(0);

						currHPDisplay = player->GetCurrPokemon()->GetCurrHealth();
						effectiveness = player->GetCurrPokemon()->DealDamage(opMove->move, *opponent->GetCurrPokemon());
						hpBarTarget = player->GetCurrPokemon()->GetCurrHealth();

						uiManager->UseTempHealth(currHPDisplay, true);		//Player has temp health active
					}
					else																							//Is the player the current actor? Then attack
					{
						attacker = player->GetCurrPokemon();

						currHPDisplay = opponent->GetCurrPokemon()->GetCurrHealth();
						effectiveness = opponent->GetCurrPokemon()->DealDamage(queuedPlayerMove->move, *player->GetCurrPokemon());
						hpBarTarget = opponent->GetCurrPokemon()->GetCurrHealth();

						uiManager->UseTempHealth(currHPDisplay, false);		//Opponent has temp health active
					}

					animationGoing = true;

					if (effectiveness == -1)										//If move missed, dont adjust hp, dont check if fainted, dont check if defeat
					{
						strcat(c_Buffer_One, attacker->GetName());
						strcat(c_Buffer_One, ap_s);
						uiManager->SetTextLine(c_Buffer_One, 1);

						uiManager->SetTextLine(atkMiss, 2);

						stateMachine->setBattleTextState(StateManagerC::B_Text_State::CHECK_IF_DONE);
						animationGoing = false;
					}

					mCurrentTimer = 0;



				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::STATE_EFFECTIVENESS)
				{
					stateMachine->progressBattleText();

					if (effectiveness == -2)								//Move doesnt affect at all
					{
						uiManager->SetTextLine("It doesn@ affect\0", 1);

						if (stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)
						{
							strcat(c_Buffer_Two, player->GetCurrPokemon()->GetName());
						}
						else
						{

							strcat(c_Buffer_Two, Enemy);
							strcat(c_Buffer_Two, opponent->GetCurrPokemon()->GetName());

						}
						strcat(c_Buffer_Two, "!");
						uiManager->SetTextLine(c_Buffer_Two, 2);
					}
					else if (effectiveness > 10)							//Super Effective
					{
						uiManager->SetTextLine("It% super\0", 1);
						uiManager->SetTextLine("effective!\0", 2);
					}
					else													//Not very effective
					{
						uiManager->SetTextLine("It% not very\0", 1);
						uiManager->SetTextLine("effective...\0", 2);
					}

					if (opponent->GetCurrPokemon()->GetCurrHealth() != 0 && stateMachine->getCurrActor() == StateManagerC::T_PLAYER)
					{
						stateMachine->setBattleTextState(StateManagerC::B_Text_State::CHECK_IF_DONE);
					}
					if (player->GetCurrPokemon()->GetCurrHealth() != 0 && stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)				//Check opposing current health. If they are alive, skip Fainted and defeat
					{
						stateMachine->setBattleTextState(StateManagerC::B_Text_State::CHECK_IF_DONE);
					}

				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::CHECK_IF_FAINTED)					//Check if anyone fainted
				{
					stateMachine->progressBattleText();
					//uimanager change text to NEXT state

					if (stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)					//Check if opponent defeated player
					{
						if (player->GetCurrPokemon()->GetCurrHealth() == 0)							//Current player pokemon fainted
						{
							uiManager->SetTextLine(player->GetCurrPokemon()->GetName(), 1);
							uiManager->SetTextLine(Fainted, 2);
							uiManager->renderPlayerPokemon = false;
						}
						else
						{
							stateMachine->progressBattleText();												//Player did not faint, so skip defeat check
						}

					}
					else																			//Check if player defeated opponent
					{
						if (opponent->GetCurrPokemon()->GetCurrHealth() == 0)						//Current opponent pokemon fainted
						{
							strcat(c_Buffer_One, Enemy);
							strcat(c_Buffer_One, opponent->GetCurrPokemon()->GetName());
							uiManager->SetTextLine(c_Buffer_One, 1);
							uiManager->SetTextLine(Fainted, 2);

							uiManager->renderOpponentPokemon = false;

						}
						else																				//Opponent did nto faint, so skip defeat check
						{
							stateMachine->progressBattleText();
						}
					}


				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::CHECK_IF_DEFEATED)				//See if the battle is over
				{
					//Check if opponent is alive

					if (stateMachine->getCurrActor() == StateManagerC::T_OPPONENT)					//Check if opponent defeated player
					{
						if (player->IsDefeated())
						{
							stateMachine->setState(StateManagerC::BATTLE_END);
							uiManager->setState(BattleUIManager::TEXT);
							playerWin = false;
						}
						else																		//Player not defeated, choose pkmn to send out
						{
							stateMachine->setState(StateManagerC::PLAYER_PKMN_FAINT);
							uiManager->setTeamIndexMax(*player);
							uiManager->setTeamIndex(0);
							uiManager->setState(BattleUIManager::POKEMONLIST);

							uiManager->SetTextLine(" \0", 1);
							uiManager->SetTextLine(" \0", 2);

						}

					}
					else																			//Check if player defeated opponent
					{
						if (opponent->IsDefeated())
						{
							stateMachine->setState(StateManagerC::BATTLE_END);
							uiManager->setState(BattleUIManager::TEXT);
							playerWin = true;
						}
						else																		//Opponent not defeated, send out next pokemon
						{
							int newIndex = opponent->GetCurrentPokemonIndex() + 1;
							opponent->SwitchCurrentPokemon(newIndex);
							//stateMachine->setState(StateManagerC::ACTION);
							//uiManager->setState(BattleUIManager::BASE);

							stateMachine->setBattleTextState(StateManagerC::B_Text_State::ENEMY_SWAP_PKMN);

							uiManager->renderOpponentPokemon = false;

						}
					}

					//stateMachine->progressBattleText();
					

				}			
				

				//For Pokemon Swapping
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SWAP_OUT_PKMN)				//Pokemon is going to be swapped out
				{
					stateMachine->progressBattleText();

					uiManager->SetTextLine(" ", 1);
					uiManager->SetTextLine(" ", 2);

					//uiManager->SetTextLine("PKMN swap", 1);
					//uiManager->SetTextLine("here!\0", 2);

				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SWAP_OUT_PKMN_ANIM)			//Animation will play
				{
					stateMachine->progressBattleText();


					//uiManager->SetTextLine("PKMN swap", 1);
					//uiManager->SetTextLine("done!\0", 2);

					uiManager->renderPlayerPokemon = false;
					
				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SEND_OUT_NEW_PKMN)			//No pokemon shown, will display GO! NAME! after click
				{
					stateMachine->progressBattleText();

					strcat(c_Buffer_One, "Go! ");
					strcat(c_Buffer_One, player->GetPokemon(playerSwapIndex)->GetName());
					strcat(c_Buffer_One, "!\0");
					uiManager->SetTextLine(c_Buffer_One, 1);
					uiManager->SetTextLine(" \0", 2);

				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::SWAP_IN_PKMN_ANIM)			//Animation for swapping in players
				{
					player->SwitchCurrentPokemon(uiManager->getTeamIndex());
					stateMachine->setBattleTextState(StateManagerC::B_Text_State::CHECK_IF_DONE);
					uiManager->renderPlayerPokemon = true;
				}
				
				
				//Opponent Pokemon Swap
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::ENEMY_SWAP_PKMN)
				{
					strcat(c_Buffer_One, opponent->GetName());
					strcat(c_Buffer_One, " sent\0");
					uiManager->SetTextLine(c_Buffer_One, 1);

					strcat(c_Buffer_Two, "out ");
					strcat(c_Buffer_Two, opponent->GetCurrPokemon()->GetName());
					strcat(c_Buffer_Two, "!");
					uiManager->SetTextLine(c_Buffer_Two, 2);

					stateMachine->progressBattleText();
				}
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::ENEMY_SWAP_ANIM)
				{
					uiManager->SetTextLine(" \0", 1);
					uiManager->SetTextLine(" \0", 2);
					//uiManager->SetTextLine("Enemy PKMN\0", 1);
					//uiManager->SetTextLine("swap here!\0", 2);

					stateMachine->setBattleTextState(StateManagerC::B_Text_State::CHECK_IF_DONE);
					stateMachine->setActorsDone();

					uiManager->renderOpponentPokemon = true;

				}
				
				
				
				else if (stateMachine->getBattleTextState() == StateManagerC::B_Text_State::CHECK_IF_DONE)			//Opposing actor is alive and turn is over
				{
					if (stateMachine->allActorsTookTurn())	//If TRUE, everyone has acted and is alive, so go to next turn start
					{
						stateMachine->setState(StateManagerC::State::ACTION);
						uiManager->setState(BattleUIManager::BASE);
					}
					else
					{
						stateMachine->nextActorTurn();
						//Go back to used text state

						if (stateMachine->getCurrActor() == StateManagerC::T_PLAYER)
						{
							uiManager->SetTextLine(player->GetCurrPokemon()->GetName(), 1);

							strcat(c_Buffer_Two, Used);
							strcat(c_Buffer_Two, queuedPlayerMove->move.GetName());
							strcat(c_Buffer_Two, "!");
							uiManager->SetTextLine(c_Buffer_Two, 2);
						}
						else
						{
							strcat(c_Buffer_One, Enemy);
							strcat(c_Buffer_One, opponent->GetCurrPokemon()->GetName());
							uiManager->SetTextLine(c_Buffer_One, 1);

							strcat(c_Buffer_Two, Used);
							strcat(c_Buffer_Two, opponent->GetCurrPokemon()->GetMove(0)->move.GetName());
							strcat(c_Buffer_Two, "!");
							uiManager->SetTextLine(c_Buffer_Two, 2);
						}

					}

				}
				
				

				g_keys->keyDown[VK_RETURN] = false;

			}
			

			break;


		case (StateManagerC::ACTION):
			
			switch (uiManager->getState())
			{
			case (BattleUIManager::BASE):

				//Select
				if (g_keys->keyDown[VK_RETURN])
				{

					Sound::Playback(AB_SoundID);
					//GET INDEX OF PLAYER DECISION
					char index = uiManager->getActionIndex();

					switch (index)
					{
					case (0):
						uiManager->setMoveIndexMax(*player->GetCurrPokemon());
						uiManager->setMoveIndex(0);
						uiManager->setState(BattleUIManager::MOVELIST);
						break;
					case (1):
						uiManager->setTeamIndexMax(*player);
						uiManager->setTeamIndex(0);
						uiManager->setState(BattleUIManager::POKEMONLIST);
						break;
					case (2):
						uiManager->setState(BattleUIManager::BAG);
						break;
					case (3):
						stateMachine->setState(StateManagerC::RUN);					//Run
						uiManager->setState(BattleUIManager::TEXT);
						uiManager->setTextState(BattleUIManager::BATTLE_TEXT);

						uiManager->SetTextLine("Can@ escape!\0", 1);
						uiManager->SetTextLine(" \0", 2);

						break;
					default:
						printf("Shouldn't be here\n");
						break;
					}

					g_keys->keyDown[VK_RETURN] = false;

				}
				else if (g_keys->keyDown['D'])													//Action Indexes
				{
					if (uiManager->moveActionIndex(1) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					
					g_keys->keyDown['D'] = false;
				}
				else if (g_keys->keyDown['A'])													//Action Indexes
				{
					if (uiManager->moveActionIndex(-1) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					g_keys->keyDown['A'] = false;
				}
				else if (g_keys->keyDown['W'])													//Action Indexes
				{
					if (uiManager->moveActionIndex(-2) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					g_keys->keyDown['W'] = false;
				}
				else if (g_keys->keyDown['S'])													//Action Indexes
				{
					if (uiManager->moveActionIndex(2) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					g_keys->keyDown['S'] = false;
				}


				break;

			case (BattleUIManager::MOVELIST):

				if (g_keys->keyDown[VK_RETURN])
				{
					Sound::Playback(AB_SoundID);
					//EXECUTE MOVE
					int moveIndex = uiManager->getMoveIndex();


					if (player->GetCurrPokemon()->UseMove(player->GetCurrPokemon()->GetMove(moveIndex)))												//If true, we have the pp, so execute
					{
						//opponent->GetCurrPokemon()->DealDamage(playerMove->move, *player->GetCurrPokemon());
							
						queuedPlayerMove = player->GetCurrPokemon()->GetMove(moveIndex);																//Queue player move to use when needed

							
						if (player->GetCurrPokemon()->GetSpeed() >= opponent->GetCurrPokemon()->GetSpeed())			//Check to see who is faster
						{
							stateMachine->executeTurn(StateManagerC::T_PLAYER);										//Player was faster


							uiManager->SetTextLine(player->GetCurrPokemon()->GetName(), 1);

							strcat(c_Buffer_Two, Used);
							strcat(c_Buffer_Two, queuedPlayerMove->move.GetName());
							strcat(c_Buffer_Two, "!");
							uiManager->SetTextLine(c_Buffer_Two, 2);
						}
						else
						{
							stateMachine->executeTurn(StateManagerC::T_OPPONENT);									//Opponent was faster

							strcat(c_Buffer_One, Enemy);
							strcat(c_Buffer_One, opponent->GetCurrPokemon()->GetName());
							uiManager->SetTextLine(c_Buffer_One, 1);

							strcat(c_Buffer_Two, Used);
							strcat(c_Buffer_Two, opponent->GetCurrPokemon()->GetMove(0)->move.GetName());
							strcat(c_Buffer_Two, "!");
							uiManager->SetTextLine(c_Buffer_Two, 2);
								
						}
						uiManager->setState(BattleUIManager::TEXT);
						uiManager->setTextState(BattleUIManager::BATTLE_TEXT);
							
						//int32_t state = stateMachine->progressState();
						//uiManager->setState(BattleUIManager::TEXT);
					}


					g_keys->keyDown[VK_RETURN] = false;
				}
				else if (g_keys->keyDown[VK_BACK])
				{
					Sound::Playback(AB_SoundID);
					uiManager->setActionIndex(0);
					uiManager->cancelMode();
					g_keys->keyDown[VK_BACK] = false;
				}
				else if (g_keys->keyDown['W'])													//Action Indexes
				{
					uiManager->moveMoveIndex(-1);
					Sound::Playback(AB_SoundID);
					g_keys->keyDown['W'] = false;
				}
				else if (g_keys->keyDown['S'])													//Action Indexes
				{
					uiManager->moveMoveIndex(1);
					Sound::Playback(AB_SoundID);
					g_keys->keyDown['S'] = false;
				}


				break;

			case (BattleUIManager::BAG):

				if (g_keys->keyDown[VK_RETURN])			//Open
				{
					//Open
					Sound::Playback(AB_SoundID);
					uiManager->cancelMode();
					g_keys->keyDown[VK_RETURN] = false;
				}
				else if (g_keys->keyDown[VK_BACK])			//Close
				{

					Sound::Playback(AB_SoundID);
					uiManager->cancelMode();
					g_keys->keyDown[VK_BACK] = false;
				}
				break;

			case (BattleUIManager::POKEMONLIST):

				if (g_keys->keyDown[VK_RETURN])
				{
					//EXECUTE Switch
					Sound::Playback(AB_SoundID);
					//*********
					//bool success = player->SwitchCurrentPokemon(uiManager->getTeamIndex());   
					playerSwapIndex = player->CanSwapToPokemon(uiManager->getTeamIndex());							//Save the index

					if (playerSwapIndex != -1)
					{
						stateMachine->setState(StateManagerC::BATTLE_TEXT);
						stateMachine->executeTurn(StateManagerC::T_PLAYER, StateManagerC::SWAP_OUT_PKMN);				//Opponent will go first, but swap out player pkmn
						

						uiManager->setState(BattleUIManager::TEXT);
						uiManager->setTextState(BattleUIManager::BATTLE_TEXT);

						strcat(c_Buffer_One, player->GetCurrPokemon()->GetName());
						strcat(c_Buffer_One, " enough!\0");
						uiManager->SetTextLine(c_Buffer_One, 1);
						uiManager->SetTextLine("Come back!\0", 2);

					}
					//*********

					g_keys->keyDown[VK_RETURN] = false;
				}
				else if (g_keys->keyDown[VK_BACK])
				{
					Sound::Playback(AB_SoundID);
					uiManager->cancelMode();
					g_keys->keyDown[VK_BACK] = false;
				}
				else if (g_keys->keyDown['W'])													//Action Indexes
				{
					if (uiManager->moveTeamIndex(-1) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					
					g_keys->keyDown['W'] = false;
				}
				else if (g_keys->keyDown['S'])													//Action Indexes
				{
					if (uiManager->moveTeamIndex(1) != -1)
					{
						Sound::Playback(AB_SoundID);
					}
					g_keys->keyDown['S'] = false;
				}
				break;
			}
			
			
			
			break;

		case (StateManagerC::PLAYER_PKMN_FAINT):
			
			if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				bool success = player->SwitchCurrentPokemon(uiManager->getTeamIndex());
				if (success)
				{
					playerSwapIndex = player->GetCurrentPokemonIndex();
					stateMachine->setState(StateManagerC::BATTLE_TEXT);
					
					stateMachine->setBattleTextState(StateManagerC::B_Text_State::SEND_OUT_NEW_PKMN);

					uiManager->setState(BattleUIManager::TEXT);
					uiManager->setTextState(BattleUIManager::BATTLE_TEXT);

					stateMachine->setActorsDone();							//End turn once Check if done comes

				}

				g_keys->keyDown[VK_RETURN] = false;
			}
			else if (g_keys->keyDown['W'])													//Action Indexes
			{
				if (uiManager->moveTeamIndex(-1) != -1)
				{
					Sound::Playback(AB_SoundID);
				}
				g_keys->keyDown['W'] = false;
			}
			else if (g_keys->keyDown['S'])													//Action Indexes
			{
				if (uiManager->moveTeamIndex(1) != -1)
				{
					Sound::Playback(AB_SoundID);
				}
				g_keys->keyDown['S'] = false;
			}
			break;

		case (StateManagerC::BATTLE_END):

			if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				if (playerWin)
				{
					Sound::Stop(battleSoundID);
					Sound::Playback(victorySoundID);
					strcat(c_Buffer_One, player->GetName());
					strcat(c_Buffer_One, " defeated\0");
					uiManager->SetTextLine(c_Buffer_One, 1);

					strcat(c_Buffer_Two, opponent->GetName());
					strcat(c_Buffer_Two, "!\0");
					uiManager->SetTextLine(c_Buffer_Two, 2);

					stateMachine->setState(StateManagerC::RETURN_TO_MENU);
				}
				else
				{
					if (defeatMessageIndex == 0)
					{
						Sound::Stop(battleSoundID);
						strcat(c_Buffer_One, opponent->GetName());
						strcat(c_Buffer_One, " defeated\0");
						uiManager->SetTextLine(c_Buffer_One, 1);

						strcat(c_Buffer_Two, player->GetName());
						strcat(c_Buffer_Two, "!\0");
						uiManager->SetTextLine(c_Buffer_Two, 2);
						defeatMessageIndex++;
					}
					else
					{
						strcat(c_Buffer_One, player->GetName());
						strcat(c_Buffer_One, " blacked\0");
						uiManager->SetTextLine(c_Buffer_One, 1);
						uiManager->SetTextLine("out!", 2);
						stateMachine->setState(StateManagerC::RETURN_TO_MENU);
					}
					
				}


				g_keys->keyDown[VK_RETURN] = false;
			}

			break;

		case (StateManagerC::RETURN_TO_MENU):

			if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				Sound::Stop(victorySoundID);
				stateMachine->setState(StateManagerC::MENU);
				uiManager->setState(BattleUIManager::MENU);

				player->RefreshTeam();
				opponent->RefreshTeam();

				uiManager->renderOpponentPokemon = false;
				uiManager->renderOpponentSprite = false;
				uiManager->renderPlayerPokemon = false;
				uiManager->renderOpponentSprite = false;
				GameReset();

				g_keys->keyDown[VK_RETURN] = false;
			}

			break;

		case (StateManagerC::RUN):

			if (g_keys->keyDown[VK_RETURN])
			{
				Sound::Playback(AB_SoundID);
				stateMachine->setState(StateManagerC::ACTION);
				uiManager->setState(BattleUIManager::BASE);


				g_keys->keyDown[VK_RETURN] = false;
			}

			break;

		default:
			assert(1 > 2);
	}



}

void InputHandler::init()
{
	menuSoundID				= Sound::LoadSound("asset/TitleScreen.wav");
	victorySoundID			= Sound::LoadSound("asset/Player_Victory.wav");
	battleSoundID			= Sound::LoadSound("asset/Battle.wav");
	AB_SoundID				= Sound::LoadSound("asset/SFX_PRESS_AB.wav");
}

void InputHandler::shutdown()
{
	Sound::UnloadSound(menuSoundID);
	Sound::UnloadSound(victorySoundID);
	Sound::UnloadSound(battleSoundID);
	Sound::UnloadSound(AB_SoundID);
}

void InputHandler::GameReset()
{
	menuTimer = 0;
	mCurrentTimer = 0;
	hpBarTarget = 0;
	currHPDisplay = 0;

	queuedPlayerMove = nullptr;
	playerSwapIndex;
	effectiveness = 0;
	playerWin = false;


	animationGoing = false;

	opponentStartDone = false;
	defeatMessageIndex = 0;

	menuNeedStart = true;
	

}






