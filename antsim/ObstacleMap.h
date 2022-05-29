#pragma once

#include <array>

#include "Config.h"
#include "MapContainer.h"
#include "PheromoneUtility.h"
#include "Vector2.h"




class ObstacleMap
{


public:
	ObstacleMap()
	{
		
	}

	Vector2 getObstructionVector(Vector2 point)
	{
		//point *= OBSTACLE_CELL_SIZE_INVERSE;
		Vector2 localPoint = point * OBSTACLE_CELL_SIZE_INVERSE;
		IntCoords topLeft{ (int)(localPoint.x - 0.5),(int)(localPoint.y - 0.5) };
		IntCoords topRight{ topLeft.x + 1,topLeft.y };
		IntCoords bottomLeft{ topLeft.x,topLeft.y + 1 };
		IntCoords bottomRight{ topLeft.x + 1,topLeft.y + 1 };

		getInsideBounds(topLeft);
		getInsideBounds(topRight);
		getInsideBounds(bottomLeft);
		getInsideBounds(bottomRight);
		float maxDist2 = OBSTRUCTION_SENSE_RADIUS* OBSTRUCTION_SENSE_RADIUS;
		
		std::list<IntCoords> coords{ topLeft,topRight,bottomLeft,bottomRight };
		Vector2 result=Vector2(0,0);
		bool foundVector=false;
		float smallestDist2 = maxDist2;

		for (IntCoords coord : coords)
		{
			if(map[coord.x][coord.y])
			{
				Vector2 closestPoint= closestPointInSquare(point, coord);
				Vector2 centerPoint = { ((float)coord.x)+0.5f,((float)coord.y) + 0.5f };//
				centerPoint *= OBSTACLE_CELL_SIZE;
				float dist2 = closestPoint.DistanceSquared(point);
				if(dist2<=smallestDist2 &&dist2!=0)
				{
					//smallestDist2 = dist2;
					result += (centerPoint - point) / dist2;
					foundVector = true;
				}
				
				
			}
		}
		return result;
	}


	Vector2 getClosestUnobstructed(Vector2 point, float radius=0.5)
	{

		Vector2 localPoint = point* OBSTACLE_CELL_SIZE_INVERSE;
		IntCoords topLeft{ (int)(localPoint.x - 0.5),(int)(localPoint.y - 0.5) };
		IntCoords topRight{ topLeft.x+1,topLeft.y };
		IntCoords bottomLeft{ topLeft.x,topLeft.y+1 };
		IntCoords bottomRight{ topLeft.x+1,topLeft.y+1 };
	
		getInsideBounds(topLeft);
		getInsideBounds(topRight);
		getInsideBounds(bottomLeft);
		getInsideBounds(bottomRight);
		float maxDist2 = OBSTRUCTION_SENSE_RADIUS * OBSTRUCTION_SENSE_RADIUS;
		
		std::list<IntCoords> coords{ topLeft,topRight,bottomLeft,bottomRight };
		std::vector<bool> isObstructed;
		Vector2 result = Vector2();
		bool foundVector = false;
		float smallestDist2 = 99999999.0f;

		for (IntCoords coord : coords)
		{
			isObstructed.push_back(!map[coord.x][coord.y]);
			if (!map[coord.x][coord.y])
			{
				Vector2 closestPoint = closestPointInSquare(point, coord, radius);
				float dist2 = closestPoint.DistanceSquared(point);
				if (dist2 <= smallestDist2)
				{
					smallestDist2 = dist2;
					result = closestPoint;
					foundVector = true;
				}

			}
		}
		
		return result;
	}

	bool isObstructed(Vector2 point)
	{
		point *= OBSTACLE_CELL_SIZE_INVERSE;
		IntCoords p = { (int)point.x,(int)point.y };
		getInsideBounds(p);
		return map[p.x][p.y];
	}
	bool isObstructed(IntCoords point)
	{
		return map[point.x][point.y];
	}
	IntCoords getMapCoords(Vector2 point)
	{
		point *= OBSTACLE_CELL_SIZE_INVERSE;
		return { (int)point.x,(int)point.y };
	}
	Vector2 getCellCenter(IntCoords coords)
	{
		Vector2 point{ (float)coords.x,(float)coords.y };
		point = (point * OBSTACLE_CELL_SIZE) + Vector2(OBSTACLE_CELL_SIZE * 0.5, OBSTACLE_CELL_SIZE * 0.5);
		return point;
	}


