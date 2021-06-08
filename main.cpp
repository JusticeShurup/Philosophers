#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <Poco/Semaphore.h>
using namespace Poco;
using namespace std::this_thread;
using namespace std::chrono;

std::mutex mtx;

class Philosopher {
public:
	Philosopher(Semaphore& _forks, std::string name) : _forks(_forks){
		this->name = name;
		forks = 0;
	}


	void takeFork() {
		if (forks <= 2) {
			mtx.lock();
			_forks.wait();
			_forks.wait();
			forks+= 2;
			mtx.unlock();
			std::cout << name << " take fork. Forks now: " << forks << std::endl;
		}
	}

	void eat() {
		takeFork();
		if (forks == 2) {
			std::cout << name << " is eating " << std::endl;
			sleep_for(seconds(1));
			std::cout << name << " just ate " << std::endl;
			returnForks();
		}

	}

	void returnForks() {
		std::cout << name << " returned forks " << std::endl;
		_forks.set(); 
		_forks.set();
		forks = 0;
	}

	void operator()() {
		while (true) {
			eat();
		}
	}

private:
	Semaphore& _forks; 
	std::string name;
	int forks;
};


int main() {
	std::vector<std::string> names{ "John", "Ivan", "AbdulJohn", "Challa", "Yaroslave" };
	std::vector<std::thread> philosophers;
	Semaphore forks(5);
	Philosopher john(forks, names[0]);
	Philosopher ivan(forks, names[1]);
	Philosopher abdulJohn(forks, names[2]);
	Philosopher challa(forks, names[3]);
	Philosopher yaroslave(forks, names[4]);

	philosophers.push_back(std::thread(std::ref(john)));
	philosophers.push_back(std::thread(std::ref(ivan)));
	philosophers.push_back(std::thread(std::ref(abdulJohn)));
	philosophers.push_back(std::thread(std::ref(challa)));
	philosophers.push_back(std::thread(std::ref(yaroslave)));
	for (int j = 0; j < 5; j++)
		philosophers[j].join();

	return 0;
}