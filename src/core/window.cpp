
#include "window.h"
#include "../win32/window_engine.h"
#include <algorithm>

using namespace junior;
using junior::_win32::window_engine;


window::window(const wchar_t* title) : _engine(new window_engine(this, title))
{
}

window::window(const window& other) : _engine(nullptr)
{
	*this = other;
}

window::window(window&& other) : _engine(nullptr)
{
	*this = std::move(other);
}

window& window::operator=(const window& other)
{
	if (this != &other)
	{
		if (_engine) delete static_cast<window_engine*>(_engine);
		_engine = new window_engine(this, static_cast<window_engine*>(other._engine)->get_title().c_str());
	}
	return *this;
}

window& window::operator=(window&& other)
{
	if (this != &other)
	{
		if (_engine) delete static_cast<window_engine*>(_engine);
		_engine = other._engine;
		other._engine = nullptr;
		if (_engine) static_cast<window_engine*>(_engine)->set_owner(this);
	}
	return *this;
}

window::~window()
{
	if (_engine) delete static_cast<window_engine*>(_engine);
}


void window::draw_line(const int x1, const int y1, const int x2, const int y2)
{
	if (_engine) static_cast<window_engine*>(_engine)->draw_line(x1, y1, x2, y2, 0xdd5544, 3);
}

void window::draw_circle(const int x, const int y, const int radius)
{
	if (_engine) static_cast<window_engine*>(_engine)->draw_ellipse(x, y, radius, radius, 0xdd5544, 3);
}

void window::write(const wchar_t* text, const int x, const int y)
{
	if (_engine) static_cast<window_engine*>(_engine)->write(text, x, y, 0xdd5544);
}

void window::write(const wchar_t* text)
{
	if (_engine) static_cast<window_engine*>(_engine)->write(text, 0xdd5544);
}
