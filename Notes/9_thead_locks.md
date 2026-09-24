# Thread Locks

Why do race conditions come up? \
Each line of C code is rarely one instruction, and when they overlap, they can try to edit the same memory
at the same time, causing a disjoint of what the value of that memory truly is

We use a __lock__ or a __mutual exclusion (mutex)__ to guard a __critical section__, which is a region of
code that __accesses a shared resource__

```C
pthread_mutex_t lock;                   // Declare lock
pthread_mutex_init(&lock, NULL)         // Initialize lock

// Alternative declaration and initialization method
/* pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER */

pthread_mutex_lock(&lock);              // Acquire lock (lock down the memory to this thread)
access_resource();                      // Perform access of shared resource
pthread_mutex_unlock(&lock);            // Release lock (allow thread to access all memory again)
```

Take a look at this in `threads/prime_0x_x.c`

