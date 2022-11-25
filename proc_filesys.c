#include <linux/kernel.h>   
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFSIZE 100


MODULE_LICENSE("GPL");


static int uuid = 1;
module_param(uuid, int, 1);
MODULE_PARM_DESC(uuid, "User ID for the process owner");

char message[BUFFSIZE];  // used to determine user payload (whoami or PID)

/* The read file system operation for this custom proc file system */
static ssize_t read_fn(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
    char buff[BUFFSIZE];

    ssize_t len = 0;  // will return the length read or fault errors

    // return 0 if offset is out of bound
    if(*ppos > 0 || count < BUFFSIZE)
        return 0;

    if(strcmp(message, "whoami") == 0)
    {
        char *asuid = "0123456789";
        char *student_name = "John Doe";
        len += sprintf(buff, "[CSE330][Fall2022][Project-4][ASUID:%s][%s]\n", asuid, student_name);
    }
    else
    {
        pid_t pid;
        int bytes = sscanf(message, "%d", &pid);  // attempt extracting process id from user message
        if(bytes < 0)
            len += sprintf(buff, "Could not get the process id\n");
        else
        {
            int rss = 1668;
            int swap = 0;
            int wss = 1688;
            char *elapsed = "00:00:05";
            len += sprintf(buff, "[PID:%d] - [RSS: %dKB] - [SWAP:%dKB] - [WSS=%dKB] - [ELAPSED TIME=%s]\n"
                    , pid, rss, swap, wss, elapsed);
        }
    }

    // send to userspace with error checking and reporting
    if(copy_to_user(ubuf, buff, len))
        return -EFAULT;

    *ppos = len; // update offset to last read position

    return len;
}

/* The write file system operation for the custom proc file system */
static ssize_t write_fn(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
    char buff[BUFFSIZE]; // will receive the message

    ssize_t len = 0; // the length written to be returned

    // check offset and buffsize bounds
    if(*ppos > 0 || count > BUFFSIZE)
        return -EFAULT;

    // receive from userspace with error checking and reporting
    if(copy_from_user(buff, ubuf, count))
        return -EFAULT;

    sscanf(buff, "%s", message);

    len += strlen(buff); // update read bytes count

    *ppos = len;  // update next read offset position

    return len;
}

// available file system operations for this custom fs
static struct proc_ops test_proc_fs = {
    .proc_read = read_fn,
    .proc_write = write_fn,
};

// proc_parent = proc_mkdir("cse330", NULL);
static struct proc_dir_entry *proc_parent;

// identity proc entry copies a message from kernel space to userspace
static struct proc_dir_entry *identity;

// statistics proc entry copies process stats from kernel space to userspace
static struct proc_dir_entry *statistics;

/* function to initialize this custom kernel module */
static int procfs_init(void)
{
    proc_parent = proc_mkdir("cse330", NULL);
    identity = proc_create("identity", 0666, proc_parent, &test_proc_fs);
    statistics = proc_create("statistics", 0666, proc_parent, &test_proc_fs);

	printk(KERN_ALERT "proc entries created...\n");
	return 0;
}

/* procedure to exit this custom kernel module */
static void procfs_exit(void)
{
    remove_proc_entry("identity", proc_parent);
    remove_proc_entry("statistics", proc_parent);
    proc_remove(proc_parent);
	printk(KERN_WARNING "proc entries destroyed...\n");
}

module_init(procfs_init);
module_exit(procfs_exit);
