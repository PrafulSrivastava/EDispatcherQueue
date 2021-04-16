#include "ThreadSafeQueue.hpp"

std::shared_ptr<ThreadSafeQueue> ThreadSafeQueue::m_QueueInstace = nullptr;
bool ThreadSafeQueue::m_Empty = true;
std::shared_ptr<Logger> ThreadSafeQueue::logger = Logger::p_GetLoggerInstance();

ThreadSafeQueue::ThreadSafeQueue() {
	
	logger->print("Starting Worker threads!");
	for (int i = 0; i < QUEUE_SIZE; i++) {
		workerthreads[i] = std::thread(&ThreadSafeQueue::p_PopAsync, this);
	}
}

std::shared_ptr<ThreadSafeQueue> ThreadSafeQueue::get_QueueInstance() {
	if (m_QueueInstace == nullptr) {
		m_QueueInstace = std::shared_ptr<ThreadSafeQueue>(new ThreadSafeQueue());
	}
	return m_QueueInstace;
}

void ThreadSafeQueue::push(std::shared_ptr<Event> p_Evt) {
	std::unique_lock<std::mutex> lock(m_Mtx);
	m_Q.push(p_Evt);
	logger->print("Pushed!");
}

std::shared_ptr<Event> ThreadSafeQueue::pop() {
	std::unique_lock<std::mutex> lock(m_Mtx);
	auto item = m_Q.front();
	m_QueueInstace->m_Q.pop();
	logger->print("Popped!");
	return item;
}

void ThreadSafeQueue::v_Push(std::shared_ptr<Event> p_Evt) {
	logger->print("Sync Write..");
	p_Evt->v_EventHandler();
}

void ThreadSafeQueue::v_PushAsync(std::shared_ptr<Event> p_Evt) {
	logger->print("Async Write..");
	m_QueueInstace->push(p_Evt);
	if (!m_Q.empty()) {
		m_Empty = false;
		m_Cv.notify_one();
	}
}

bool ThreadSafeQueue::b_IsEmpty() {
	if (m_Q.empty()) {
		m_Empty = true;
	}
	else {
		m_Empty = false;
	}
	return m_Empty;
}

void ThreadSafeQueue::p_PopAsync() {
	logger->print("Async Read..");
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (true) {
		m_Cv.wait(lock, [] {return !m_Empty; });
		auto item = m_Q.front();
		m_QueueInstace->m_Q.pop();
		b_IsEmpty();
		v_Push(item);
	}
}
ThreadSafeQueue::~ThreadSafeQueue() {
	logger->print("Getting rid of worker threads..");
	for (int i = 0; i < QUEUE_SIZE; i++) {
		if (workerthreads[i].joinable()) {
			workerthreads[i].join();
		}
	}
}