#include "Ocean.h"
#include "WarBoats.h"

namespace CS170
{
namespace WarBoats
{
	Ocean::Ocean(int numBoats, int xQuadrants, int yQuadrants)
	{
		//x,y size
		m_xQuadrants = xQuadrants;
		m_yQuadrants = yQuadrants;

		//grid
		m_pGrid = new int[xQuadrants * yQuadrants];
		for (int Index = 0;
			Index < xQuadrants*yQuadrants;
			Index++)
		{
			m_pGrid[Index] = 0;
		}

		//boats
		m_numBoats = numBoats;
		m_pBoats = new Boat[numBoats];
		for (int Index = 0;
			Index < numBoats;
			Index++)
		{
			m_pBoats[Index].id = 0;
		}

		//stats
		m_stats.duplicates = 0;
		m_stats.hits = 0;
		m_stats.misses = 0;
		m_stats.sunk = 0;
	}

	Ocean::~Ocean(void)
	{
		delete m_pBoats;
		delete m_pGrid;
	}

	BoatPlacement Ocean::PlaceBoat(const Boat& boat)
	{
		if ((m_pGrid[boat.position.y * m_xQuadrants + boat.position.x] == 0) &&
			(boat.position.x >= 0) && (boat.position.y >= 0) &&
			(boat.position.x < m_xQuadrants) && (boat.position.y < m_yQuadrants))
		{
			if (boat.orientation == CS170::WarBoats::O_HORIZONTAL && (boat.position.x < m_xQuadrants - 3))
			{
				if ((m_pGrid[boat.position.y * m_xQuadrants + boat.position.x] == 0) &&
					(m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 1] == 0) &&
					(m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 2] == 0) &&
					(m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 3] == 0))
				{
					m_pGrid[boat.position.y * m_xQuadrants + boat.position.x] = boat.id;
					m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 1] = boat.id;
					m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 2] = boat.id;
					m_pGrid[boat.position.y * m_xQuadrants + boat.position.x + 3] = boat.id;
					for (int Index = 0;
						Index < m_numBoats;
						Index++)
					{
						if (m_pBoats[Index].id == 0)
						{
							m_pBoats[Index] = boat;
							m_pBoats[Index].hits = 0;
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
			else if (boat.orientation == CS170::WarBoats::O_VERTICAL && (boat.position.y < m_yQuadrants - 3))
			{
				if ((m_pGrid[(boat.position.y)* m_xQuadrants + boat.position.x] == 0) &&
					(m_pGrid[(boat.position.y + 1)* m_xQuadrants + boat.position.x] == 0) &&
					(m_pGrid[(boat.position.y + 2) * m_xQuadrants + boat.position.x] == 0) &&
					(m_pGrid[(boat.position.y + 3) * m_xQuadrants + boat.position.x] == 0))
				{
					m_pGrid[boat.position.y * m_xQuadrants + boat.position.x] = boat.id;
					m_pGrid[(boat.position.y + 1) * m_xQuadrants + boat.position.x] = boat.id;
					m_pGrid[(boat.position.y + 2)* m_xQuadrants + boat.position.x] = boat.id;
					m_pGrid[(boat.position.y + 3)* m_xQuadrants + boat.position.x] = boat.id;
					for (int Index = 0;
						Index < m_numBoats;
						Index++)
					{
						if (m_pBoats[Index].id == 0)
						{
							m_pBoats[Index] = boat;
							m_pBoats[Index].hits = 0;
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

	ShotResult Ocean::TakeShot(const Point& coordinate)
	{
		if (coordinate.x < 0 || coordinate.x > m_xQuadrants ||
			coordinate.y < 0 || coordinate.y > m_yQuadrants)
		{
			return SR_ILLEGAL;
		}
		int ShotPlace = m_pGrid[coordinate.y * m_xQuadrants + coordinate.x];
		if (ShotPlace == 0)
		{
			m_stats.misses += 1;
			m_pGrid[coordinate.y * m_xQuadrants + coordinate.x] = -1;
			return SR_MISS;
		}
		else if (ShotPlace <= m_numBoats && 0 < ShotPlace)
		{
			m_stats.hits += 1;
			m_pGrid[coordinate.y * m_xQuadrants + coordinate.x] += 100;
			for (int Index = 0;
				Index < m_numBoats;
				Index++)
			{
				if (m_pBoats[Index].orientation == O_HORIZONTAL)
				{
					if (m_pBoats[Index].position.x <= coordinate.x &&
						coordinate.x <= m_pBoats[Index].position.x + 3 &&
						m_pBoats[Index].position.y == coordinate.y)
					{
						m_pBoats[Index].hits += 1;
						if (m_pBoats[Index].hits == 4)
						{
							m_stats.sunk += 1;
							return SR_SUNK;
						}
					}
				}
				else
				{
					if (m_pBoats[Index].position.y <= coordinate.y &&
						coordinate.y <= m_pBoats[Index].position.y + 3 &&
						m_pBoats[Index].position.x == coordinate.x)
					{
						m_pBoats[Index].hits += 1;
						if (m_pBoats[Index].hits == 4)
						{
							m_stats.sunk += 1;
							return SR_SUNK;
						}
					}
				}
			}
			return SR_HIT;
		}
		else
		{
			m_stats.duplicates += 1;
			return SR_DUPLICATE;
		}
	}

	ShotStats Ocean::GetShotStats(void) const
	{
		return m_stats;
	}

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

