#include "Ocean.h"
#include "WarBoats.h"

namespace CS170
{
namespace WarBoats
{
  /*
   * ALL STUDENT IMPLEMENTATION GOES HERE
  */ 
  const int* Ocean::GetGrid(void) const
  {
    return m_pGrid;
  }
  Point Ocean::GetDimensions(void) const
  {
    Point pt = {m_xQuadrants, m_yQuadrants};
    return pt;
  }

} // namespace WarBoats
} // namespace CS170

