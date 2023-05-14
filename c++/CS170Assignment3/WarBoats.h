/******************************************************************************/
/*!
\file   WarBoats.h
\author Matt Casanova 
\par    email: mcasanov\@digipen.edu
\par    CS170
\par    Assignment #2
\date   2012/3/21
\brief  
This is the Header file for the WarBoats.h.

*/
/******************************************************************************/
#ifndef WARBOATS_H
#define WARBOATS_H

namespace CS170
{
namespace WarBoats
{
  //! The orientation of the boat
  enum Orientation { O_HORIZONTAL, O_VERTICAL };
  //! The possible results of a shot
  enum ShotResult { SR_HIT, SR_MISS, SR_DUPLICATE, SR_SUNK, SR_ILLEGAL };
  //! Damage type to the ocean.
  enum DamageType { DT_OK = 0, DT_BLOWNUP = -1 };
  //! Possible results of a placed boat.
  enum BoatPlacement { BP_ACCEPTED, BP_REJECTED };

  //! A 2D location on the grid
  struct Point
  {
    int x;//!< The x location
    int y;//!< The y location
  };

  //! A Simple representation of a boat.
  struct Boat
  {
    Orientation orientation;//!< Horizontal/Vertical
    Point position;         //!< x-y coordinate (left-top)
    int hits;               //!< Hits taken so far
    int id;                 //!< Unique ID
  };

  //! For keeping track of what happened in the game.
  struct ShotStats
  {
    int hits;      //!< The number of hits
    int misses;    //!< The number of misses
    int duplicates;//!< The number of duplicates
    int sunk;      //!< The number of boats sunk
  };

} // namespace WarBoats
} // namespace CS170

#endif // WARBOATS_H
////////////////////////////////////////////////////////////////////////////////
