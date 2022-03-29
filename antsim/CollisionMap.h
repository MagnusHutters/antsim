#pragma once


#include "Config.h"

#include <unordered_map>
#include <array>
#include <list>


class Body;
class BodyDriver;

#include "Body.h"



class CollisionMap
{

	std::array< std::array<float, COLLISION_GRID_HEIGHT>, COLLISION_GRID_WIDTH> collisionGrid;






};

