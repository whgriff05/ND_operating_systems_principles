# Semaphores

A __semaphore__ is a synchronization primative that consists of an integer we can manipulate with two operations:
- __wait__: decrease value; if value becomes negative, make the thread wait
- __post__: increase value; wake up the next waiting thread
