# Scheduling: FIFO and Round-Robin

What is the purpose of a __scheduler__? \ 
The scheduler determines which process runs next. \
The decision making process is called the __scheduling policy__ or __discipline__.

*Note: recall Process Life Cycle, Process States, and Context Switch from `Notes 4`*

## Overview: When does a scheduler execute?

1. Timer interrupt
2. Process terminates
3. System call

A scheduler needs to be __fast__ since it runs so often

## Scheduling Workload

To make scheudling policies, we consider our workload. We will start with the following (bad) assumptions and then remove them as necessary.
1. Each job runs for the same amount of time
2. All jobs arrive at the same time
3. Once started, each job runs to completion
4. All jobs only use the CPU (no I/O)
5. The runtime of each job is known

## Scheduling Metrics

__Turnaround Time__: time between a job arriving into the schedule queue and the job being completed \
`T_turnaround = T_finish - T_arrival`
- How soon does the job complete?
- *throughput/volume*

__Response Time__: time between a job arriving into the schedule queue and the job starting \
`T_response = T_start - T_arrival`
- How soon does the job start?
- *latency/delay*

## FIFO (First In, First Out)

__Algorithm:__ execute jobs in the order they arrive

### Example 1: Jobs A, B, and C arrive at time 0 and run for 5 seconds each
`T_turnaround_avg = (5 + 10 + 15) / 3 = 10 s/job` \
`T_response_avg = (0 + 5 + 10) / 3 = 5 s/job`

Notice: FIFO is not great at response

### Example 2: Job A arrives at time 0 running for 30 seconds, Jobs B and C arrive at time 0 but run for 5 seconds
`T_turnaround_avg = (30 + 35 + 40) / 3 = 35 s/job` \
`T_response_avg = (0 + 30 + 35) / 3 = ~22 s/job`

This is incredibly bad; this is an example of the __convoy effect__ where a large job hogs the processor

Our thought is to run the __shortest job first__, maybe that will help

### Example 3: Same job arrival timing, but our FIFO scheduler will select the shortest job first
`T_turnaround_avg = (5 + 10 + 40) / 3 = ~18 s/job` \
`T_response_avg = (0 + 5 + 10) / 3 = 5 s/job`

This is a huge improvement on our times, but if other shortest jobs keep getting scheduled, the large job will be \
constantly deprioritized: this is known as __process starvation__

### Summary

- FIFO is a straightforward algorithm
- Good turnaround time (generally), but poor response time
- It can suffer from the convoy effect

## Round-Robin

__Algorithm:__ each process is executed for a certain time-slice (time allotment) before another process is selected

This is a __fair__ policy because it evenly divides the processor among active processes

### Example 1: Jobs A, B, and C arrive at time 0 and run for 5 seconds each
`T_turnaround_avg = (13 + 14 + 15) / 3 = 14 s/job` \
`T_response_avg = (0 + 1 + 2) / 3 = 1 s/job`

Notice: the huge improvement in response time

### Summary

- Round-Robin is fairly straightforward
- Poor turnaround time, strong in response time
- Requires pre-emption (a timer interrupt)
- Overhead due to constant context switching
