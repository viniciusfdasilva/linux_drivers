#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#include "my_ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinicius GNU/Linux");
MODULE_DESCRIPTION("Implementation for ioctl in a LKM");

static const int32_t read_write = 42;

static int open(struct inode *device_fd, struct file *instance)
{
    printk("ioctl_driver - Open was called\n");
    return 0;
}

static int close(struct inode *device_fd, struct file *instance)
{
    printk("ioctl_driver - Close was called\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
    struct s test;

    switch (cmd)
    {
        case WR_VALUE:
            if(copy_from_user(&read_write, (int32_t *)arg, sizeof(read_write)))
            {
                printk("ioctl_driver - Error copying data from user!\n");
            }else
            {
                printk("ioctl_driver - Update the answer to %d\n", read_write);
            }
            break;

        case RD_VALUE:
            if(copy_to_user((int32_t *)arg, &read_write, sizeof(read_write)))
            {
                printk("ioctl_driver - Error copying data to user!\n");
            }else
            {
                printk("ioctl_driver - The answer was copied\n");
            }
            break;
        
        case GREETER:
            if(copy_from_user(&test, (struct s *)arg, sizeof(test)))
            {
                printk("ioctl_driver - Error copying data from user!\n");
            }else{
                printk("ioctl_driver - %d greets to %s\n", test.repeat, test.name);
            }
            break;
    }
    return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = open,
    .release        = close,
    .unlocked_ioctl = my_ioctl
};

#define MMAJOR 64

static int __init m_init(void)
{
    int retval = register_chrdev(MMAJOR, "my_ioctl", &fops);

    if(retval == 0)
    {
        printk("ioctl_driver - registered Device number Major: %d, Minor: %d", MMAJOR,0);
    }else if(retval > 0)
    {
        printk("ioctl_driver - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
    }else{
        printk("Could not register device number!\n");
        return -1;
    }
    return 0;
}

static void __exit m_exit(void)
{
    unregister_chrdev(MMAJOR, "my_ioctl");
    printk("Left from Kernel\n");
}

module_init(m_init);
module_exit(m_exit);