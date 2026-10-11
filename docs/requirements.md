
# RaidVol - Project Requirements

## 1. Project Overview

RaidVol is a C-based software RAID prototype that combines
three virtual disk-image files into one logical storage layer.

## 2. Functional Requirements

1. The system shall maintain three virtual disk-image files.
2. The system shall distribute logical blocks using RAID 0 striping.
3. The system shall support writing data to logical blocks.
4. The system shall support reading data from logical blocks.
5. The system shall check whether each virtual disk is available.
6. The system shall report READY or DEGRADED volume status.
7. The command-line interface shall provide menu-driven operations.
8. The system shall reject invalid read/write arguments, including
   null buffers and zero-length transfers.
9. The RAID layer shall protect individual read/write operations
   using a POSIX mutex.

## 3. Non-Functional Requirements

- Implementation language: C
- Operating system: Ubuntu Linux
- Compiler: GCC
- Build system: GNU Make
- Thread support: POSIX pthreads
- Block size: 512 bytes
- Code should compile with warning flags enabled.
- The system should be testable using automated tests.
- Memory behavior should be checked with Valgrind.
- System calls should be inspectable using strace.

## 4. Hardware and Software Requirements

### Software

- Ubuntu Linux or a compatible Linux environment
- GCC compiler
- GNU Make
- POSIX threads
- Valgrind
- strace

### Storage

Three virtual disk-image files:

- disks/disk1.img
- disks/disk2.img
- disks/disk3.img

## 5. Testing Requirements

The project should be checked for:

1. Successful write operations.
2. Successful read and data comparison.
3. Correct mapping across all three disks.
4. Correct mapping to subsequent physical blocks.
5. Rejection of invalid write buffers.
6. Rejection of invalid read buffers.
7. Rejection of zero-size writes.
8. Concurrent read/write operation behavior.
9. Memory leaks and memory errors.
10. Disk availability and system-call behavior.

## 6. Current Validation Results

- Automated RAID tests: 8 passed, 0 failed.
- Concurrency test: 3 threads completed 100 iterations each.
- Valgrind: no remaining heap allocations and 0 reported errors.
- strace: virtual disk file open and close operations observed.
- GNU time: program exited successfully with status 0.

## 7. Constraints and Limitations

- RAID 0 does not provide redundancy or parity.
- Failure of a disk may make part of the logical volume unavailable.
- The project uses disk-image files rather than physical disks.
- The mutex protects individual RAID operations, not an entire
  multi-block transaction.
- Performance measurements require repeatable workloads for
  meaningful comparison.

## 8. Future Enhancements

- Add repeatable read/write performance benchmarks.
- Expand concurrency stress testing.
- Improve error reporting and recovery.
- Add persistent metadata validation and crash-recovery handling.
- Explore RAID 1 or RAID 5 as future extensions.
