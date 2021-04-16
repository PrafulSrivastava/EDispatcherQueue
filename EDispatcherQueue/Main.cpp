#include "Button.hpp"
#include "ThreadSafeQueue.hpp"
#include <vector>
#include "Logger.hpp"

void getQueue(std::shared_ptr<ThreadSafeQueue>& queue) {
	auto logger = Logger::p_GetLoggerInstance();
	logger->print("Starting input thread!", __func__, __LINE__);
	queue = ThreadSafeQueue::get_QueueInstance();
	while (true) {
		logger->print("Waiting on input..", __func__, __LINE__);
		char key;
		std::cin >> key;
		queue->v_PushAsync(std::shared_ptr<Event>(new Button(key)));		
	}

}

int main() {
	auto logger = Logger::p_GetLoggerInstance();
	logger->print("Main!", __func__, __LINE__);
	std::shared_ptr<ThreadSafeQueue> queue = nullptr;
	std::thread executerThread(getQueue, std::ref(queue));
	if (executerThread.joinable()) {
		executerThread.join();
	}
}