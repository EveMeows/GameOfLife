#include "panel.h"

#include <format>
#include <raymath.h>


KeyBindPanel::KeyBindPanel(float x, float y, float width, float height, std::shared_ptr<game_data> data)
{
	this->m_size = Rectangle(x, y, width, height);

	this->m_opacity = 0;
	this->m_enabled = false;

	this->m_font = GetFontDefault();

	this->m_data = data;
	this->m_scroller = Rectangle(0, 0, 0, 0);
}

void KeyBindPanel::Update()
{
	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		if (IsKeyPressed(KEY_K))
		{
			this->m_enabled = !this->m_enabled;
		}
	}

	if (this->m_enabled)
	{
		std::string format = std::format("Advance speed: {:.3f} seconds", this->m_data->time_out);
		Vector2 dim = MeasureTextEx(this->m_font, format.c_str(), 20, 1);
		this->m_scroller = Rectangle(
			this->m_size.x + this->BASE_X, this->m_size.y + this->m_size.height - 30,
			dim.x + 30, dim.y
		);

		if (CheckCollisionPointRec(GetMousePosition(), this->m_scroller))
		{
			if (GetMouseWheelMove() != 0)
			{
				this->m_data->time_out = Clamp(this->m_data->time_out + (float)GetMouseWheelMove() * 0.005f, 0.005f, 1.5f);
			}
		}

		this->m_opacity += 5 * GetFrameTime();
		if (this->m_opacity >= 1)
		{
			this->m_opacity = 1;
			return;
		}
	}
	else
	{
		this->m_opacity -= 5 * GetFrameTime();
		if (this->m_opacity <= 0)
		{
			this->m_opacity = 0;
			return;
		}
	}
}

void KeyBindPanel::Draw() const
{
	DrawRectangleRec(this->m_size, Fade(BLACK, this->m_opacity));
	DrawRectangleLinesEx(this->m_size, 3, Fade(GRAY, this->m_opacity));

	DrawText("KeyBinds", this->m_size.width / 2 + 30, this->m_size.y + 5, 30, Fade(WHITE, this->m_opacity));

	for (Item item : this->m_items)
	{
		DrawText(
			item.name.c_str(),
			item.position.x + this->m_size.x,
			item.position.y + this->m_size.y,
			this->FONT_SIZE,
			Fade(WHITE, this->m_opacity)
		);
	}

	std::string format = std::format("Advance speed: {:.3f} seconds", this->m_data->time_out);
	DrawText(format.c_str(), this->m_scroller.x, this->m_scroller.y, 20, Fade(WHITE, this->m_opacity));
	// DrawRectangleLinesEx(this->m_scroller, 3, YELLOW);
}

void KeyBindPanel::push_string(std::string name)
{
	float x = this->BASE_X;
	float y = this->BASE_Y;

	if (!this->m_items.empty())
	{
		Item last = this->m_items.back();
		Vector2 dim = MeasureTextEx(this->m_font, last.name.c_str(), this->FONT_SIZE, 1);

		y = last.position.y + dim.y + this->SPACING;
	}

	this->m_items.push_back(Item(name, Vector2(x, y)));
}

bool KeyBindPanel::visible() const
{
	return this->m_enabled;
}

void KeyBindPanel::OnWindowResize(float x, float y, float width, float height)
{
	if (!IsWindowResized()) return;
	this->m_size = Rectangle(x, y, width, height);
}