
#if defined(__linux__) || defined(__APPLE__)

#include "PClock.h"

#if defined(__APPLE__)
    #include <mach/clock.h>
    #include <mach/mach.h>
#endif

struct timespec get_monotonic_time ()
{
    struct timespec timestamp;
#if defined(__APPLE__)
    clock_serv_t clock_server;
    mach_timespec_t mach_timestamp;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock_server);
    clock_get_time(clock_server, &mach_timestamp);

    mach_port_deallocate(mach_task_self(), clock_server);

    timestamp.tv_sec = mach_timestamp.tv_sec;
    timestamp.tv_nsec = mach_timestamp.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
#endif
    return timestamp;
}

struct timespec get_current_utc_time ()
{
    struct timespec timestamp;
#if defined(__APPLE__)
    clock_serv_t clock_server;
    mach_timespec_t mach_timestamp;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &clock_server);
    clock_get_time(clock_server, &mach_timestamp);

    mach_port_deallocate(mach_task_self(), clock_server);

    timestamp.tv_sec = mach_timestamp.tv_sec;
    timestamp.tv_nsec = mach_timestamp.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &timestamp);
#endif
    return timestamp;
}

#endif // defined(__linux__) || defined(__APPLE__)
