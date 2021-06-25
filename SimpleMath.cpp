#include "SimpleMath.h"

#include <cstdlib>
#include <cstdio>
#include <windows.h>


class TimerClass
{
public:
	LARGE_INTEGER _frequency;	
	LARGE_INTEGER _start;		

	TimerClass() 
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_start);
	}
};

double GetTime()
{
	static TimerClass time0; 

	LARGE_INTEGER ticks;
	LONGLONG time;

	QueryPerformanceCounter(&ticks);

	time = (ticks.QuadPart - time0._start.QuadPart);
	time *= 1000;
	time /= time0._frequency.QuadPart;

	return (double)((unsigned long)time);
}

float Randf()
{
	return float(rand())/float(RAND_MAX);
}
