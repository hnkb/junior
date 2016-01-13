
#include "junior.h"
#include "../win32/window_engine.h"
#include <algorithm>

using namespace junior;
using junior::_win32::window_engine;


window::window(const wchar_t* title) : _engine(new window_engine(this, title)), _last_color(0)
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

		_last_color = other._last_color;
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

		_last_color = other._last_color;
	}
	return *this;
}

window::~window()
{
	if (_engine) delete static_cast<window_engine*>(_engine);
}


window& window::begin_draw()
{
	if (_engine) static_cast<window_engine*>(_engine)->begin_draw();
	return *this;
}

window& window::end_draw()
{
	if (_engine) static_cast<window_engine*>(_engine)->end_draw();
	return *this;
}


window& window::draw_line(const int x1, const int y1, const int x2, const int y2)
{
	if (_engine) static_cast<window_engine*>(_engine)->draw_line((float)x1, (float)y1, (float)x2, (float)y2, _last_color, 3);
	return *this;
}

window& window::draw_line(const int x1, const int y1, const int x2, const int y2, const unsigned int color)
{
	_last_color = color;
	return draw_line(x1, y1, x2, y2);
}

window& window::draw_circle(const int x, const int y, const int radius)
{
	if (_engine) static_cast<window_engine*>(_engine)->draw_ellipse((float)x, (float)y, (float)radius, (float)radius, _last_color, 3);
	return *this;
}

window& window::fill_circle(const int x, const int y, const int radius)
{
	if (_engine) static_cast<window_engine*>(_engine)->fill_ellipse((float)x, (float)y, (float)radius, (float)radius, _last_color);
	return *this;
}

window& window::write(const wchar_t* text, const int x, const int y)
{
	if (_engine) static_cast<window_engine*>(_engine)->write(text, (float)x, (float)y, _last_color);
	return *this;
}

window& window::write(const wchar_t* text)
{
	if (_engine) static_cast<window_engine*>(_engine)->write(text, _last_color);
	return *this;
}
