#pragma once
#include "Event.hpp"
#include "Logger.hpp"
class Button : public Event {
private:
	char m_KeyPressed;
public:
	Button(char key);
	void v_EventHandler();
};