#pragma once
#include <iostream>
#include "Logger.hpp"

class Event {
protected :
	static std::shared_ptr<Logger> logger;
	Event() {
	}
public:
	virtual void v_EventHandler() = 0;
};