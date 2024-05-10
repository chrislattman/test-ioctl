#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/spinlock.h>

#include "testdriver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chris Lattman");
MODULE_DESCRIPTION("ioctl example");
MODULE_VERSION("0.1.0");

static int char_dev_major_number = 0;
static const char *const name = "testdevice";
static int answer = 0;
static struct mystruct test = {0, ""};
static char *string;
static DEFINE_SPINLOCK(myspinlock);

static int driver_open(struct inode *dev_file, struct file *instance)
{
    return 0;
}

static int driver_release(struct inode *dev_file, struct file *instance)
{
    return 0;
}

// the return value of this function is the return value of ioctl() in user.c
static long driver_unlocked_ioctl(struct file *file, unsigned int request, unsigned long arg)
{
    int retval = 0;

    spin_lock(&myspinlock); // copy_to/from_user should be fast
    switch (request) {
    case RD_VALUE:
        if (copy_to_user((int *) arg, &answer, sizeof(answer)) != 0) {
            pr_info("Error copying value to user.\n");
            retval = -1;
        }
        break;
    case WR_VALUE:
        if (copy_from_user(&answer, (int *) arg, sizeof(answer)) != 0) {
            pr_info("Error copying value from user.\n");
            retval = -1;
        } else {
            pr_info("Received answer = %d\n", answer);
        }
        break;
    case RD_STRUCT:
        if (copy_to_user((struct mystruct *) arg, &test, sizeof(test)) != 0) {
            pr_info("Error copying struct to user.\n");
            retval = -1;
        }
        break;
    case WR_STRUCT:
        if (copy_from_user(&test, (struct mystruct *) arg, sizeof(test)) != 0) {
            pr_info("Error copying struct from user.\n");
            retval = -1;
        } else {
            pr_info("Received num = %d, name = \"%s\"\n", test.num, test.name);
        }
        break;
    default:
        pr_info("Invalid request.\n");
        retval = -1;
    }
    spin_unlock(&myspinlock);

    return retval;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_release,
    .unlocked_ioctl = driver_unlocked_ioctl
};

static int __init module_start(void)
{
    char_dev_major_number = register_chrdev(0, name, &fops);
    if (char_dev_major_number < 0) {
        pr_info("Error registering character device.\n");
        return -1;
    }
    string = kzalloc(30, GFP_KERNEL); // use kmalloc if zeroing memory isn't necessary
    if (!string) {
        pr_info("Error allocating memory for string");
        return -1;
    }
    strncpy(string, "Hello from testdriver.c!", 30);
    pr_info("%s\n", string);
    kfree(string);
    pr_info("Module loaded.\n");
    return 0;
}

static void __exit module_end(void)
{
    unregister_chrdev(char_dev_major_number, name);
    pr_info("Module unloaded.\n");
}

module_init(module_start);
module_exit(module_end);
