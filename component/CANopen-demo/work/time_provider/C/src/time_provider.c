#include "time_provider.h"
#include <time.h>

void time_provider_startup(void) {}

void time_provider_PI_get_elapsed_time(asn1SccTime *OUT_time) {
  struct timespec time;
  if (clock_gettime(CLOCK_MONOTONIC, &time) == 0) {
    OUT_time->seconds = time.tv_sec;
    OUT_time->nanoseconds = time.tv_nsec;
  } else {
    OUT_time->seconds = 0;
    OUT_time->nanoseconds = 0;
    printf("[time_provider] ERROR: could not fetch current system time, "
           "rolling back to 0!\n");
  }
}
