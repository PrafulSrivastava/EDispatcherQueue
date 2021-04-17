#include "Button.hpp"

Button::Button(char key) {
	callback = std::bind(&Button::v_EventHandler, this);
	m_KeyPressed = key;
}

void Button::v_EventHandler() {
	logger->print(std::string(1, m_KeyPressed), __func__, __LINE__);
}