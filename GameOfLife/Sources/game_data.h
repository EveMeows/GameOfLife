#pragma once

#include "./entities/cells.h"

struct game_data
{
	bool show_grid = true;

	bool paused = true;

	std::string generation;
	int generations = 0;

	float timer = 0;
	float time_out = 0.1f;

	std::shared_ptr<Cells> cells;
};