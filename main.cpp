#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <Poco/Semaphore.h>
using namespace Poco;
using namespace std::this_thread;
using namespace std::chrono;
class Philosopher {
public:
	Philosopher(Semaphore& _forks, std::string name) : _forks(_forks){
		this->name = name;
		forks = 0;
	}

	void operator()() {
		while (true) {
			eat();
		}
	}

	void takeFork() {
		if (forks <= 2) {
			_forks.wait();
			std::cout << name << " take fork. Forks now: " << forks;
			forks++;
		}
	}

	void eat() {
		takeFork();
		if (forks == 2) {
			std::cout << name << " is eating" << std::endl;
			sleep_for(seconds(1));
			std::cout << name << " John just ate" << std::endl;
			returnForks();
		}

	}

	void returnForks() {
		std::cout << name << " returned forks" << std::endl;
		_forks.set(); 
		_forks.set();
		forks = 0;
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

	for (int i = 0; i < 5; i++) {
		Philosopher philosopher(forks, names[i]);
		philosophers.push_back(std::thread(std::ref(philosopher)));
	}

	for (int j = 0; j < 5; j++)
		philosophers[j].join();

	return 0;
}
