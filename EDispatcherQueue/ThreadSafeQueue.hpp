#pragma once
#include "Event.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

template <typename T>
class ThreadSafeQueue{
private:
	ThreadSafeQueue();
	std::recursive_mutex m_Mtx;
	std::condition_variable_any m_Cv;
	std::queue<std::shared_ptr<T>> m_Q;
	std::shared_ptr<T> pop();
	void push(std::shared_ptr<T> p_Evt);
	std::thread workerthreads[QUEUE_SIZE];
	static std::shared_ptr<ThreadSafeQueue<T>> m_QueueInstace;
	static bool m_Empty;
	static std::shared_ptr<Logger> logger;
public:
	static std::shared_ptr<ThreadSafeQueue<T>> get_QueueInstance();
	void v_Pop(std::shared_ptr<T> p_Evt);
	void v_PushAsync(std::shared_ptr<T> p_Evt);
	void p_PopAsync();
	void v_HandleCallback(std::shared_ptr<T> p_Evt);
	bool b_IsEmpty();
	~ThreadSafeQueue();
};

template <typename T>
std::shared_ptr<ThreadSafeQueue<T>> ThreadSafeQueue<T>::m_QueueInstace = nullptr;
template <typename T>
bool ThreadSafeQueue<T>::m_Empty = true;
template <typename T>
std::shared_ptr<Logger> ThreadSafeQueue<T>::logger = Logger::p_GetLoggerInstance();

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() {

	logger->print("Starting Worker threads!", __func__, __LINE__);
	for (int i = 0; i < QUEUE_SIZE; i++) {
		workerthreads[i] = std::thread(&ThreadSafeQueue<T>::p_PopAsync, this);
	}
}

template <typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue<T>() {
	logger->print("Getting rid of worker threads..", __func__, __LINE__);
	for (int i = 0; i < QUEUE_SIZE; i++) {
		if (workerthreads[i].joinable()) {
			workerthreads[i].join();
		}
	}
}

template <typename T>
std::shared_ptr<ThreadSafeQueue<T>> ThreadSafeQueue<T>::get_QueueInstance() {
	if (m_QueueInstace == nullptr) {
		m_QueueInstace = std::shared_ptr<ThreadSafeQueue<T>>(new ThreadSafeQueue<T>());
	}
	return m_QueueInstace;
}

template <typename T>
void ThreadSafeQueue<T>::push(std::shared_ptr<T> p_Evt) {
	std::unique_lock<std::recursive_mutex> lock(m_Mtx);
	m_Q.push(p_Evt);
	logger->print("Pushed!", __func__, __LINE__);
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop() {
	std::unique_lock<std::recursive_mutex> lock(m_Mtx);
	auto item = m_Q.front();
	m_Q.pop();
	logger->print("Popped!", __func__, __LINE__);
	b_IsEmpty();
	return item;
}


template <typename T>
bool ThreadSafeQueue<T>::b_IsEmpty() {
	if (m_Q.empty()) {
		m_Empty = true;
	}
	else {
		m_Empty = false;
		m_Cv.notify_all();
	}
	return m_Empty;
}

template <typename T>
void ThreadSafeQueue<T>::p_PopAsync() {
	logger->print("Async Read..", __func__, __LINE__);
	std::unique_lock<std::recursive_mutex> lock(m_Mtx);
	while (true) {
		m_Cv.wait(lock, [] {return !m_Empty; });
		logger->print("Wait Complete..", __func__, __LINE__);
		v_Pop(pop());
	}
}

template <typename T>
void ThreadSafeQueue<T>::v_Pop(std::shared_ptr<T> p_Evt) {
	logger->print("Sync Write..", __func__, __LINE__);
	v_HandleCallback(p_Evt);
}

template <typename T>
void ThreadSafeQueue<T>::v_PushAsync(std::shared_ptr<T> p_Evt) {
	logger->print("Async Write..", __func__, __LINE__);
	push(p_Evt);
	b_IsEmpty();
}

template <typename T>
void ThreadSafeQueue<T>::v_HandleCallback(std::shared_ptr<T> p_Evt) {
	p_Evt->callback();
}