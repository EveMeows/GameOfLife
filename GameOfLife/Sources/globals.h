#pragma once

#include <raylib.h>

// Map
#define CELL_SIZE 8

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#define MAP_WIDTH_PX (CELL_SIZE * MAP_WIDTH)
#define MAP_HEIGHT_PX (CELL_SIZE * MAP_HEIGHT)

#define GRID_COLOUR DARKGRAY

// Cell states
#define DEAD 0
#define ALIVE 1

// Camera
#define MAX_ZOOM 4.0f
#define MIN_ZOOM 1.0f