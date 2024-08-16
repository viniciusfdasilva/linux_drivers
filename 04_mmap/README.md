# Simple mmap Implementation in Linux Kernel

This project provides a simple example of implementing memory-mapped I/O in a Linux kernel module. The module creates a character device that supports basic read, write, and memory mapping (`mmap`) operations. A corresponding user-space application is provided to interact with the device.

## Project Structure

- **Kernel Module (`mmap.c`)**: This file contains the kernel module that implements the character device with read, write, and mmap operations.
- **User-Space Application (`user_test.c`)**: This file contains a user-space program that interacts with the device to perform various operations.

## Kernel Module Overview

The kernel module implements the following features:

- **Memory Allocation**: Allocates a page of memory using `kzalloc()` during initialization.
- **Character Device**: Registers a character device with the specified major number.
- **Read and Write Operations**: Implements custom read and write functions that allow user-space applications to read from and write to the allocated memory.
- **Memory Mapping (`mmap`)**: Implements a custom `mmap` function that maps the allocated memory into the user-space application's address space.

### Functions in Kernel Module

- **`my_read`**: Reads data from the allocated memory and copies it to user space.
- **`my_write`**: Writes data from user space to the allocated memory.
- **`my_mmap`**: Maps the allocated memory into the user-space application's address space using `remap_pfn_range()`.

### How to Compile and Load the Kernel Module

1. **Compile the Kernel Module**:
```bash
   make
   ```


   Load the Kernel Module:

```bash
sudo insmod mmap.ko
```

Check dmesg Output:

```bash
Copy code
dmesg | tail
```

Create Device File:

```bash
sudo mknod /dev/mydev c 64 0
sudo chmod 666 /dev/mydev
```

Unload the Kernel Module:

```bash
sudo rmmod mmap
```

2. **User-Space Application Overview**

The user-space application interacts with the kernel module to perform read, write, and memory mapping operations.

Compiling:

```bash
gcc user_test.c -o user_test
```

Usage:

```bash
./user_test [m,r,w,p] {data}
```

**Commands:**

r: Reads up to 4096 bytes from the device and prints the result.

w: Writes the provided data (up to 4096 bytes) to the device.

m: Maps the device's memory into the user-space application. If data is provided, it writes the data to the mapped memory.

p: Prints the byte located at the specified offset within the mapped memory.


Example Usage

Write Data:

```bash
./user_app w "Hello, World!"
```

Read Data:

```bash
./user_app r
```

Memory Map and Manipulate Data:

```bash
./user_app m "This is a test"
```

Print Byte at Specific Offset:

```bash
./user_app p 5
```

License

This project is licensed under the GPLv2 License. See the LICENSE file for more details.

Author
Vinicius Silva GNU_Linux

References

[Ref 1](https://www.xml.com/ldd/chapter/book/ch13.html#t2)

[Ref 2](https://linux-kernel-labs.github.io/refs/heads/master/labs/memory_mapping.html#overview)
