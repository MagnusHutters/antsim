#pragma once
#include "MapContainer.h"
#include "ObstacleMap.h"
#include "JobMap.h"
#include "Pheromones2.h"

class TerrainFactory
{
public:
	explicit TerrainFactory(const MapContainer& m)
		: m(m)
	{
		srand(RANDOM_SEED+100);

		for (int y = 0; y < OBSTACLE_MAP_HEIGHT; ++y)
		{
			setObstructed({ 0,y });
			setObstructed({ OBSTACLE_MAP_WIDTH-1,y });
		}

		for (int x = 0; x < OBSTACLE_MAP_WIDTH; ++x)
		{
			setObstructed({ x,0 });
			setObstructed({ x,OBSTACLE_MAP_HEIGHT-1 });
		}
	}

	void update()
	{
		tryCreateTerrain();
		tryDeleteTerrain();
	}





	void progressTerrain(int number=1)
	{
		for (int i = 0; i < number; ++i)
		{
			update();
		}
	}

	void tryCreateTerrain(int number, bool mustBeConnected= true, bool center=false)
	{
		for (int i = 0; i < number; ++i)
		{
			tryCreateTerrain(mustBeConnected, center);
		}
	}

	

	void tryDeleteTerrain()
	{
		IntCoords p = m.obstacleMap->getMapCoords(Vector2::RandomWithinMap());
		IntCoords rectPos = { p.x * OBSTACLE_CELL_SIZE, p.y * OBSTACLE_CELL_SIZE } ;
		IntCoords rectSize = { OBSTACLE_CELL_SIZE, OBSTACLE_CELL_SIZE };
		

		if (!m.obstacleMap->isObstructed(p)) return; //Exit: If not terrain


		int count = 0;
		int clearCount = 0;

		std::vector<bool> isAround;
		isAround.reserve(8);
		std::list<IntCoords> around = {
			{p.x - 1,p.y - 1},
			{p.x - 0,p.y - 1},
			{p.x + 1,p.y - 1},
			{p.x + 1,p.y - 0},
			{p.x + 1,p.y + 1},
			{p.x + 0,p.y + 1},
			{p.x - 1,p.y + 1},
			{p.x - 1,p.y + 0},
		};
		IntCoords last = m.obstacleMap->getInsideBounds(around.back());
		bool differ = !m.obstacleMap->isObstructed(last);


		for (IntCoords& currentCoord : around)
		{
			m.obstacleMap->getInsideBounds(currentCoord);
			bool obstrcutionTest = !m.obstacleMap->isObstructed(currentCoord);
			clearCount += obstrcutionTest;
			isAround.push_back(obstrcutionTest);
			count += differ ^ obstrcutionTest;

			differ = obstrcutionTest;
		}

		if(count==2)
		{
			if (isAround[1] || isAround[3] || isAround[5] || isAround[7])
			{
			
				int d8 = (rand() % 8);
				if (d8 < clearCount)
				{
					clearObstructed(p);
					
				}
			}

		}
	}

	void clearObstructed(const IntCoords& p)
	{
		IntCoords rectPos = { p.x * OBSTACLE_CELL_SIZE, p.y * OBSTACLE_CELL_SIZE };
		IntCoords rectSize = { OBSTACLE_CELL_SIZE, OBSTACLE_CELL_SIZE };

		m.obstacleMap->clearObstructed(p);
		m.pheromoneMap->cleatObstructedRect(rectPos, rectSize);
	}
	void setObstructed(const IntCoords& p)
	{
		IntCoords rectPos = { p.x * OBSTACLE_CELL_SIZE, p.y * OBSTACLE_CELL_SIZE };
		IntCoords rectSize = { OBSTACLE_CELL_SIZE, OBSTACLE_CELL_SIZE };

		m.obstacleMap->setObstructed(p);
		m.pheromoneMap->setObstructedRect(rectPos, rectSize);
	}

	void tryCreateTerrain(bool mustBeConnected=true, bool isCenter=true)
	{
		IntCoords p;
		if(isCenter)
		{
			p = m.obstacleMap->getMapCoords((Vector2::RandomWithinMap() / 2) + Vector2(MAP_WIDTH / 4, MAP_HEIGHT / 4));
		}else
		{
			p = m.obstacleMap->getMapCoords(Vector2::RandomWithinMap() );
		}
		 
		Vector2 rectPos= Vector2( (float)p.x, (float)p.y )*OBSTACLE_CELL_SIZE;
		Vector2 rectSize = Vector2(OBSTACLE_CELL_SIZE, OBSTACLE_CELL_SIZE);

		IntCoords rectPosInt = { p.x * OBSTACLE_CELL_SIZE, p.y * OBSTACLE_CELL_SIZE };
		IntCoords rectSizeInt = { OBSTACLE_CELL_SIZE, OBSTACLE_CELL_SIZE };

		if (m.obstacleMap->isObstructed(p)) return; //Exit : if already terrain

		if (m.jobMap->areaIsOccupied(rectPos, rectSize)) return; //Exit : if occupied by job

		
		
		int count = 0;
		int terrainCount = 0;

		std::vector<bool> isAround;
		isAround.reserve(8);
		std::list<IntCoords> around = {
			{p.x - 1,p.y - 1},
			{p.x - 0,p.y - 1},
			{p.x + 1,p.y - 1},
			{p.x + 1,p.y - 0},
			{p.x + 1,p.y + 1},
			{p.x + 0,p.y + 1},
			{p.x - 1,p.y + 1},
			{p.x - 1,p.y + 0},
		};
		IntCoords last = m.obstacleMap->getInsideBounds(around.back());
		bool differ = m.obstacleMap->isObstructed(last);


		for (IntCoords& currentCoord : around)
		{
			m.obstacleMap->getInsideBounds(currentCoord);
			bool obstrcutionTest= m.obstacleMap->isObstructed(currentCoord);
			terrainCount += obstrcutionTest;
			isAround.push_back(obstrcutionTest);
			count += differ ^ obstrcutionTest;
			
			differ = obstrcutionTest;
		}
		if(count==0)
		{
			if(!mustBeConnected)
			{
				m.obstacleMap->setObstructed(p);
				m.pheromoneMap->setObstructedRect(rectPosInt, rectSizeInt);
			}

		}else if(count==2)
		{
			if(isAround[1]|| isAround[3] || isAround[5] || isAround[7] )
			{
				int d8 = (rand() % 8)-1;
				if (d8 < terrainCount)
				{
					m.obstacleMap->setObstructed(p);
					m.pheromoneMap->setObstructedRect(rectPosInt, rectSizeInt);
				}

			
			}
		}
		else if(count==1)
		{
			throw("PARITY ERRROR: should not be posible");
		}
		
	}

private:
	MapContainer m;
};

