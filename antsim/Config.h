#pragma once



#define RANDOM_SEED 5
#define ANT_CONTROLLER	1 //0:Ant, 1:Central
//MAP

#define NUM_ANTS 100
#define MAP_WIDTH 384 // 256 384 512 768
#define MAP_HEIGHT 256
#define MAP_CELLS MAP_WIDTH*MAP_HEIGHT


//=====================ANT=============================


#define FOLLOW_TRAIL_TOTAL_STRENGTH	10.0f

#define SENSOR_RADIUS_TINY 1
#define SENSOR_RADIUS_SMALL 2
#define SENSOR_RADIUS_MEDIUM 4
#define SENSOR_DISTANCE 12
#define SENSOR_RADIUS_LARGE 8
#define SENSOR_RADIUS_HUGE 16

#define JOB_SENSOR_RADIUS 15
#define JOB_INTERACT_RADIUS 8

#define OBSTRUCTION_SENSE_RADIUS	4





//BODY

#define COLLISION_GROUP_SIZE 8
#define COLLISION_GRID_WIDTH  (MAP_WIDTH/COLLISION_GROUP_SIZE)+1
#define COLLISION_GRID_HEIGHT (MAP_HEIGHT/COLLISION_GROUP_SIZE)+1


//RIGID BODY

#define RIGID_BODY_CELL_SIZE	4
#define RIGID_BODY_RADIUS		1
#define RIGID_BODY_MOVEMENT_ITERATIONS 4

//TASKS

#define TASK_SUCCESS_THRESHOLD 5
//#define TASK_

//====================JOBS====================

#define JOB_MINIMUM_DISTANCE				75

#define JOB_MAX_AMOUNT						100
#define PACKET_SIZE							1
#define EXPLORE_WEIGHT						100
#define WEIGHT_SIZE							1000
#define JOB_RECRUITMENT_STRENGHT_MULTIPLIER	500
#define JOB_TIMEOUT							300
#define JOB_EXPLORE_RECRUIT_LOCKOUT			150

//============================PATHS=========================

#define PATH_INTERVAL						10

//=======================PHEROMONES===================

#define MAX_NUMBER_OF_TASKS				16
#define DEFAULT_PHEROMONES_NUMBER		4
#define NUMBER_OF_PHEROMONE_PAIRS		MAX_NUMBER_OF_TASKS+DEFAULT_PHEROMONES_NUMBER
#define EXPLORED_PHEROMONE_ID			MAX_NUMBER_OF_TASKS+0
#define RECRUIT_PHEROMONE_ID			MAX_NUMBER_OF_TASKS+1
#define SUCCESS_PHEROMONE_ID			MAX_NUMBER_OF_TASKS+2
#define FAILURE_PHEROMONE_ID			MAX_NUMBER_OF_TASKS+3
//#define OUTER_CONTAINS_MAP_RESOLUTION	32

#define PHEROMONE_FLOOR					0.1
#define PHEROMONE_TRAIL_START_STRENGTH	20
#define PHEROMONE_DECAY					0.992f
#define PHEROMONE_DECAY_FAST			PHEROMONE_DECAY * PHEROMONE_DECAY
#define PHEROMONE_DECAY_VERY_FAST		PHEROMONE_DECAY * PHEROMONE_DECAY * PHEROMONE_DECAY
//==============OBSTACLES==========================

#define OBSTACLE_CELL_SIZE				16
#define OBSTACLE_CELL_SIZE_INVERSE		1.0f/OBSTACLE_CELL_SIZE
#define OBSTACLE_DETECTION_RANGE		4
#define OBSTACLE_MAP_WIDTH				MAP_WIDTH/OBSTACLE_CELL_SIZE
#define OBSTACLE_MAP_HEIGHT				MAP_HEIGHT/OBSTACLE_CELL_SIZE
#define OBSTACLE_SIGHT_CHECK_INTERVAL	1.0f / OBSTACLE_CELL_SIZE



//GRAPHIC
#define WINDOW_HEIGHT					900
