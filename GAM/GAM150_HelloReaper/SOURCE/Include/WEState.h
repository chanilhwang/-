/******************************************************************************/
/*!
\file   WEState.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Typedefs and struct for my WEState

*/
/******************************************************************************/
#ifndef WE_STATE_H
#define WE_STATE_H

/*!A pointer type for State functions */
typedef void(*WEFunc)(void);
/*! A pointer type for State Update Functions.*/
typedef void(*WEUpdateFunc)(float dt);


/*! A struct to hold the function pointers for states.*/
struct WEState
{
  WEFunc       Load;    /*!< A states Load function pointer*/
  WEFunc       Init;    /*!< A states Init function pointer*/
  WEUpdateFunc Update;  /*!< A states update function pointer*/
  WEFunc       Shutdown;/*!< A states Shutdown function pointer*/
  WEFunc       Unload;  /*!< A states unload function pointer*/
};

#endif