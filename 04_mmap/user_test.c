#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEVNAME "/dev/m_dev" // Define the path to the device file
#define KB_4 4096

int main(int argc, char **argv)
{
    int fd, status, offset;
    char text[KB_4]; // Buffer to hold data

    void *ptr;  // Pointer for memory mapping

    // Check if the user provided the required arguments
    if(argc < 2)
    {
        printf("Usage: %s [m,r,w,p] {data}\n", argv[0]);
        return 0;
    }

    // Open the device file with read/write access
    fd = open(DEVNAME, O_RDWR);

    if(fd < 0)
    {
        perror("open"); // Print an error message if opening fails
        return 1;
    }

    // Determine the operation based on the first argument
    switch(argv[1][0])
    {
        // Handle read operation
        case 'r':
            status = read(fd, text, KB_4); // Read data from the device
            printf("RED: I got %d bytes with '%s'\n", status, text);
            break;

        // Handle write operation
        case 'w':

            if(argc < 3)
            {
                printf("Usage: %s w [data]\n", argv[0]);
                break;
            }

            // Clear the buffer and copy user-provided data into it
            memset(text, 0, KB_4);
            strcpy(text, argv[2]);
            
            // Write the data to the device
            status = write(fd, text, KB_4);
            printf("Wrote %d bytes\n", status);
            break;
        
        // Handle memory mapping operation
        case 'm':

            // Map the device memory into the process's address space
            ptr = mmap(NULL,  KB_4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            if(ptr == MAP_FAILED)
            {
                perror("mmap");
                break;
            }

            // If additional data is provided, copy it into the mapped memory
            if(argc > 2)
            {
                memset(text, 0, KB_4);
                strcpy(text, argv[2]);
                memcpy(ptr, text, KB_4);
            }

            // Copy the content of the mapped memory into the buffer and print it
            memset(text, 0, KB_4);
            memcpy(text, ptr, KB_4);
            printf("MMAP: I got '%s'\n", text);
            
            // Unmap the memory
            munmap(ptr, KB_4);
            break;

        // Handle operation to print a specific byte at a given offset
        case 'p':
            if(argc < 3)
            {
                printf("Usage: %s p [offset]\n", argv[0]);
                break;
            }

            offset = atoi(argv[2]); // Convert the offset argument to an integer

            // Map the device memory into the process's address space
            ptr = mmap(NULL, KB_4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if(ptr == MAP_FAILED)
            {
                perror("mmap");
                break;
            }

            printf("MMAP: Byte on Offset %d: *(ptr + %d) = '%c'\n", offset, offset, *((char *) (ptr + offset)));
            munmap(ptr, KB_4);
            break;
            
        default:
            printf("'%c' is invalid.\n", argv[1][0]);
            break;
    }

    close(fd);
}
