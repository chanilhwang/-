
#ifndef OCEAN_H
#define OCEAN_H

#include "WarBoats.h"

//! Namespace for CS170 class
namespace CS170
{
//! Namespace for the WarBoats assignment
namespace WarBoats
{
    const int BOAT_LENGTH = 4; 
    const int HIT_OFFSET = 100;

    class Ocean
    {
      public:
        /*
         * Other Public Functions
        */

        // Provided
        const int* GetGrid(void) const;
        Point      GetDimensions(void) const;

      private:
        int* m_pGrid;       
        int  m_xQuadrants; 
        int  m_yQuadrants; 
        
        /*
         * Other private data
        */

    }; // class Ocean

} // namespace WarBoats
} // namespace CS170

#endif // OCEAN_H
////////////////////////////////////////////////////////////////////////////////
