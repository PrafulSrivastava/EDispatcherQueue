#pragma once
#include <iostream>
#include <mutex>
#include <sstream>

class Logger {
private:
	std::mutex m_Mtx;
	static std::shared_ptr<Logger> m_LoggerInstance;
	Logger();
public:
	static std::shared_ptr<Logger> p_GetLoggerInstance();
	void print(const std::string& str, const std::string& func, const int& line);
};