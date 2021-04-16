#include "ThreadSafeQueue.hpp"

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(m_Mtx);
	auto item = m_Q.front();
	m_Q.pop();
	logger->print("Popped!", __func__, __LINE__);
	return item;
}
