= Introduction
An operating system is a program that acts as an intermediary between the user and the hardware.
The main goals of an operating system are to:
- execute programs that solve problems
- make the computer easy to use
- use the hardware in an efficient manner
#figure(
  image("Images/Overview.png", width: 50%)
)
To do so it coordinates access to the hardware among the different applications and users. 

The program that runs all the time is the *kernel*, everything else is either a system program (ships with the operating system) or an application program.
== Computer startup
The first program that runs on startup is the *bootstrap program*. This is stored in ROM or EEPROM and is usually called *firmware*. It initializes registers, memories and device controllers and loads the kernel into the main memory. The kernel starts *daemons*, i.e. different programs from kernels but that must be started at startup. For example in UNIX there is _systemd_, that in turn starts other daemons.
== General computer architecture
The general model of a computer architecture is the following: there are one or more CPUs and device controllers that are connected through a common bus with a shared memory.
#figure(
  image("Images/Computer architecture.png", width: 70%)
)
=== Interrupts
In interrupt driven operating systems, device controllers communicate with the CPU using *interrupts*. While waiting for the CPU device controllers store information in local buffers, that are then read/written by the CPU and the stored data is transferred from/to the main memory.
#figure(
  image("Images/Interrupts.png", width: 70%),
  caption: [Interrupt timing diagram]
)
When an interrupt happens, the OS saves the current program counter (PC), jumps to the routine that handles the interrupt and then resumes normal execution by jumping back to where the PC was pointing to. The position to where to jump to when an interrupt is received is stored in the *interrupt vector or table*, which is a map where each interrupt is linked to an instruction memory address.

A *trap* or *exception* is a software-generated interrupt caused by an error or a user request.

Depending on the importance of the interrupt, some interrupts must be handled immediately, while others can wait. The first ones are called *non-maskable*, while the latter are *maskable*.

While transferring data, the CPU receives an interrupt when every chunk of data has been successfully received. This can be very very wasteful, therefore a feature called DMA (Direct Memory Access) has been introduced. In this technique the CPU receives an interrupt only after all the data has been transferred successfully.

=== Storage
Storage systems are categorized by speed, cost and volatility. Each storage systems has therefore its advantages and disadvantages, therefore there is no "best" storage device.

The main memory of a computer, which can be accessed directly by the CPU is DRAM (dynamic random access memory, based on charged capacitors) or SRAM (static random access memory, based on inverters), which is usually volatile. Secondary storage has much bigger capacity and is non-volatile (ex. hard disks, solid-state drives).

Each storage system has a device controller and a device driver. The driver provides an uniform interface between the controller and the kernel.

==== Caching
Caching is a very common technique for speeding up access to commonly used data. Information is copied from slower to faster storage temporarily and then every time that information is needed the OS will first check if it is present in cache. Due to cost constraints, cache is very often smaller than the storage being cached, therefore cache management must be properly optimized.

== Modern system architectures
#figure(
  image("Images/Von Neumann.png", width: 60%),
  caption: [A Von-Neumann architecture]
)
Currently most systems are multiprocessor and/or multi-core. In these systems, we can allocate tasks to processors in different ways:
- asymmetric processing: each processor/core is assigned a specific task
- symmetric processing: each processor/core performs all tasks
=== Difference between multiprocessor and multi-core
Multiprocessor systems have multiple processors with a single CPU and share the same system bus and sometimes the clock. Multi-core systems have a single processor that contains multiple CPUs.
#figure(
  image("Images/Multicore.png", width: 50%),
  caption: [A multi-core processor]
)
Multi-core systems are more widespread because they usually consume less power than multiprocessor systems and because on-chip buses are faster.
=== Clustered systems
Clustered systems are systems composed of multiple machines that usually share the same storage via a storage-area network (SAN). These systems provide a high-availability service that can survive failures of single machines.
- Symmetric clustering: all machines can run tasks and they monitor each other. If a machine fails the other can take over.
- Asymmetric clustering: each machine is assigned to a specific set of tasks. If a machine fails another machine that was turned on and in "hot-standby mode" takes over.
=== Multi-programmed systems
The OS can run multiple tasks on the same CPU by using a technique called multiprogramming (batch system): the OS organizes jobs so that the CPU has always one ready to execute. When a job has to wait (for example for I/O) the OS switches to another job. This is called job scheduling. Timesharing (multitasking) is an extension of this technique where the OS switches so frequently among different tasks that the user doesn't notice and can interact with all applications at the same time. This is needed for "window" based systems, where the user can see multiple things are the same time.
#figure(
  image("Images/Memory layout.png", width: 30%),
  caption: [Memory layout for multiprogramming systems]
)
The OS and users share the same hardware, devices and software resources. To protect the system and avoid that different jobs can write in some areas of the memory a privilege system is established. In dual-mode systems jobs can be run in user mode or kernel mode. Some instructions are allowed only for kernel mode systems. Intel processors have for modes of operation, where 0 is fully privileged and 3 is fully restricted.
=== Process management
A process is a program in execution. The _program_ is a passive entity, while the _process_ is an active entity. The life of the process is generally managed by the operating system. Single-threaded processes have a *program counter* specifying the location of the next instruction to execute. Instructions are executed sequentially, until the end fo the program is reached. Multi-threaded process has one program counter per thread.
If a system has more cores, each core has its own program counter.
=== Memory management
To execute a program, the instructions must be in memory. Memory management is handled by the operating system and has the following goals:
- Keeping track of which parts of memory are currently being used and by whom
- Deciding which processes (or parts thereof) and data to move into and out of memory
- Allocating and deallocating memory space as needed
=== Storage management
The OS provides a logical view of the storage and abstracts the physical properties in *files*. Files are organized in directories and there usually is an access control system. The OS deals with:
- Free-space management
- Storage allocation
- Disk scheduling

