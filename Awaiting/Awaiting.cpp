// Awaiting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <iostream>
using namespace std::chrono_literals;

#include <ppltasks.h>

#include <future>
#include <experimental\resumable>
using namespace std::experimental;


class slowcounter
{
public:
	slowcounter() :counter(0) {}
	int counter;
	void count() {
		for (int i = 0; i < 5; i++) 
		{ 
			std::this_thread::sleep_for(1s); 
			++counter; 
			std::cout << counter << std::endl;
		} 
	}
};

class futurecounter
{
public:
	futurecounter() :counter(0) {}
	int counter;
	std::future<void> count() {	return std::async([this] 
	{
		for (int i = 0; i < 5; i++)
		{
			std::this_thread::sleep_for(1s);
			++counter;
			std::cout << counter << std::endl;
		}
	});	}
};


void countwrapperget(futurecounter& f)
{
	f.count().get();
	std::cout << "finished get" << std::endl;
}

std::future<void> countwrapperawait(futurecounter& f)
{
	__await f.count();
	std::cout << "finished await" << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{

	slowcounter slow;
	slow.count();
	std::cout << "did it the slow way" << std::endl;

	concurrency::create_task([&]() {slow.count(); }).then([]() {std::cout << "did it in a task" << std::endl; }).wait();

	futurecounter f1;
	auto result = countwrapperawait(f1);
	std::cout << "this is after starting countwrapperawait" << std::endl;
	//result.wait();

	futurecounter f2;
	countwrapperget(f2);
	std::cout << "this is after starting countwrapperget" << std::endl;

	return 0;
}

