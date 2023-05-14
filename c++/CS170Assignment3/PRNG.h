/******************************************************************************/
/*!
\file   PRNG.h
\author Matt Casanova 
\par    email: mcasanov\@digipen.edu
\par    CS170
\par    Assignment #1
\date   2012/31/1
\brief  
This is the header file for pseudo random number generator prototypes.

*/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
#ifndef PRNG_H
#define PRNG_H
////////////////////////////////////////////////////////////////////////////////

namespace CS170
{
  //! For helper functions 
  namespace Utils
  {
    unsigned rand(void);              // returns a random 32-bit integer
    void srand(unsigned, unsigned);   // seed the generator
    int Random(int low, int high);    // range
  }
}

#endif
////////////////////////////////////////////////////////////////////////////////
