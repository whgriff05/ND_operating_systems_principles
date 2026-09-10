# Scheduling: Multi-Level Feedback Queue (MLFQ)

## Multi-Level Feedback Queue

Like FIFO, it tries to complete the *shortest jobs first* \
Like Round Robin, it tries to be *fair* \
Unlike either, it will incorporate I/O and adjust priority levels over time

__Algorithm:__ 
1. Pick the highest priority
2. Within a level, do round robin on the jobs
3. New jobs start at highest priority
4. Lower priority once time allotment expires
5. After a period of time, all jobs move up in priority

### Example 1: Single Long Job

- Process starts at highest queue
- After time allotment, process moves down a queue
- Continue prior step until bottom queue is reached
- After a period of time, the process will be boosted back to the highest queue

### Example 2: Long and Short

- Assume long job is running in bottom queue
- Short job arrives, gains the CPU and runs in the highest queue
- After time allotment, short job moves down a queue and continues until the bottom
- Once long job and short job are both at the bottom, they round robin in scheduling

This allows short jobs to have fast __turnaround time__ and __response time__

### Example 3: I/O vs CPU

- Assume long job is running in the bottom queue
- Other job arrives in the highest queue, but performs I/O
- During this I/O blocking, the long job runs; when the other job is NOT performing I/O, it runs

Jobs that are mostly I/O will maintain a __higher priority__ since they do not use up their __time slice__ as quickly as __compute jobs__. \
This is good for __interactive jobs__ that require good __response time__.

### Problem and Priority Boost

__Problem:__ If a job is always in the lowest priority level, it will __starve__ because it has no opportunity to run

__Solution:__ Boost all processes to the highest priority level

### Accounting and Turnaround Time

To determine when a job should be moved down a queue, we keep track of how much of a time slice or
allotment the job has used

When a time allotment is used up, we deprioritize the job

We allocate more time for jobs in lower priority levels

This means __lower priority jobs run for longer time slices than higher priority jobs__

### Summary

MLFQ is an attempt to optimize turnaround time and response time \
It prioritizes __new, short, I/O heavy jobs__ over __old, long, compute-heavy jobs__ \
MLFQ devolves into Round-Robin \
MLFQ involves some tricks and magic numbers \
MLFQ *(or some variant)* has been used in real-world operating systems

