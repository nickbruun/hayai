#include <hayai.hpp>


#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

int msleep(unsigned val)
{
    Sleep(val);
	return 0;
}

#else
#include <stdio.h> 
#include <unistd.h>

int msleep(unsigned val)
{
    return usleep(val*1000);
}
#endif

// The delivery man need some sleep sometimes

// Sleep for 1 ms 10 times
BENCHMARK(SomeSleep, Sleep1ms, 5, 10)
{
    msleep(1);
}

// Sleep for 5 ms 10 times
BENCHMARK(SomeSleep, Sleep10ms, 5, 10)
{
    msleep(10);
}

// Sleep for 20 ms 10 times
BENCHMARK(SomeSleep, Sleep20ms, 5, 10)
{
    msleep(20);
}