The memory is therefore organized in a hierarchy, where each level offers different access speeds. While transferring data from a level to another, the OS must ensure that the data stays consistent. Moreover, multiprocessor environment must provide cache coherency in hardware such that all CPUs have the most recent value in their cache.
=== I/O management
The OS hides the peculiarities of hardware devices from the user using I/O subsystems. These subsystems are responsible for the device-driver interfaces and memory management of I/O including buffering (storing data temporarily while it is being transferred), caching (storing parts of data in faster storage for performance), spooling (the overlapping of output of one job with input of other jobs).
== OS protection
OS must provide mechanisms to defend the system against external attacks. An attack is anything posing a threat to:
- Confidentiality
- Availability
- Integrity
For example OS distinguish among users, where each has a specific set of privileges. Privilege escalation is an attack where a user can gain privileges of a more privileged user.
== Computing environments
There exist many computing environments, such as:
- Stand-alone general purpose machines
  - Network computers (thin clients)
  - Mobile computers
- Real-time embedded systems: operating system that runs processes with very important time constraints
- Cloud computing
  - Client-server computing
  - Peer-to-peer computing
- Distributed computing: many systems connected together over a network
- Virtualization: guest OS emulates another OS or hardware and runs software on it. The program that manages this is called VMM (Virtual machine manager).
#figure(
  image("Images/Virtualization.png", width: 50%),
  caption: [Virtualization]
)
== Services provided by operating systems
Operating systems provide the following services:
- User interface: can be command-line (CLI), Graphics User Interface (GUI), Batch
- Program execution - The system must be able to load a program into memory and to run that program
- I/O operations
- File-system manipulation
- Communication between processes
- Error detection: errors may occur in CPU and memory hardware, in I/O devices, in user program
- Resource allocation: when multiple users or multiple jobs running concurrently, resources must be allocated to each of them
- Accounting: to keep track of which users use how much and what kinds of computer resources
- Protection and security
#figure(
  image("Images/Services.png", width: 80%),
  caption: [Services provided by an operating system]
)
== System calls
System calls are an interface provided by the operating system to interact with it. They are mostly accessed by using a high-level API provided by a language such as C, C++ etc. In this way developers can use a single API that works on all operating systems and leave the actual system call to the underlying library written for that specific platform. The high-level API can also check for errors before calling the system call.
#figure(
  image("Images/C example.png", width: 40%),
  caption: [C program invoking printf() library call, which calls write() system call]
)
=== Parameter passing
A system call usually requires some parameters, for ex. the _open_file_ system call needs to know the name of the file. Parameters can be passed using predefined specific registers. Often there are not enough registers for all required parameters, so parameters can be also stored in memory in a table and just the address of the table is put into the register.
#figure(
  image("Images/Parameter passing.png", width: 60%),
  caption: [Parameter passing]
)
Examples of system calls:
- Process management: create, terminate, load, execute, get process attributes, set process attributes, wait time, wait event, signal event, dump memory on error, single step executing for debugging, locks for managing shared data
- File management: create, open, delete, read, write
- Device management: request device, release device, read, write, get device attributes, set device attributes
- Information maintenance: get time or date, set time or date, get system data, set system data
- Communications: send/receive messages, open/close connection, gain access to shared memory
- Protection: control access to resources, get and set permissions, allow/deny user access
== OS design
When designing an operating system, the following aspects need to be taken into consideration:
- User goals: friendly, reliable, safe, fast
- System goals: easy to design, modular, error-free, flexible, efficient
== OS structure
OSs may be structured in different ways or may be designed according to different architectures.
=== Simple structure - MS-DOS
MS-DOS has a very simple structure: a shell starts a program and when the process ends the shell is rebooted into a new program. There is at most one process running.
=== Monolithic kernel - UNIX
Originally UNIX had a monolithic structure. The kernel provided a large number of functions, such as the file system, CPU scheduling, memory management. The advantages of using a monolithic kernel are that it is fast and energy-efficient, but it is not modular and even small changes require refactoring of the code and recompilation of the whole OS.
=== Layered approach
The operating system is divided into multiple layers, where each layer is built on top of the lower layers (similar to ISO/ISO reference model and TCP/IP stack). This allows for more modularity and a change of one layer doesn't always imply a recompilation of the whole operating system. An example of a possible layered structure is the following: hardware -> drivers -> file system -> error detection & protection -> user programs.
=== Microkernel
The microkernel approach moves processes as much as possible outside the kernel into the user space. Communication between modules is achieved using message passing. The advantages of this approach are full modularity and extendability, security (a malicious process can't damage others) and reliability (less code is running in kernel mode). Message passing introduces additional overhead, thus has a negative performance impact.
#figure(
  image("Images/Microkernel structure.png", width: 60%),
  caption: [Microkernel structure]
)