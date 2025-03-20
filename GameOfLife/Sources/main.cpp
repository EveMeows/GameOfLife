	#include <cmath>
#include <cstdio>

#include <memory>
#include <format>

#include <raylib.h>
#include <raymath.h>

#include "globals.h"
#include "entities/cells.h"
#include "entities/panel.h"

#include "game_data.h"

// Disable terminal in release mode only
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG


static void draw_grid(void)
{
	// Draw horizontal lines
	for (int i = 0; i <= MAP_HEIGHT; i++)
		DrawLine(0, CELL_SIZE * i, MAP_WIDTH_PX, CELL_SIZE * i, GRID_COLOUR);

	// Draw vertical lines
	for (int i = 0; i <= MAP_WIDTH; i++)
		DrawLine(CELL_SIZE * i, 0, CELL_SIZE * i, MAP_HEIGHT_PX, GRID_COLOUR);
}

static Camera2D make_camera(void)
{
	Camera2D game_camera = {};

	game_camera.target = Vector2((float)MAP_WIDTH_PX / 2, (float)MAP_HEIGHT_PX / 2);
	game_camera.offset = Vector2((float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2);
	game_camera.rotation = 0.0f;
	game_camera.zoom = 2.0f;

	return game_camera;
}

static void make_cells(std::shared_ptr<game_data> game)
{
	game->cells = std::make_shared<Cells>();

	game->cells->alive.clear();

	game->cells->alive.push_back(Vector2(49, 49));
	game->cells->alive.push_back(Vector2(50, 49));
	game->cells->alive.push_back(Vector2(50, 50));
	game->cells->alive.push_back(Vector2(48, 51));
	game->cells->alive.push_back(Vector2(49, 51));
	game->cells->alive.push_back(Vector2(50, 51));

	game->cells->populate();
}

static void handle_camera(Camera2D& game_camera)
{
	// Handle camera zoom
	if (GetMouseWheelMove() != 0)
	{
		game_camera.zoom += (float)GetMouseWheelMove() * 0.05f;

		if (game_camera.zoom > MAX_ZOOM) game_camera.zoom = MAX_ZOOM;
		else if (game_camera.zoom < MIN_ZOOM) game_camera.zoom = MIN_ZOOM;
	}

	// Camera dragging
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		game_camera.target = Vector2Subtract(game_camera.target, Vector2Scale(GetMouseDelta(), 1.0f / game_camera.zoom));
}

static void handle_keys(std::shared_ptr<game_data> game)
{
	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		if (IsKeyPressed(KEY_G))
		{
			game->show_grid = !game->show_grid;
		}

		if (IsKeyPressed(KEY_R))
		{
			game->generations = 0;
			game->generation = "Generations: 0";
			game->paused = true;

			make_cells(game);
		}
	}
}

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Game Of Life");
	SetWindowMinSize(600, 500);

	Camera2D game_camera = make_camera();

	Vector2 mouse_screen = GetScreenToWorld2D(GetMousePosition(), game_camera);
	Vector2 previous_mouse = mouse_screen;

	std::shared_ptr<game_data> data = std::make_shared<game_data>();

	// Default positions
	make_cells(data);

	Font font = GetFontDefault();
	data->generation = "Generations: 0";
	Vector2 generation_measurements = MeasureTextEx(font, data->generation.c_str(), 20, 1);

	Vector2 pause_measurements = MeasureTextEx(font, "PAUSED", 20, 1);
	Vector2 running_measurements = MeasureTextEx(font, "RUNNING", 20, 1);

	KeyBindPanel panel = KeyBindPanel(100, 100, GetScreenWidth() - 200, GetScreenHeight() - 200, data);
	panel.push_string("SPACE -> Toggle pause");
	panel.push_string("CTRL + G -> Toggle grid ");
	panel.push_string("CTRL + K -> Show keybinds.");
	panel.push_string("CTRL + R -> Reset cells.");
	panel.push_string("-------------------------");
	panel.push_string("Left Click -> Drag camera");
	panel.push_string("Right Click -> Place cell");
	panel.push_string("Scroll speed text to adjust.");

	while (!WindowShouldClose())
	{
		Vector2 mouse_screen = GetScreenToWorld2D(GetMousePosition(), game_camera);
		if (IsWindowResized())
		{
			panel.OnWindowResize(100, 100, GetScreenWidth() - 200, GetScreenHeight() - 200);

			// The camera will stay focused on the same point, even though the window is resized.
			game_camera.offset = Vector2((float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2);
		}

		if (IsKeyPressed(KEY_SPACE)) data->paused = !data->paused;

		handle_keys(data);
		if (!panel.visible()) handle_camera(game_camera);

		data->cells->handle(mouse_screen, previous_mouse);

		previous_mouse = mouse_screen;

		if (!data->paused)
		{
			data->timer += GetFrameTime();
			if (data->timer >= data->time_out)
			{
				data->timer = 0;

				data->generations++;
				data->cells->advance_generation();

				data->generation = std::format("Generations {}", data->generations);
				generation_measurements = MeasureTextEx(font, data->generation.c_str(), 20, 1);
			}
		}

		panel.Update();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			BeginMode2D(game_camera);
			{
				data->cells->draw_alive();

				if (data->show_grid) draw_grid();
				else DrawRectangleLines(0, 0, MAP_WIDTH_PX, MAP_HEIGHT_PX, GRID_COLOUR);
			}
			EndMode2D();

			if (data->paused)
				DrawText("PAUSED", GetScreenWidth() - pause_measurements.x - 10, 5, 20, RED);
			else
				DrawText("RUNNING", GetScreenWidth() - running_measurements.x - 10, 5, 20, GREEN);

			DrawText(data->cells->message.c_str(), 5, 5, 20, RAYWHITE);
			DrawText(data->generation.c_str(), GetScreenWidth() - generation_measurements.x - 18, 21, 20, RAYWHITE);
			DrawFPS(5, GetScreenHeight() - 20);

			panel.Draw();
		}
		EndDrawing();
	}

	// Close window
	CloseWindow();

	return EXIT_SUCCESS;
}