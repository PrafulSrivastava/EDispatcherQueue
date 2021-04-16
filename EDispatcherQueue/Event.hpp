#pragma once
#include <iostream>
#include "Logger.hpp"

class Event {
public :

	static std::shared_ptr<Logger> logger;
	Event() {
	}
	virtual void v_EventHandler() = 0;
};