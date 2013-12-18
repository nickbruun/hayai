#ifndef __HAYAI_CHRONO
#define __HAYAI_CHRONO

#if defined(__APPLE__)
# include <mach/mach_time.h>
#else
#include <sys/time.h>
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
        clock_gettime(CLOCK_MONOTONIC_RAW, &result);
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
