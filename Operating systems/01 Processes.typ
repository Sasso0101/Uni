= Processes
A process is a program in execution.  Processes are identified by a *process identifier* (pid). It is composed of multiple parts:
- Text section: the program code
- Data section: contains global variables (initialized and uninitialized)
- Heap: memory dynamically allocated during runtime
- Stack: contains temporarily variables, such as function parameters, return addresses, local variables
#figure(
  image("Images/Process.png", width: 20%),
  caption: [Memory layout for a process]
)
A process during execution cycles through the following states:
- new: the process is created
- ready: The process is in a queue and is waiting to be assigned to a processor
- running: Instructions are being executed
- waiting: The process is in a queue and is waiting for some event to occur (ex. a memory transfer, an I/O)
- terminated: The process has finished execution
#figure(
  image("Images/Process FSM.png", width: 50%),
  caption: [Process state machine]
)
The information about the state of the process is stored in the RAM in the process control block (PCB). It contains the following information:
- Process state: running, waiting, etc.
- Program counter: location of next instruction
- CPU registers: contents of registers used by the process
- CPU scheduling information: priorities, scheduling queue pointers
- Memory-management information: memory allocated to the process
- Accounting/Debug information: CPU used, clock time elapsed since start, time limits
- I/O status information: I/O devices allocated to process, list of open files
In Linux the PCB for every process is stored as a file in the /proc folder: ```bash less /proc/<pid::self>/status ```.

When a process is stopped it saves its state in the PCB and if reloads it when it resumes executing. The time when the CPU stores the PCB of a process and loads the PCB of another process is called *context switch*. Context switches can be categorized in:
- voluntary c. s.: the process stops itself because needs to wait for a resource
- nonvoluntary c. s.: the processor decides to switch process
== Multithreading
A process can execute multiple instructions at once by using multiple threads. Each thread has its own program counter and uses different registers, therefore all this information has to be also stored in the PCB.
== Scheduling
The CPU has a process scheduler, which decides which process to execute. The scheduler stores the processes in various queues:
- Job queue: set of all processes in the system
- Ready queue: set of all processes residing in main memory, ready and waiting to execute
- Device queues: set of processes waiting for an I/O device
#figure(
  image("Images/Queues.png", width: 70%),
  caption: [Process queues]
)
== Process creation
A process can create other _child_ processes, which in turn can have other children. Therefore processes are arranged in a tree data structure. In Linux the process tree can be printed using ```bash pstree```.

The parent and children have different options for sharing resources:
- Parent and children share all resources
- Children share subset of parent's resources
- Parent and child share no resources
Moreover they have different options for execution:
- Parent and children execute concurrently
- Parent waits until children terminate
In a Linux system the root process that spawns all other processes is called _systemd_. 
In the UNIX processes are managed using the following system calls:
- ```bash fork()```: creates a new process
- ```bash exec()```: replaces the parent's memory with the children's one (machine code, data, heap, and stack)
- ```bash wait()```: called by parent to wait for the end of the child's execution
#figure(
  image("Images/Process children.png", width: 90%),
  caption: [Creation of children processes]
)
#figure(
  raw(read("Lectures/fork.c"), lang: "c", align: left, block: true),
  caption: [A process that spawns a child process and waits for its termination],
)
== Communication between processes
Processes can communicate using:
- shared memory: processes that wish to communicate create a shared area of memory, that is managed directly by the processes
- message passing
#figure(
  image("Images/Process communication.png", width: 60%),
  caption: [Models of communication between processes]
)
=== Shared memory
Processes can communicate using shared memory by creating a dedicated area in memory and writing and reading to it. To access shared memory it processes need to map it to memory using memory mapping. Shared memory can be accessed by their name.
==== Memory-mapped files
In modern operating systems files, shared memory objects and other resources that can be addressed by a file descriptor can be accessed by processes using a procedure called "memory mapping". When memory mapping is used, a byte-to-byte correlation with a part of memory and the resource is established. This means that the processor can access the file very quickly, as if it was stored in memory. In UNIX this feature is provided by the ```bash mmap()``` system call.
#figure(
  raw(read("Lectures/posix-producer.c"), lang: "c", align: left, block: true),
  caption: [A process that creates a shared memory area and writes to it],
)
#figure(
  raw(read("Lectures/posix-consumer.c"), lang: "c", align: left, block: true),
  caption: [A process that opens a shared memory area and reads from it],
)
== Message passing
Processes can communicate without using shared memory by using message passing. This can be physically implemented in the following ways:
- Shared memory (we already saw that)
- Hardware bus
- Network
We can distinguish the channel on a logical level in the following ways:
 Direct or indirect
- Synchronous or asynchronous
- Automatic or explicit buffering
=== Pipes
Pipes provide a way for to processes to communicate directly with each other. We can distinguish among two different types of pipes: ordinary pipes and named pipes. Pipes are accessed using the file descriptor.
==== Ordinary pipes
Ordinary pipes cannot be accessed from outside the process that created it. Typically, a parent process creates a pipe and uses it to communicate with a child process that it created. Ordinary pipes are unidirectional, meaning that the parent process can only write to it and the child process can only read from it. In Windows they are called ordinary pipes.
#figure(
  raw(read("Lectures/pipe.c"), lang: "c", align: left, block: true),
  caption: [A process that spawn a child and communicates to it using an ordinary pipe],
)
==== Named pipes
Named pipes can be accessed without a parent-child relationship. They are bidirectional and multiple processes can read and write to it. When no process holds a reference to the file descriptor the pipe is destroyed by the system.
#figure(
  raw(read("Lectures/named_pipe_write.c"), lang: "c", align: left, block: true),
  caption: [A process that creates a named pipe and writes the content from the standard input in the pipe],
)
#figure(
  raw(read("Lectures/named_pipe_write.c"), lang: "c", align: left, block: true),
  caption: [A process that opens a named pipe and prints the content in the pipe to the standard output],
)