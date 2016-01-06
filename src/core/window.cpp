
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
	if (!_engine) return;
	auto hdc = static_cast<window_engine*>(_engine)->get_dc();
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void window::draw_circle(const int x, const int y, const int radius)
{
	if (!_engine) return;
	auto hdc = static_cast<window_engine*>(_engine)->get_dc();
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

void window::write(const wchar_t* text, const int x, const int y)
{
	if (!_engine) return;
	auto hdc = static_cast<window_engine*>(_engine)->get_dc();
	TextOutW(hdc, x, y, text, lstrlenW(text));
}

void window::write(const wchar_t* text)
{
	write(text, 10, _cursor_y += 20);
}
