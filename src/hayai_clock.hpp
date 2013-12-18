//
// System-specific implementation of the clock functions.
//
// On Apple (OS X, iOS), mach_absolute_time() is used. It gets
// CPU/bus dependent real-time clock with up to nanosecond precision.
//
// On Linux, clock_gettime() is used to access monotonic real-time clock
// with up to nanosecond precision. On kernels 2.6.28 and newer, the ticks
// are also raw and are not subject to NTP and/or adjtime(3) adjustments.
//
// Other POSIX compliant platforms resort to using gettimeofday(). It is
// subject to clock adjustments, does not allow for higher than microsecond
// resolution and is also declared obsolete by POSIX.1-2008.
//
// Note on C++11:
//
// Starting with C++11, we could use std::chrono. However, the details of
// what clock is really being used is implementation-specific. For example,
// Visual Studio 2012 defines "high_resolution_clock" as system clock with
// ~1 millisecond precision that is not acceptable for performance measurements.
// Therefore, we are much better off having full control of what mechanism we
// use to obtain the system clock.
//

#ifndef __HAYAI_CLOCK
#define __HAYAI_CLOCK

#if defined(__APPLE__)
# include <mach/mach_time.h>
#elif defined(__linux__)
# include <time.h>
#else
# include <sys/time.h>
#endif
#include <stdint.h>

namespace hayai
{

#if defined(__linux__)

struct Clock {
    typedef struct timespec TimePoint;

    static TimePoint now()
    {
        TimePoint result;
#ifdef CLOCK_MONOTONIC_RAW
        clock_gettime(CLOCK_MONOTONIC_RAW, &result);
#else
        clock_gettime(CLOCK_MONOTONIC, &result);
#endif
        return result;
    }

    static int64_t diff(const TimePoint& startTime, const TimePoint& endTime)
    {
        TimePoint time_diff;
        time_diff.tv_sec = endTime.tv_sec - startTime.tv_sec;
        if (endTime.tv_nsec < startTime.tv_nsec) {
            time_diff.tv_nsec = (
                endTime.tv_nsec + 1000000000L - startTime.tv_nsec
            );
            time_diff.tv_sec--;
        } else {
            time_diff.tv_nsec = endTime.tv_nsec - startTime.tv_nsec ;
        }
        return time_diff.tv_sec * 1000000 + time_diff.tv_nsec / 1000;
    }
};

#elif defined(__APPLE__)

struct Clock {
    typedef uint64_t TimePoint;

    static TimePoint now()
    {
        return mach_absolute_time();
    }

    static int64_t diff(TimePoint startTime, TimePoint endTime)
    {
        mach_timebase_info_data_t time_info;
        mach_timebase_info(&time_info);
        return (endTime - startTime) * time_info.numer / time_info.denom / 1000;
    }
};

#else

struct Clock {
    typedef struct timeval TimePoint;

    static TimePoint now()
    {
        TimePoint result;
        gettimeofday(&result, NULL);
        return result;
    }

    static int64_t diff(const TimePoint& startTime, const TimePoint& endTime)
    {
        return ((endTime.tv_sec - startTime.tv_sec) * 1000000 +
                (endTime.tv_usec - startTime.tv_usec));
    }
};

#endif

}

#endif
