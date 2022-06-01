#pragma once
#include <list>
#include <unordered_map>

#include "Vector2.h"
#include "GraphicsUtility.h"


class Path
{

public:

	Path() : typeId(-1), oldPoint(Vector2()), color(sf::Color::Black)
	{
		
	}

	Path(int typeId, Vector2 startPoint, int handle) : typeId(typeId), oldPoint(startPoint), color(GraphicsUtility::createColorFromTypeId(typeId-1)), vertexArray(sf::VertexArray(sf::LineStrip)), handle(handle)
	{
		points.push_back(startPoint);
		vertexArray.append({ startPoint,color });
	}

	void addPoint(Vector2 point)
	{
		points.push_back(point);
		vertexArray.append({ point,color });

		totalLenght += point.Distance(oldPoint);
		oldPoint = point;
	}

	float getLenght()
	{
		return totalLenght;
	}


	int typeId;
	int handle;
	sf::Color color;
	sf::VertexArray vertexArray;
private:

	Vector2 oldPoint=Vector2(0,0);
	float totalLenght=0;
	std::list<Vector2> points;
	



	
};

class PathArchive
{
public:
	void startPath(int antId, int typeId, Vector2 point, int handle)
	{
		activePaths[antId] = Path(typeId, point, handle);
	}
	void stepPath(int antId, Vector2 point)
	{
		auto it = activePaths.find(antId);
		if(it!=activePaths.end())
		{
			it->second.addPoint(point);
		}
	}
	void endPath(int antId, Vector2 lastPoint)
	{
		stepPath(antId, lastPoint);
		auto it = activePaths.find(antId);
		if (it != activePaths.end())
		{
			archivedPaths[it->second.typeId].push_front(it->second);
			unreadPaths[it->second.typeId].push_front(it->second);
			activePaths.erase(it);
		}
	}


	std::unordered_map<int, std::list<Path>>* getAllPaths()
	{
		return &archivedPaths;
	}
	std::unordered_map<int, std::list<Path>> getUnreadPaths()
	{
		std::unordered_map<int, std::list<Path>> returnPaths = unreadPaths;
		unreadPaths.clear();
		return returnPaths;
	}
private:
	std::unordered_map<int, Path> activePaths; //Stored bu ant id

	std::unordered_map<int, std::list<Path>> archivedPaths; //Stored by typeId
	std::unordered_map<int, std::list<Path>> unreadPaths;

};

