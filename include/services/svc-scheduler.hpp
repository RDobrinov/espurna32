// -----------------------------------------------------------------------------
// SCHEDULER
// -----------------------------------------------------------------------------

#ifndef SCHEDULER_SUPPORT
#define SCHEDULER_SUPPORT           1               // Enable scheduler (2.45Kb)
#endif

#ifndef SCHEDULER_MAX_SCHEDULES
#define SCHEDULER_MAX_SCHEDULES     10              // Max schedules alowed
#endif

#ifndef SCHEDULER_RESTORE_LAST_SCHEDULE
#define SCHEDULER_RESTORE_LAST_SCHEDULE      0      // Restore the last schedule state on the device boot
#endif

#ifndef SCHEDULER_WEEKDAYS
#define SCHEDULER_WEEKDAYS          "1,2,3,4,5,6,7" // (Default - Run the schedules every day)
#endif

// -----------------------------------------------------------------------------
// SCHEDULER
// -----------------------------------------------------------------------------

#define SCHEDULER_TYPE_SWITCH       1
#define SCHEDULER_TYPE_DIM          2

void schSetup();