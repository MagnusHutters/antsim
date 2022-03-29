#pragma once


//MAP

#define NUM_ANTS 1500
#define MAP_WIDTH 256
#define MAP_HEIGHT 256
#define MAP_CELLS MAP_WIDTH*MAP_HEIGHT


//=====================ANT=============================

#define SENSOR_DISTANCE 8
#define SENSOR_RADIUS 4


//BODY

#define COLLISION_GROUP_SIZE 8
#define COLLISION_GRID_WIDTH  (MAP_WIDTH/COLLISION_GROUP_SIZE)+1
#define COLLISION_GRID_HEIGHT (MAP_HEIGHT/COLLISION_GROUP_SIZE)+1


//=======================PHEROMONES===================

#define MAX_NUMBER_OF_TASKS				16
#define DEFAULT_PHEROMONES_NUMBER		4
#define NUMBER_OF_PHEROMONE_PAIRS		MAX_NUMBER_OF_TASKS+DEFAULT_PHEROMONES_NUMBER
#define EXPLORED						MAX_NUMBER_OF_TASKS+0
#define PATH							MAX_NUMBER_OF_TASKS+1
//#define OUTER_CONTAINS_MAP_RESOLUTION	32

#define PHEROMONE_FLOOR					0.1
#define PHEROMONE_DECAY					0.995