	IntCoords& getInsideBounds(IntCoords& coords)
	{
		if (coords.x < 0) coords.x += OBSTACLE_MAP_WIDTH;
		else if (coords.x >= OBSTACLE_MAP_WIDTH) coords.x -= OBSTACLE_MAP_WIDTH;

		if (coords.y < 0) coords.y += OBSTACLE_MAP_HEIGHT;
		else if (coords.y >= OBSTACLE_MAP_HEIGHT) coords.y -= OBSTACLE_MAP_HEIGHT;

		return coords;

	}

	void setObstructed(const IntCoords intCoords)
	{
		
		map[intCoords.x][intCoords.y] = true;
	}
	void clearObstructed(const IntCoords intCoords)
	{
		map[intCoords.x][intCoords.y] = false;
	}

	const std::array<std::array<bool, OBSTACLE_MAP_HEIGHT>, OBSTACLE_MAP_WIDTH>& getTerrain()
	{
		return map;
	}



	bool checkLineOfSight(Vector2 point1, Vector2 point2) //True: Line not obstrcuted, False: Line obstructed
	{
		point1 *= OBSTACLE_CELL_SIZE_INVERSE;
		point2 *= OBSTACLE_CELL_SIZE_INVERSE;

		float dist = point1.Distance(point2);

		Vector2 dif=point2 - point1;
		dif.Normalize();
		dif *= OBSTACLE_SIGHT_CHECK_INTERVAL;


		Vector2 check = point1;

		for (float i = 0; i < dist; i+= OBSTACLE_SIGHT_CHECK_INTERVAL)
		{
			if(map[(int)check.x][(int)check.y])
			{
				return false;
			}
			check += dif;
		}

		return true;
	}

private:






	Vector2 closestPointInSquare(Vector2 point, IntCoords coord, float radius=0)
	{


		float x_min = (coord.x * OBSTACLE_CELL_SIZE)+radius;
		float y_min = (coord.y * OBSTACLE_CELL_SIZE)+ radius;
		float x_max = (x_min + OBSTACLE_CELL_SIZE)-radius;
		float y_max = (y_min + OBSTACLE_CELL_SIZE)-radius;



		if (point.x < x_min) {
			if (point.y < y_min)
				return { x_min, y_min };
			else if (point.y <= y_max)
				return { x_min, point.y };
			else
				return { x_min, y_max };

		}
		else if (point.x <= x_max) {
			if (point.y < y_min)
				return { point.x, y_min };
			else if (point.y <= y_max)
				return { point.x, point.y };
			else
				return { point.x, y_max };
		}
		else {
			if (point.y < y_min)
				return { x_max, y_min };
			else if (point.y <= y_max)
				return { x_max, point.y };
			else
				return { x_max, y_max };
		}
	}

	float squareDistanceToPoint(Vector2 point, int x, int y) {
		float x_min = (float)(x*OBSTACLE_CELL_SIZE);
		float y_min = (float)(y*OBSTACLE_CELL_SIZE);
		float x_max = x_min + 0;
		float y_max = y_min + 0;


		if (point.x < x_min) {
			if (point.y < y_min)
				return pow(x_min - point.x, 2) + pow(y_min - point.y, 2);
			else if (point.y <= y_max)
				return pow(x_min - point.x, 2);
			else
				return pow(x_min - point.x, 2) + pow(y_max - point.y, 2);

		}
		else if (point.x <= x_max) {
			if (point.y < y_min)
				return y_min - point.y;
			else if (point.y <= y_max)
				return 0;
			else
				return point.y - y_max;
		}
		else {
			if (point.y < y_min)
				return pow(x_max - point.x, 2) + (y_min - point.y, 2);
			else if (point.y <= y_max)
				return pow(point.x - x_max, 2);
			else
				return pow(x_max - point.x, 2) + (y_max - point.y, 2);
		}
	}
	

	std::array<std::array<bool, OBSTACLE_MAP_HEIGHT>, OBSTACLE_MAP_WIDTH> map={};
	

};

