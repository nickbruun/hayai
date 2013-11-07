#ifndef __HAYAI_TIMER
#define __HAYAI_TIMER

/*
Adapted code from David Robert Nadeau
http://nadeausoftware.com/articles/2012/03/c_c_tip_how_measure_cpu_time_benchmarking
*/

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unable to define high resolution timer for an unknown OS."
#endif


namespace Hayai
{
    // High resolution timer
    class HighResolutionTimer
    {
    public:
        /// Platform specific cpu time type
        #if defined(_WIN32)
        typedef LARGE_INTEGER CpuTime;
        #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        struct CpuTime
        {
            union Value {
                int64_t nsec;
                int64_t usec;
                clock_t systck;
                clock_t clockT;
            } value;

            enum Type {
                ERROR,
                NSEC,
                USEC,
                SYSTCK,
                CLOCKT
            } type;
        };
        #endif
        


        /**
         * Returns the amount of CPU time used by the current process,
         * in seconds, or -1.0 if an error occurred.
         */
        static CpuTime getCpuTime();

        /**
         * Get duration between two CPU time.
         */
        static int64_t getDuration(
            const CpuTime& startTime, const CpuTime& endTime);

    private:
        /**
         * Get overhead cpu time induced by calling timer functions.
         */
        static CpuTime getOverhead();

        #if defined(_WIN32)
        static LARGE_INTEGER getPerformanceFrequency();
        #endif

    };


    inline HighResolutionTimer::CpuTime HighResolutionTimer::getCpuTime()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        CpuTime result;
        QueryPerformanceCounter(&result);
        return result;
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        CpuTime result;
        #if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
        /* Prefer high-res POSIX timers, when available. */
        {
            struct timespec ts;
            if (clock_gettime(CLOCK_REALTIME, &ts ) != -1) {
                result.value.nsec = ts.tv_sec * 1e9 + ts.tv_nsec;
                result.type = CpuTime::NSEC;
                return result;
            }
        }
        #endif

        #if defined(RUSAGE_SELF)
        {
            struct rusage rusage;
            if ( getrusage( RUSAGE_SELF, &rusage ) != -1 ) {
                result.value.usec = 
                    rusage.ru_utime.tv_sec * 1e6 + rusage.ru_utime.tv_usec;
                result.type = CpuTime::USEC;
                return result;
            }
        }
        #endif

        #if defined(_SC_CLK_TCK)
        {
            struct tms tms;
            if ( times( &tms ) != (clock_t)-1 ) {
                result.value.systck = tms.tms_utime;
                result.type = CpuTime::SYSTCK;
                return result;
            }
        }
        #endif

        #if defined(CLOCKS_PER_SEC)
        {
            clock_t cl = clock( );
            if ( cl != (clock_t)-1 ) {
                result.value.clockT = cl;
                result.type = CpuTime::CLOCKT;
                return result;
            }
        }
        #endif
        result.type = CpuTime::ERROR;
        return result;      /* Failed. */
    #endif
    }



    inline int64_t HighResolutionTimer::getDuration(
        const CpuTime& startTime, const CpuTime& endTime)
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
		const static LARGE_INTEGER performanceFrequency = getPerformanceFrequency();
        const CpuTime overhead = getOverhead();
        LONGLONG duration = endTime.QuadPart - startTime.QuadPart - overhead.QuadPart;
        return static_cast<int64_t>((double)duration * 1e9
             / performanceFrequency.QuadPart);
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        const CpuTime overhead = getOverhead();
        if (startTime.type != endTime.type && 
            startTime.type != overhead.type)
        {
            return -1;
        }
        switch (startTime.type) {
            case CpuTime::NSEC: 
                return endTime.value.nsec - startTime.value.nsec 
                    - overhead.value.nsec;
            case CpuTime::USEC: 
                return (endTime.value.usec - startTime.value.usec 
                    - overhead.value.nsec) * 1e3;
            case CpuTime::SYSTCK: 
            {
                #if defined(_SC_CLK_TCK)
                const double ticks = (double)sysconf( _SC_CLK_TCK );
                const clock_t diffTime = 
                    endTime.value.systck - startTime.value.systck 
                    - overhead.value.systck;
                return static_cast<int64_t>((double)diffTime 
                    * 1e9 / ticks);
                #else
                return -1;
                #endif
            }
            case CpuTime::CLOCKT:
            {
                #if defined(CLOCKS_PER_SEC)
                const clock_t diffTime = 
                    endTime.value.clockT - startTime.value.clockT
                    - overhead.value.clockT;
                return static_cast<int64_t>((double)diffTime
                    * 1e9 / (double)CLOCKS_PER_SEC);
                #else
                return -1;
                #endif
            }
            case CpuTime::ERROR:
                return -1;
        }
        return -1;
    #endif
    }


    inline HighResolutionTimer::CpuTime HighResolutionTimer::getOverhead()
    {
        CpuTime time1 = getCpuTime();
        CpuTime time2 = getCpuTime();
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        time2.QuadPart -= time1.QuadPart;
        return time2;
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        if (time1.type != time2.type)
        {
            time2.type = CpuTime::ERROR;
        }
        switch (time1.type) {
            case CpuTime::NSEC: 
                time2.value.nsec -= time1.value.nsec;
                break;
            case CpuTime::USEC: 
                time2.value.usec -= time1.value.usec;
                break;
            case CpuTime::SYSTCK: 
                time2.value.systck -= time1.value.systck;
                break;
            case CpuTime::CLOCKT:
                time2.value.clockT -= time1.value.clockT;
                break;
            case CpuTime::ERROR:
                break;
        }
        return time2;
    #endif
    }


    #if defined(_WIN32)
    inline LARGE_INTEGER HighResolutionTimer::getPerformanceFrequency()
    {
        LARGE_INTEGER result;
        QueryPerformanceFrequency(&result);
        return result;
    }
    #endif

}



#endif // __HAYAI_TIMER

