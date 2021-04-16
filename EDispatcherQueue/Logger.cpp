#include "Logger.hpp"

std::shared_ptr<Logger> Logger::m_LoggerInstance = nullptr;

Logger::Logger() {

}

std::shared_ptr<Logger> Logger::p_GetLoggerInstance() {
	if (m_LoggerInstance == nullptr) {
		m_LoggerInstance = std::shared_ptr<Logger>(new Logger());
	}
	return m_LoggerInstance;
}

void Logger::print(const std::string& str, const std::string& func, const int& line) {
	std::unique_lock<std::mutex> lock(m_Mtx);
	std::stringstream ss;
	ss << "[";
	ss << std::this_thread::get_id();
	ss << "]";
	ss << " : ";
	ss << "(";
	ss << line;
	ss << ")";
	ss << " : ";
	ss << "Function : ";
	ss << func << "()";
	ss << " : ";
	ss << str;
	std::cout << ss.str() << std::endl;
}