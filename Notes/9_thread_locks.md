# Thread Locks

Why do race conditions come up? \
Each line of C code is rarely one instruction, and when they overlap, they can try to edit the same memory
at the same time, causing a disjoint of what the value of that memory truly is

We use a __lock__ or a __mutual exclusion (mutex)__ to guard a __critical section__, which is a region of
code that __accesses a shared resource__

```C
// One declaration and initialization method
pthread_mutex_t lock;                   // Declare lock
pthread_mutex_init(&lock, NULL)         // Initialize lock

// Alternative declaration and initialization method
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER

pthread_mutex_lock(&lock);              // Acquire lock (lock down the memory to this thread)
access_resource();                      // Perform access of shared resource
pthread_mutex_unlock(&lock);            // Release lock (allow thread to access all memory again)
```

Take a look at this in `threads/prime_0x_x.c`

## Evaluation of Locks

Metrics:
1. Correctness (Does it actually provide mutual exclusion?)
2. Fairness (Does it give each thread a fair shot at acquiring the lock?)
3. Performance (How much overhead is added by using the lock?)

## Implementations

### Disabling Interrupts

```py
class Mutex:
    def lock(self):
        DisableInterrupts()

    def unlock(self):
        EnableInterrupts()
```

This works because no interrupt can cause the thread to stop or the lock to be removed.

__Problems:__
- (Correctness) Can possibly __lose__ interrupts
- (Fairness) Need to trust threads to not hog processing
- (Performance) Doesn't scale to multiple processors (disabling interrupts for the __whole__ system)

### Spin Lock

```py
class Mutex:
    # 0: available; 1: unavailable
    flag: int = 0

    def lock(self):
        # Keep checking availability
        while self.flag == 1: pass
        self.flag = 1

    def unlock(self):
        # Clear the flag
        self.flag = 0
```

__Problems:__
- (Correctness) Race condition possible (multiple things seeing if lock is available or not; multiple things trying to lock if available)
- (Performance) Busywaiting

### Test and Set

To effectively implement a lock (without a race condition in itself), we need special hardware instructions that provide atomic exchanges

Test-and-set provides an __atomic__ way to change a value (with no way of interrupting)

```py
# Note: at the hardware level, this all happens in one instruction atomically
def TestAndSet(old_ptr: int*, new_value: int*):
    old_value = *old_ptr
    *old_ptr = new_value
    return old_value
```

```py
class Mutex:
    flag: int = 0

    def lock(self):
        while TestAndSet(self.flag, 1) == 1:
            pass

    def unlock(self):
        self.flag = 0
```

__Problems:__
- (Performance) Busywaiting

### Yielding

Instead of holding the CPU busywaiting, we can give up (yield) the CPU as we spin

```py
class Mutex:
    flag: int = 0

    def lock(self):
        while TestAndSet(self.flag, 1) == 1:
            yield_cpu() # Give up CPU to scheduler

    def unlock(self):
        self.flag = 0
```

__Problems:__
- (Fairness) Starvation
- (Performance) Lots of context switches
