
#include <time.h>

//
// These functions replace`clock_gettime`
// on macOS before 10.12
//
// `get_monotonic_time()` is `clock_gettime(CLOCK_MONOTONIC...)`
// `get_current_utc_time()` is `clock_gettime(CLOCK_REALTIME...)`
//

struct timespec get_monotonic_time();
struct timespec get_current_utc_time();
