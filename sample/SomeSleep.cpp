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

#include <iostream>

BENCHMARK(SomeSleep, Sleep1ms, 5, 10)
{
    msleep(1);
}

BENCHMARK(SomeSleep, Sleep10ms, 5, 10)
{
    msleep(10);
}

BENCHMARK(SomeSleep, Sleep20ms, 5, 10)
{
    msleep(20);
}