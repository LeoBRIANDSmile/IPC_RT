# POSIX Real Time applications

This directory demonstrate the use of different IPCs in a Real Time context.

## Shared memory

Shared memory is the fastest interprocess communication mechanism. The operating system maps a memory segment in the address space of several processes, so that several processes can read and write in that memory segment without calling operating system functions.

## Mutex

A mutex is a program object that prevents multiple threads from accessing the same shared resource simultaneously.

## Semaphore

A semaphore is a variable or abstract data type used to control access to a common resource by multiple threads and avoid critical section problems in a concurrent system such as a multitasking operating system.

## Message queue

Message Queue provides temporary message storage when the destination program is busy or not connected. 

## Scripts

### make_all.sh : 

This script allows you to compile each directory that contains a Makefile

### clean_all.sh : 

This script allows you to clean each directory that contains a Makefile

/!\ Before commiting something make sure to clean_all so that you won't commit executables. /!\
