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

#define GPIO2 12
#define DEV_NAME "button_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

struct cdev gpio_cdev;

int button_open(struct inode *pinode, struct file *pfile) {
	printk(KERN_ALERT "OPEN button_dev\n");
	gpio_request(GPIO2, "GPIO2"); // allocation before using GPIO2
	gpio_direction_input(GPIO2); // GPIO2(button) is used for input

	return 0;
}

ssize_t button_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	char* pressed = "pressed"; // strlen(pressed) = 7
	char* notpress = "not pressed";
	printk(KERN_ALERT "READ button_dev\n");

	if(gpio_get_value(GPIO2) == 1) {  //return value of GPIO2, 1 : not pressed, 0 : pressed.
		copy_to_user(buffer,notpress, strlen(notpress)+1); // device driver transmit device file(message "not pressed") to app.
		printk("button X\n");

	}
	else {
		copy_to_user(buffer,pressed, strlen(pressed)+1); // device driver transmit device file(message "pressed") to app.
		printk("button O\n");
	}
	return 0;
}

int button_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "RELEASE button_dev\n");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = button_open,
	.read = button_read,
	.release = button_close,
};

int __init button_init(void) {
	printk(KERN_ALERT "INIT button test\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit button_exit(void){
	printk(KERN_ALERT "EXIT button_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
	gpio_free(GPIO2); //GPIO2 is returned to the system.
}

module_init(button_init);
module_exit(button_exit);
