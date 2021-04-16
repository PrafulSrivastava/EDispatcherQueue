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

void Logger::print(const std::string& str) {
	std::unique_lock<std::mutex> lock(m_Mtx);
	std::stringstream ss;
	ss << "[";
	ss << std::this_thread::get_id();
	ss << "]";
	ss << " : ";
	ss << "(";
	ss << __LINE__;
	ss << ")";
	ss << " : ";
	ss << "Function : ";
	ss << __func__ << "()";
	ss << " : ";
	ss << str;
	std::cout << ss.str() << std::endl;
}