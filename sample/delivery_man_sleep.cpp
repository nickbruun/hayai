#include <hayai.hpp>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

inline void msleep(unsigned int duration)
{
    Sleep(duration);
}

#else
#include <unistd.h>

inline void msleep(unsigned int duration)
{
    usleep(duration * 1000);
}
#endif

// The delivery man need some sleep sometimes.
//
// Sleep for 1 ms 10 times.
BENCHMARK(SomeSleep, Sleep1ms, 5, 10)
{
    msleep(1);
}

// The delivery man need some sleep sometimes.
//
// Sleep for 5 ms 10 times.
BENCHMARK(SomeSleep, Sleep10ms, 5, 10)
{
    msleep(10);
}

// The delivery man need some sleep sometimes.
//
// Sleep for 20 ms 10 times.
BENCHMARK(SomeSleep, Sleep20ms, 5, 10)
{
    msleep(20);
}
