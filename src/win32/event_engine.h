#pragma once

#include "junior.h"
#include "common-winsdk.h"


namespace junior
{
	namespace _win32
	{
		class event_engine
		{
		public:
			static mouse_event _mouse_event_from_msg(const MSG& msg);
			static keyboard_event _keyboard_event_from_msg(const MSG& msg);
			static event _event_from_msg(const MSG& msg);

			static event wait_for_event();
		};
	}
}
