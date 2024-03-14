#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define MYIOCTL_MAGIC 'k'
#define MYIOCTL_RESET _IO(MYIOCTL_MAGIC, 0)
#define MYIOCTL_GET_COUNT _IOR(MYIOCTL_MAGIC, 1, int)
#define MYIOCTL_INCREMENT _IOW(MYIOCTL_MAGIC, 2, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group01");
MODULE_DESCRIPTION("Simple IOCTL Example");

static int myioctl_major;
static int myioctl_cnt = 0;
static char *myioctl_file_path __initdata = "/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt";
static struct file *myioctl_file;

// Function prototypes
static int myioctl_open(struct inode *inode, struct file *filp);
static int myioctl_release(struct inode *inode, struct file *filp);
static long myioctl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

// File operations structure
static const struct file_operations myioctl_fops = {
    .open = myioctl_open,
    .release = myioctl_release,
    .unlocked_ioctl = myioctl_ioctl,
};

// Module initialization
static int __init myioctl_init(void) {

    char cnt_valstr[20] = {0};
    loff_t pos = 0;
    size_t ret = 0;

    myioctl_major = register_chrdev(0, "myioctl", &myioctl_fops);

    if (myioctl_major < 0) {
        pr_err("Failed to register character device\n");
        return myioctl_major;
    }

    myioctl_file = filp_open(myioctl_file_path, O_RDWR | O_CREAT, 0644);

    if (IS_ERR(myioctl_file)){
        pr_err("Output file open error\n");
        return -1;
    }

    ret = kernel_read(myioctl_file, cnt_valstr, sizeof(cnt_valstr), &pos);

    if ( ret < 0 ){
        pr_err("Failed to read file\"/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt\"\n");
        return -1;
    }

    sscanf(cnt_valstr, "%d\n", &myioctl_cnt);

    pr_info("myioctl_cnt file read. cnt number: %d\n", myioctl_cnt);

    pr_info("myioctl module loaded. Major number: %d\n", myioctl_major);

    return 0;
}

// Module cleanup
static void __exit myioctl_exit(void) {

    char cnt_valstr[20] = {0};
    loff_t pos = 0;
    size_t strlen = 0;
    size_t ret = 0;

    if (IS_ERR(myioctl_file)){
        pr_err("Output file open error\n");
    }

    strlen = scnprintf(cnt_valstr, sizeof(cnt_valstr), "%d", myioctl_cnt);

    for(int i = strlen; i < sizeof(cnt_valstr) - 2; ++i){
        cnt_valstr[i] = ' ';
    }
    cnt_valstr[sizeof(cnt_valstr) - 2] = '\n';
    cnt_valstr[sizeof(cnt_valstr) - 1] = 0;

    ret = kernel_write(myioctl_file, cnt_valstr, sizeof(cnt_valstr), &pos);

    if ( ret < 0 ){
        pr_err("Failed to write file\"/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt\"\n");
    }

    pr_info("myioctl_cnt file write. cnt number: %d\n", myioctl_cnt);

    filp_close(myioctl_file, NULL);

    unregister_chrdev(myioctl_major, "myioctl");
    pr_info("myioctl module unloaded\n");
}

// Open function
static int myioctl_open(struct inode *inode, struct file *filp) {
    pr_info("myioctl device opened\n");
    return 0;
}

// Release function
static int myioctl_release(struct inode *inode, struct file *filp) {
    pr_info("myioctl device closed\n");
    return 0;
}

// IOCTL function
static long myioctl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int err = 0;
    int tmp;

    if (_IOC_TYPE(cmd) != MYIOCTL_MAGIC) {
        pr_err("Invalid magic number\n");
        return -ENOTTY;
    }

    switch (cmd) {
        case MYIOCTL_RESET:
            pr_info("myioctl: Resetting counter\n");
            myioctl_cnt = 0;
            break;

        case MYIOCTL_GET_COUNT:
            pr_info("myioctl: Getting counter value\n");
            err = copy_to_user((int *)arg, &myioctl_cnt, sizeof(int));
            break;

        case MYIOCTL_INCREMENT:
            pr_info("myioctl: Incrementing counter\n");
            err = copy_from_user(&tmp, (int *)arg, sizeof(int)) ;
            if (err == 0) {
                myioctl_cnt += tmp;
            }
            break;

        default:
            pr_err("Unknown myioctl command\n");
            return -ENOTTY;
    }

    return err;
}

module_init(myioctl_init);
module_exit(myioctl_exit);