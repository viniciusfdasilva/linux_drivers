#include <linux/module.h>
#include <linux/slab.h>   // For memory allocation (kmalloc, kfree)
#include <linux/fs.h>     // For file system operations (register_chrdev)
#include <linux/mm.h>     // For memory management (vm_area_struct)
#include <asm/page.h>     // For page-related macros and functions
#include <asm/io.h>       // For I/O memory access (virt_to_phys)


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinicius Silva");
MODULE_DESCRIPTION("Memory map device driver");

// Define the major number for the device and the device name
#define MYMAJOR 64
#define DEVNAME "dev"

// Pointer to hold the allocated memory
static void *data;

/*
    Custom read function to handle read system calls
*/
static ssize_t m_read(struct file *file, char __user *user_buff, size_t len, loff_t *offs)
{
    int not_copied, to_copy = (len > PAGE_SIZE) ? PAGE_SIZE : len;

    // Copy data from kernel space (my_data) to user space (user_buffer)
    not_copied = copy_to_user(user_buff, data, to_copy);
    return to_copy - not_copied;
}

/*
    Custom write function to handle write system calls
*/
static ssize_t m_write(struct file *file, const char __user *user_buff, size_t len, loff_t *offs)
{
    int not_copied, to_copy = (len > PAGE_SIZE) ? PAGE_SIZE : len;

    // Copy data from user space (user_buffer) to kernel space (my_data)
    not_copied = copy_from_user(data, user_buff, to_copy);
    return to_copy - not_copied;
}

static int m_mmap(struct file *file, struct vm_area_struct *vma)
{
    int status;

    // Calculate the physical address of the allocated page and set vm_pgoff
    vma->vm_pgoff = virt_to_phys(data) >> PAGE_SHIFT;

    // Remap the memory area to the user space address
    status = remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot);

    if(status)
    {
        printk("mmap - Error remap_pfn_range: %d", status);
        return -EAGAIN; // Return error if remapping fails
    }

    return 0;
}

// Define the file operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,  // Specify the owner of the module
	.read  = m_read,       // Assign the read function
	.write = m_write,     // Assign the write function
	.mmap  = m_mmap,      // Assign the mmap function
};

// Module initialization function
static int __init m_init(void)
{
    printk("mmap - START DEVICE DRIVER");
    
    int status;
    // Allocate a page of memory with DMA capability
    data = kzalloc(PAGE_SIZE, GFP_DMA);

    if(!data)
    {
        return -ENOMEM; // Return error if allocation fails
    }    

    printk("mmap - I have allocated a page (%ld Bytes)\n", PAGE_SIZE);
    printk("mmap - PAGESHIFT: %d\n", PAGE_SHIFT);

    // Register the character device with the defined major number and file operations
    status = register_chrdev(MYMAJOR, DEVNAME, &fops);

    if(status < 0)
    {
        printk("mmap - Error registering device number!\n");
        kfree(data); // Free allocated memory in case of error
        return status;
    }

    return 0;
}

static void __exit m_exit(void)
{
    if(data)
    {
        kfree(data); // Free the allocated memory
    }
    
    unregister_chrdev(MYMAJOR, DEVNAME);
    printk("mmap - Goodbye\n");
}

// Specify the initialization and cleanup functions
module_init(m_init);
module_exit(m_exit)