/******************************************************************************/
/*!
\file   Ocean.cpp
\author Hwang Chan Il 
\par    email: c.hwang\@digipen.edu
\par    CS170
\par    Assignment #2
\duedate   16/04/2017
\brief  
This file has the implemintaiton of the prototype function found in
Warboats.h.

*/
/******************************************************************************/
#include "Warboats.h"
#include "Ocean.h"  //Ocean
#include <iostream> // cout, endl
#include <iomanip>  // setw

using namespace std;

//! Namespace for CS170
namespace CS170
{
  //! namespace for the WarBoats project
  namespace WarBoats
  {
	  Ocean* CreateOcean(int numBoats, int xSize, int ySize)
	  {
		  Ocean* pOcean = new Ocean;
		  pOcean->xSize = xSize;
		  pOcean->ySize = ySize;
		  int* Ocean_Info = new int[xSize * ySize];
		  for (int Index = 0;
			  Index < xSize*ySize;
			  Index++)
		  {
			  Ocean_Info[Index] = 0;
		  }
		  pOcean->pGrid = Ocean_Info;
		  pOcean->numBoats = numBoats;
		  Boat *Boat_Info = new Boat[numBoats];
		  for (int Index = 0;
			  Index < numBoats;
			  Index++)
		  {
			  Boat_Info[Index].id = 0;
		  }
		  pOcean->pBoats = Boat_Info;
		  pOcean->stats = { 0, //!< The number of boat hits.
						   0, //!< The number of boat misses.
						   0, //!< The number of duplicate shots.
						   0 };//!< The number of ships sunk.

		  return pOcean;
	  }

	  void DestroyOcean(Ocean* pOcean)
	  {
		  delete pOcean->pBoats;
		  delete pOcean->pGrid;
		  delete pOcean;
	  }

	  BoatPlacement PlaceBoat(Ocean& ocean, const Boat& boat)
	  {
		  if (ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x] == 0)
		  {
			  if (boat.orientation == CS170::WarBoats::O_HORIZONTAL && (boat.position.x < ocean.xSize - 3))
			  {
				  if ((ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 1] == 0) &&
					  (ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 2] == 0) &&
					  (ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 3] == 0))
				  {
					  ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x] = boat.id;
					  ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 1] = boat.id;
					  ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 2] = boat.id;
					  ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x + 3] = boat.id;
					  for (int Index = 0;
						  Index < ocean.numBoats;
						  Index++)
					  {
						  if (ocean.pBoats[Index].id == 0)
						  {
							  ocean.pBoats[Index] = boat;
							  ocean.pBoats[Index].hits = 0;
							  break;
						  }
					  }
					  return CS170::WarBoats::BP_ACCEPTED;
				  }
				  else
				  {
					  return CS170::WarBoats::BP_REJECTED;
				  }
			  }
			  else if (boat.orientation == CS170::WarBoats::O_VERTICAL && (boat.position.y < ocean.ySize - 3))
			  {
				  if ((ocean.pGrid[(boat.position.y + 1)* ocean.xSize + boat.position.x] == 0) &&
					  (ocean.pGrid[(boat.position.y + 2) * ocean.xSize + boat.position.x] == 0) &&
					  (ocean.pGrid[(boat.position.y + 3) * ocean.xSize + boat.position.x] == 0))
				  {
					  ocean.pGrid[boat.position.y * ocean.xSize + boat.position.x] = boat.id;
					  ocean.pGrid[(boat.position.y + 1) * ocean.xSize + boat.position.x] = boat.id;
					  ocean.pGrid[(boat.position.y + 2)* ocean.xSize + boat.position.x] = boat.id;
					  ocean.pGrid[(boat.position.y + 3)* ocean.xSize + boat.position.x] = boat.id;
					  for (int Index = 0;
						  Index < ocean.numBoats;
						  Index++)
					  {
						  if (ocean.pBoats[Index].id == 0)
						  {
							  ocean.pBoats[Index] = boat;
							  ocean.pBoats[Index].hits = 0;
							  break;
						  }
					  }
					  return CS170::WarBoats::BP_ACCEPTED;
				  }
				  else
				  {
					  return CS170::WarBoats::BP_REJECTED;
				  }
			  }
			  else
			  {
				  return CS170::WarBoats::BP_REJECTED;
			  }
		  }
		  else
		  {
			  return CS170::WarBoats::BP_REJECTED;
		  }
	  }

	  ShotResult TakeShot(Ocean& ocean, const Point& coordinate)
	  {
		  if (coordinate.x < 0 || coordinate.x > ocean.xSize ||
			  coordinate.y < 0 || coordinate.y > ocean.ySize)
		  {
			  return SR_ILLEGAL;
		  }
		  int ShotPlace = ocean.pGrid[coordinate.y * ocean.xSize + coordinate.x];
		  if (ShotPlace == 0)
		  {
			  ocean.stats.misses += 1;
			  ocean.pGrid[coordinate.y * ocean.xSize + coordinate.x] = -1;
			  return SR_MISS;
		  }
		  else if (ShotPlace <= ocean.numBoats && 0 < ShotPlace)
		  {
			  ocean.stats.hits += 1;
			  ocean.pGrid[coordinate.y * ocean.xSize + coordinate.x] += 100;
			  for (int Index = 0;
				  Index < ocean.numBoats;
				  Index++)
			  {
				  if (ocean.pBoats[Index].orientation == O_HORIZONTAL)
				  {
					  if (ocean.pBoats[Index].position.x <= coordinate.x &&
						  coordinate.x <= ocean.pBoats[Index].position.x + 3 &&
						  ocean.pBoats[Index].position.y == coordinate.y)
					  {
						  ocean.pBoats[Index].hits += 1;
						  if (ocean.pBoats[Index].hits == 4)
						  {
							  ocean.stats.sunk += 1;
							  return SR_SUNK;
						  }
					  }
				  }
				  else
				  {
					  if (ocean.pBoats[Index].position.y <= coordinate.y &&
						  coordinate.y <= ocean.pBoats[Index].position.y + 3 &&
						  ocean.pBoats[Index].position.x == coordinate.x)
					  {
						  ocean.pBoats[Index].hits += 1;
						  if (ocean.pBoats[Index].hits == 4)
						  {
							  ocean.stats.sunk += 1;
							  return SR_SUNK;
						  }
					  }
				  }
			  }
			  return SR_HIT;
		  }
		  else
		  {
			  ocean.stats.duplicates += 1;
			  return SR_DUPLICATE;
		  }
	  }

	  ShotStats GetShotStats(const Ocean& ocean)
	  {
		  return ocean.stats;
	  }
/******************************************************************************/
/*!

Prints the grid (ocean) to the screen.

\param ocean
The Ocean to print

\param width
How much space each position takes when printed.

\param extraLine
If true, an extra line is printed after each row. If false, no extra
line is printed.

\param showBoats
If true, the boats are shown in the output. (Debugging feature)
*/
/******************************************************************************/
    void DumpOcean(const Ocean& ocean, int width, bool extraLine, 
    bool showBoats)
    {
      // For each row
      for (int y = 0; y < ocean.ySize; y++)
      {
        // For each column
        for (int x = 0; x < ocean.xSize; x++)
        {
          int value = ocean.pGrid[y * ocean.xSize + x];

          // Is it a boat?
          if ( (value > 0) && (value < 1) && (showBoats == false) )
            value = 0;

          std::cout << std::setw(width) << value;
        }
        std::cout << std::endl;
        if (extraLine)
          std::cout << std::endl;
      }
    }

  } // namespace WarBoats
} // namespace CS170


