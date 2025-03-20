#pragma once

#include <raylib.h>

#include "item.h"
#include <vector>

class KeyBindPanel
{
private:
	Font m_font;
	Rectangle m_size;

	float m_opacity;
	bool m_enabled;

	std::vector<Item> m_items;

	const int FONT_SIZE = 20;
	const int SPACING = 5;

	const int BASE_X = 20;
	const int BASE_Y = 50;
public:

	KeyBindPanel(float x, float y, float width, float height);

	void OnWindowResize(float x, float y, float width, float height);
	void Update();
	void Draw() const;

	void push_string(std::string name);
};