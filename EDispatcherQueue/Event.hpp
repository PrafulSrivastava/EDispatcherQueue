#pragma once
#include <iostream>
#include <functional>
#include "Logger.hpp"

class Event {
protected :
	static std::shared_ptr<Logger> logger;
public:
	std::function<void()> callback;
	virtual void v_EventHandler() = 0;
};