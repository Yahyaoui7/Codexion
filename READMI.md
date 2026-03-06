# Codexion

*This project has been created as part of the 42 curriculum by nyahyaou.*

---

# Description

Codexion is a concurrency simulation written in C using POSIX threads.
The goal of the project is to simulate multiple coders working in a shared environment where they must compete for limited resources (USB dongles) in order to compile their code.

Each coder runs in a separate thread and must periodically compile.
To compile, a coder must acquire two dongles (left and right). After compiling, the coder will debug and refactor before attempting to compile again.

The simulation stops when:

* A coder burns out (fails to compile before the deadline), or
* All coders complete the required number of compiles.

This project focuses on solving classical concurrency problems such as resource sharing, synchronization, deadlock prevention, and fair scheduling.

---

# How the Simulation Works

From first principles:

1. Each coder is a thread.
2. Each dongle is a shared resource protected by a mutex.
3. Coders must acquire **two dongles** before compiling.
4. After compilation, dongles enter a **cooldown period** before being reused.
5. A monitor thread constantly checks if a coder burned out.
6. Requests for dongles are scheduled using:

   * FIFO (First In First Out)
   * EDF (Earliest Deadline First)

---

# Compilation

```bash
make
```

The program is compiled with:

```
-Wall -Wextra -Werror -pthread
```

---

# Usage

```
./codexion number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

Example:

```
./codexion 5 2000 100 50 50 5 10 fifo
```

Arguments explanation:

| Argument                    | Description                                        |
| --------------------------- | -------------------------------------------------- |
| number_of_coders            | Number of coder threads and dongles                |
| time_to_burnout             | Maximum time without compiling before burnout      |
| time_to_compile             | Time spent compiling                               |
| time_to_debug               | Time spent debugging                               |
| time_to_refactor            | Time spent refactoring                             |
| number_of_compiles_required | Simulation stops when all coders reach this number |
| dongle_cooldown             | Time before a dongle becomes available again       |
| scheduler                   | Scheduling policy: fifo or edf                     |

---

# Example Output

```
1 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
101 1 is debugging
151 1 is refactoring
```

---

# Blocking Cases Handled

This project addresses several classical concurrency problems.

### Deadlock Prevention

Deadlock occurs when threads wait forever for resources.

To prevent this:

* Coders always acquire dongles in a **consistent order**.
* If a coder fails to obtain the second dongle, the first one is released.

This prevents circular wait (one of Coffman's conditions).

---

### Starvation Prevention

Starvation happens when one thread never gets resources.

To prevent starvation:

* A **priority queue (heap)** is used.
* Requests are served using:

  * FIFO → fair order
  * EDF → earliest deadline first

EDF prioritizes the coder closest to burnout.

---

### Dongle Cooldown

After releasing a dongle:

```
cooldown_until = now + dongle_cooldown
```

The dongle cannot be taken again until this time has passed.

Threads waiting for the dongle use condition variables.

---

### Burnout Detection

A **monitor thread** periodically checks:

```
current_time - last_compile_start > time_to_burnout
```

If true:

* The simulation stops
* "burned out" is printed within 10ms.

---

### Log Serialization

Multiple threads may print logs simultaneously.

To prevent mixed output:

```
pthread_mutex_lock(log_mutex)
printf(...)
pthread_mutex_unlock(log_mutex)
```

This guarantees that each message is printed atomically.

---

# Thread Synchronization Mechanisms

The following primitives are used:

### pthread_mutex_t

Used to protect shared data:

* dongle state
* logging
* coder data

Example:

```
pthread_mutex_lock(&dongle->mutex);
```

---

### pthread_cond_t

Used to allow threads to wait for a dongle.

Example:

```
pthread_cond_wait(&dongle->cond, &dongle->mutex);
```

Threads wake up when the dongle becomes available.

---

### Monitor Thread

A separate thread continuously checks burnout conditions and stops the simulation when necessary.

---

# Technical Choices

### Heap (Priority Queue)

The heap is used to implement scheduling:

* FIFO → priority based on arrival time
* EDF → priority based on deadline

This ensures fair arbitration between competing coders.

---

# Resources

Concurrency concepts:

* Operating Systems: Three Easy Pieces
* POSIX Threads Programming
* The Dining Philosophers Problem

Useful documentation:

* https://man7.org/linux/man-pages/man3/pthread_create.3.html
* https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html

---

# AI Usage

AI tools were used for:

* Understanding concurrency concepts
* Improving documentation clarity


