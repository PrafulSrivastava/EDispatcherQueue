#include "Button.hpp"

Button::Button(char key) {
	m_KeyPressed = key;
}

void Button::v_EventHandler() {
	logger->print(std::string(1, m_KeyPressed));
}