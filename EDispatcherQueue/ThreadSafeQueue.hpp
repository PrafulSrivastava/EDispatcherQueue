#pragma once
#include "Event.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

class ThreadSafeQueue {
private:
	ThreadSafeQueue();
	std::mutex m_Mtx;
	std::condition_variable m_Cv;
	std::queue<std::shared_ptr<Event>> m_Q;
	static std::shared_ptr<ThreadSafeQueue> m_QueueInstace;
	std::shared_ptr<Event> pop();
	static bool m_Empty;
	void push(std::shared_ptr<Event> p_Evt);
	std::thread workerthreads[QUEUE_SIZE];
	static std::shared_ptr<Logger> logger;
public:
	static std::shared_ptr<ThreadSafeQueue> get_QueueInstance();
	void v_WriteData(std::shared_ptr<Event> p_Evt);
	void v_WriteDataAsync(std::shared_ptr<Event> p_Evt);
	std::shared_ptr<Event> p_ReadData();
	void p_ReadDataAsync();
	bool b_IsEmpty();
	~ThreadSafeQueue();
};