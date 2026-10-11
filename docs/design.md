
# RaidVol - A Software RAID Layer

## 1. Project Objective

RaidVol is a software-based RAID layer that manages multiple
virtual disk-image files as a single logical storage volume.

The project demonstrates block-level data distribution, disk
read/write operations, disk status monitoring, concurrency
synchronization, and system-level testing.

## 2. Platform and Tools

- Operating System: Ubuntu Linux
- Programming Language: C
- Compiler: GCC
- Build Tool: GNU Make
- Debugger: GDB
- Memory Checker: Valgrind
- System Call Tracer: strace
- Thread Library: POSIX pthreads

## 3. RAID Configuration

- RAID Level: RAID 0
- Number of Virtual Disks: 3
- Disk Image Files:
  - disks/disk1.img
  - disks/disk2.img
  - disks/disk3.img
- Block Size: 512 bytes

RAID 0 distributes data across disks using striping. It
improves data distribution but does not provide redundancy
or fault tolerance.

## 4. Main Modules

### 4.1 CLI Module

Implemented in src/main.c.

Provides a menu for checking the RAID volume, writing data,
reading data, displaying disk status, and exiting the program.

### 4.2 Disk Manager

Implemented in src/disk.c.

Provides disk existence checks and block-level read/write
operations on the virtual disk-image files.

### 4.3 RAID Manager

Implemented in src/raid.c.

Maps logical blocks to physical blocks on individual disks
and provides RAID read, write, and status operations.

### 4.4 Disk Status and Failure Handling

The RAID status function checks whether each disk image is
available and reports the volume as READY or DEGRADED.

RAID 0 does not reconstruct missing data if a disk fails.

### 4.5 Concurrency Synchronization

A POSIX mutex protects individual RAID read and write
operations from concurrent execution within the process.

The concurrency test runs three threads, each performing
100 write/read iterations.

## 5. RAID 0 Design

Data is distributed across the three virtual disks using
logical-block striping.

The mapping formulas are:

    disk_index = logical_block % NUM_DISKS

    physical_block = logical_block / NUM_DISKS

Example:

    Logical block 0 -> Disk 1, physical block 0
    Logical block 1 -> Disk 2, physical block 0
    Logical block 2 -> Disk 3, physical block 0
    Logical block 3 -> Disk 1, physical block 1
    Logical block 4 -> Disk 2, physical block 1
    Logical block 5 -> Disk 3, physical block 1

The implementation uses zero-based disk indices internally.
The user interface displays disk numbers starting from 1.

Each RAID read/write operation accepts a non-zero transfer
size up to 512 bytes.

## 6. Operating System Concepts

- File operations on virtual disk-image files
- Block-level file I/O
- System calls and file descriptors
- POSIX threads
- Mutex-based synchronization
- Resource and storage management
- Error handling and disk availability checks
- Memory debugging and system-call tracing

## 7. System Architecture

    User
      |
      v
    Command-Line Interface
      |
      v
    RAID Manager
      |
      +---- RAID 0 Block Mapping
      |
      +---- Mutex Synchronization
      |
      v
    Disk Manager
      |
      +---- disk1.img
      +---- disk2.img
      +---- disk3.img

## 8. Testing and Validation

The project has been tested using automated tests,
concurrency testing, Valgrind, strace, and GNU time.

Recorded results:

- Automated RAID tests: 8 passed, 0 failed
- Concurrency test: 3 threads, 100 iterations each
- Valgrind: 0 reported errors and no remaining heap blocks
- strace: disk image open/close operations observed
- GNU time: successful exit status 0

## 9. Performance Measurement

A menu-exit run using /usr/bin/time -v reported:

- Elapsed wall-clock time: 2.70 seconds
- Maximum resident set size: 1576 KB
- User CPU time: 0.00 seconds
- System CPU time: 0.00 seconds

The elapsed time includes time spent entering the menu
selection and is not a pure benchmark of RAID performance.

## 10. Limitations

- RAID 0 provides no redundancy or data recovery.
- Failure of a disk can make part of the logical volume
  unavailable.
- The current implementation is a prototype using disk
  image files rather than physical storage devices.
- The mutex protects individual RAID operations; a sequence
  of multiple block writes is not an atomic transaction.
- The current tests do not establish performance under
  heavy real-world storage workloads.

## 11. Future Enhancements

- Add larger-scale performance benchmarks.
- Improve error reporting and recovery procedures.
- Add stress tests for concurrent access.
- Explore RAID 1 mirroring or RAID 5 parity as extensions.
- Improve metadata management and crash recovery.
