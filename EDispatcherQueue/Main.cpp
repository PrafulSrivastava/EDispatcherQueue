#include "Button.hpp"
#include "ThreadSafeQueue.hpp"
#include <vector>
#include "Logger.hpp"

void getQueue(std::shared_ptr<ThreadSafeQueue>& queue) {
	auto logger = Logger::p_GetLoggerInstance();
	logger->print("Starting input thread!");
	queue = ThreadSafeQueue::get_QueueInstance();
	while (true) {
		logger->print("Waiting on input..");
		char key;
		std::cin >> key;
		queue->v_WriteDataAsync(std::shared_ptr<Event>(new Button(key)));		
	}

}

int main() {
	auto logger = Logger::p_GetLoggerInstance();
	logger->print("Main!");
	std::shared_ptr<ThreadSafeQueue> queue = nullptr;
	std::thread executerThread(getQueue, std::ref(queue));
	if (executerThread.joinable()) {
		executerThread.join();
	}
}