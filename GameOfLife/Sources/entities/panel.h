#pragma once

#include <raylib.h>

#include "item.h"
#include "../game_data.h"

#include <vector>
#include <memory>

class KeyBindPanel
{
private:
	Font m_font;
	Rectangle m_size;
	Rectangle m_scroller;

	float m_opacity;
	bool m_enabled;

	std::vector<Item> m_items;

	const int FONT_SIZE = 20;
	const int SPACING = 5;

	const int BASE_X = 20;
	const int BASE_Y = 50;

	std::shared_ptr<game_data> m_data;
public:

	KeyBindPanel(float x, float y, float width, float height, std::shared_ptr<game_data> data);

	void OnWindowResize(float x, float y, float width, float height);
	
	void Update();
	void Draw() const;

	void push_string(std::string name);
	bool visible() const;
};