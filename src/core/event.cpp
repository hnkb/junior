
#include "junior.h"
#include "../win32/event_engine.h"

using namespace junior;


event junior::wait_for_event()
{
	return _win32::event_engine::wait_for_event();
}
