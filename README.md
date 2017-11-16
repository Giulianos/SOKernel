# OS 2<sup>nd</sup> Assignment: Kernel implementation

This report explains the features, algorithms, data structures and kernel functions.

## Simple Demo in video
<a href="http://www.youtube.com/watch?feature=player_embedded&v=v7IXh4SGed4" target="_blank"><img src="http://img.youtube.com/vi/v7IXh4SGed4/0.jpg"
alt="Demo Video" width="240" height="180" border="10" /></a>


## Apps
Below is a list of the available apps in the Operating System.

| Number | Name         |
| :----: | :----:       |
| 0      | idle         |
| 1      | init         |
| 2      | sh           |
| 3      | help         |
| 4      | ps           |
| 5      | producer     |
| 6      | consumer     |
| 7      | sample       |
| 8      | philosophers |
| 9      | chat         |

## Process and threads
In this section is explained the meaning of **thread** and **process** in our kernel, in other words, the scope each one covers and how are them used.

### Thread
A **thread** is an entity that represent executable code and the single one scheduled. It has information such as
- A tid (thread id)
- A state (indicates if it's ready or blocked)
- A stack
- An IP (stored in its stack)
- A set of registers (stored in its stack)
- The process which it belongs

### Process
A **process** is the entity that manages its threads resources. It has information such as
- A pid (process id)
- A ppid (parent process id)
- The pages that are part of the heap
- A non-empty thread list
- The tty which it's connected
- Flags

### Life cycle of a process
From Userland a process can be created using the syscall **fork** (as in UNIX), which clone the calling process, creating a new child with just one thread, the one that made the call to fork.
Then it's possible to use the syscall **execve** to replace the process' image loading the requested module.
Unlike the rest, the only processes that aren't created in this way, are the ones created by the kernel, *idle*, *init* and the shells (*sh*). Those later processes are created with the function *create_process* during initialization of the kernel.
Finally, to terminate a process, the syscall **exit** is used. It ends the calling process (and all its threads).

### Life cycle of a thread
A thread is always created when a process is spawned. Also, a thread can be created in Userland with the syscall **new_thread** or in the kernel with the function *create_thread*. If the later is used, we must specify the process which will contain the new thread, this is because a thread can't exist without a container process.
In the case that the syscall is used, the thread will be contained by the process that made the call. Then, to end a thread we must use the syscall **exit_thread**.

## Scheduler
In this section is explained the scheduling algorithm and the data structures involved in it.

### Algorithm
The scheduling algorithm used is **Round Robin** with a quantum equivalent to the interrupt interval of the timer tick. To implement this algorithm, a doubly-linked circular list (the ready queue) is used. This queue is "rotated" (with *rotate_thread_cqueue*) whenever the scheduling algorithm is executed.

### Blocked queue
Our scheduler has blocked queues in a manner that when a thread blocks, we take it from the ready queue and offer it to the indicated blocked queue.
To make this possible, every kernel module (e.g. tty), must ask the scheduler for a new blocked queue in initialization time.

#### Blockings
When a module has to block a thread, it indicates the kernel who has to be blocked and in which queue (its queue). Also, extra info can be included with every blocked thread which can be used when the thread is unblocked.

#### Unblockings
When a kernel module has to unblock a thread because a certain condition is fulfilled, it calls the scheduler and tells that it wants to unblock a thread from his blocked queue and provides a callback function which is executed with the extra info (explained before) in the thread address space.

## Message Queues
As IPC (InterProcess Comunication) we implemented **Message Queues**. This ones are identified with a 256 max. character string and support writing messages of variable length. To implement them we used a linked list where each node is a message queue with its identifier string.
To create a message queue the syscall **mq_open** must be used. It receives the identifier string and creates the queue if it doesn't already. Once the queue is created, messages can be sent with the syscall **mq_send** which receives the identifier string of the queue, the message (a string) and the length of it.
Then, messages can be polled using the syscall **mq_receive** which receives the queue (its string), a buffer where the message will be copied and the maximum buffer length. The call to mq_receive blocks the thread in the message queue if there is no message available and unblocks it when a message arrives.

## Mutex
Mutexes, as message queues, are identified with a string. They are created with the syscall **mx_create** y are locked and unlocked with the syscalls **mx_block** and **mx_unblock**. When a thread calls to block and the mutex was already locked by another thread, the later blocks until the other unlocks it.
Finally, we decided to implement it without using the instructions *xchg* and *lock* from Intel because our syscalls are atomic with respect to the user processes anyways. This means that if a thread makes a call to block, the block is executed entirely before another thread can execute a single instruction.

## Semaphoress
We decided to implement semaphores because we needed them to implement **prodcons**.
The implementation (despite being used for different things) is very similar to the one for mutex with the difference in the possible values range.

## Terminals
 To better show the multitasking features of our Kernel we decided to include a  **terminal multiplexer**. This allow us to have various terminals with one screen and keyboard.
 The module is simple, we have an ADT that represents a virtual terminal. It has information such as the input/output buffers, the blocked queues, etc.
Then it is the terminals mux that contains eight of this ADTs and allows to switch with  **F1-F8** between terminal. Being the first seven used by the processes for in/out and the last one (8) for the kernel log. This later shows scheduling information, initialization, etc.
With respect to reading and writing, this calls are handled by the mux. It decides (based in which terminal is connected to the process that wants to read/write) to which terminal should be write/read and if the screen must be refreshed (in case the terminal is the one active).

## Page allocator
To allocate physical pages we used a simple page allocator which algorithm we found in OSDev. It consists in that every time a page is reserved, it is given from an array of preallocated pages. In this way we avoid traversing all the array to know which pages are free every time a page must be allocated.

## Background / Foreground
When a command is executed and a '&' is included at the end of it, the shell doesn't give the forground to the process neither it waits for it. Because of that, if the process has to read from keyboard in this situation, we suspend it sending it to the queue of suspended. Would have been better to implement a shell command that allow the user to give foreground to suspended processes.
Es important to have in mind that the foreground is given to the main thread of a process.

## Memory protection
We couldn't implement memory protection because we had problems going back from ring 3 into ring 0. We had to set up a TSS correctly (wich wasn't really used) in order to go into kernel context. Another possible solution was to use the instructions *syscalls* and *sysret* to make the syscalls. But because of time constraints we couldn't research any further because we decided to prioritize the rest of the things.

## Extra notes
- In **philosophers** the quantity of philosophers can be increased with I and decreased with D. To exit Q is used.
- After executing sample, the call to PS can produce an exception. We believe it was important to include the program in spite of knowing it produces an error which we couldn't solve because of time constraints.
- We couldn't make a syscalls table, but they are available in the **syscalls** folder inside the Kernel.
