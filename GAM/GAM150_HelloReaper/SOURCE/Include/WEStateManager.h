/******************************************************************************/
/*!
\file   WEStateManager.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Prototypes for the StateManager functions

*/
/******************************************************************************/
#ifndef WE_STATEMANAGER_H
#define WE_STATEMANAGER_H

//Forward Declarations
struct WEState;
struct WEGameData;

//! Functions to control the different states of the game.
namespace WEStateManager
{
/*Functions to initialize the State Manager*/

//Adds a state the the StateManager
int  AddState(const WEState& state);
//Sets the given state ID to the starting state of the game
void SetStartState(int startState);

/*Allows the user to get information about the State Manager*/

//Test if the game is quitting
bool IsQuitting(void);
//Test state is restarting
bool IsRestarting(void);
//Gets the previous state of the game
int  GetPreviousState(void);
//Gets the Current State of the game
int  GetCurrentState(void);
//Gets the Next state of the game
int  GetNextState(void);
//Gets the pointer to the users game specific data
WEGameData* GetGameData(void);

/*Allows the user to Set information about the State Manager*/

//Sets the next state for the game
void SetNextState(int nextState);
//Tells the game to quit
void Quit(void);
//Tells the state to restart
void Restart(void);
}//end namespace WEStateManager



#endif