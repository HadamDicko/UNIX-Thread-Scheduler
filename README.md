# Operating Systems - Thread Scheduler
## Fall 2024

### Overview
This project implements a thread scheduler using UNIX user-mode threads (pthread) and timer interrupts. The program creates and manages five child threads, controlling their execution through an interrupt handler based on a predefined schedule vector.

### Features
- Implements user-mode thread scheduling using pthread library
- Uses timer interrupts to manage thread execution
- Controls thread execution using a configurable schedule vector
- Provides synchronized thread execution without spin-waiting
- Implements proper thread synchronization using mutex and condition variables

### Requirements
- UNIX-based operating system (tested on Fedora)
- GCC compiler
- pthread library

### File Structure
```
.
├── main.c           # Main source code file
└── README.md        # This documentation file
```

### Compilation
To compile the program:
```bash
cc -o thread_scheduler main.c -pthread
```

### Execution
To run the program:
```bash
./thread_scheduler
```

### Implementation Details

#### Key Components
1. **Schedule Vector**
   - Global array determining thread execution order
   - Can be modified for different scheduling patterns
   - Default order: 0, 1, 2, 3, 4

2. **Thread Synchronization**
   - Uses pthread mutex for mutual exclusion
   - Uses condition variables for thread signaling
   - Avoids spin-waiting in all threads

3. **Interrupt Handler**
   - Activates every SCHEDULE_INTERVAL seconds
   - Controls thread scheduling based on schedule vector
   - Provides proper thread switching

#### Configurable Parameters
- `SCHEDULE_INTERVAL`: Time between thread switches (in seconds)
- `SLOWDOWN_FACTOR`: Controls frequency of thread running messages
- `schedule_vector`: Determines thread execution order

### Expected Output
The program will display:
1. Initial thread creation messages
2. Periodic interrupt handler messages
3. Thread execution messages

Example output:
```
Child thread 0 started ...
Child thread 1 started ...
Child thread 2 started ...
Child thread 3 started ...
Child thread 4 started ...
I woke up on the timer interrupt (0)
Thread: 0 is running ...
[...]
```
![Screenshot 2024-12-01 at 3 05 06 PM](https://github.com/user-attachments/assets/d457d7c8-9a13-464e-8803-9ef89c1add49)

### Testing Notes
- The program can be tested with different schedule vectors
- SLOWDOWN_FACTOR can be adjusted to change output frequency
- Thread execution order follows the schedule vector precisely

### Known Limitations
- Fixed number of threads (5)
- User-mode scheduling limitations
- Requires UNIX-based system

### Author
Hadam Dicko 
hadramedko@msn.com
