#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GPIO1 24
#define DEV_NAME "touch_dev"
#define DEV_NUM 220

MODULE_LICENSE("GPL");

struct cdev gpio_cdev;

int touch_open(struct inode *pinode, struct file *pfile) {
	printk(KERN_ALERT "OPEN touch_dev\n");
	gpio_request(GPIO1, "GPIO1"); // allocation before using GPIO1
	gpio_direction_input(GPIO1);  // GPIO1(touch) is used for input
	return 0;
}

ssize_t touch_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	char* detect = "Yes"; //strlen(detect) = 3
	char* nodetect = "No";
	printk(KERN_ALERT "READ touch_dev\n");

	if(gpio_get_value(GPIO1)) { // return value of GPIO1, 1 means sensed, 0 means not sensed.
		printk("touch O\n");
		copy_to_user(buffer,detect,strlen(detect)+1); // device driver transmit device file(message "Yes") to app.
	}
	else {
		printk("touch X\n");
		copy_to_user(buffer,nodetect, strlen(nodetect)+1); // device driver transmit device file(message "No") to app.
	}
	return 0;
}

int touch_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "RELEASE touch_dev\n");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = touch_open,
	.read = touch_read,
	.release = touch_close,
};

int __init touch_init(void) {
	printk(KERN_ALERT "INIT touch test\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit touch_exit(void){
	printk(KERN_ALERT "EXIT touch_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
	gpio_free(GPIO1); // GPIO1 is returned to the system.
}

module_init(touch_init);
module_exit(touch_exit);
