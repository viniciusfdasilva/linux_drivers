#include <linux/module.h>
#include <linux/init.h>

/* Headers of module */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinicius Silva");
MODULE_DESCRIPTION("This module contains functions that print a greetings message inside the kernel");
MODULE_VERSION("0.1");

static const int EXIT_CODE = 0;

/**
 * @brief This function prints a hello message inside the kernel. Is called
 * when the module is loaded into the kernel. 
 */
static int __init init_greetings(void)
{
    printk(KERN_INFO "Hello, kernel!\n");
    return EXIT_CODE;
}

/**
 * @brief This function prints a exit message inside the kernel. Is called
 * when the module is removed to the kernel.
 */
static void __exit exit_greetings(void)
{
    printk(KERN_INFO "Goodbye, kernel!\n");
}

module_init(init_greetings);
module_exit(exit_greetings);