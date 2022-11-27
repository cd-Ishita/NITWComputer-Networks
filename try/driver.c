#include <linux/module.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/string.h>
#include<linux/kdev_t.h>
#include<linux/random.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>


#define DEV_NAME "ig"
MODULE_LICENSE("GPL");

struct cdev cdev;
dev_t dev;
char *ker_buff;
static int *usr_points,*ker_points;

static int dev_open(struct inode * in,struct file *fp);
static int dev_close(struct inode * in,struct file *fp);
static ssize_t dev_read(struct file *fp,char __user *buff,size_t len,loff_t * off);
static ssize_t dev_write(struct file *fp,const char *buff,size_t len,loff_t * off);

struct file_operations fops={
    .open=dev_open,
    .release=dev_close,
    .read=dev_read,
    .write=dev_write
};

static int dev_open(struct inode *in,struct file *fp){
    ker_buff=kmalloc(10,GFP_KERNEL);
    usr_points=kmalloc(10,GFP_KERNEL);
    ker_points=kmalloc(10,GFP_KERNEL);

    int num=get_random_int()%100;
    printk("Device is opened\n");
    printk("Kernel chose %d\n\n", num);

	int m = num;
	int digit = 0;
	while (m){
        digit++;
		m /= 10;
	}

	char ker_buff1[digit];
	int index = 0;
	while (num) {
		ker_buff1[++index] = num % 10 + '0';
		num /= 10;
	}

	int i;
	for (i = 0; i < index; i++) {
		ker_buff[i] = ker_buff1[index - i];
	}

	ker_buff[i] = '\0';

    return 0; 
}
static int dev_close(struct inode *in,struct file *fp){
    kfree(ker_buff);
    kfree(usr_points);
    kfree(ker_points);
    return 0;
}
static ssize_t dev_read(struct file *fp,char __user *buff,size_t len,loff_t * off){
    copy_to_user(buff,ker_buff,10);
    return 0;
}
static ssize_t dev_write(struct file *fp,const char *buff,size_t len,loff_t * off){
    copy_from_user(ker_buff,buff,10);
    return 0;
}

static int __init dev_init(void){
    alloc_chrdev_region(&dev,0,1,DEV_NAME);
    cdev_init(&cdev,&fops);
    cdev_add(&cdev,dev,1);
    printk("Sucessfully created in the kernel\n");
    return 0;
}
static void __exit dev_exit(void){
    cdev_del(&cdev);
    unregister_chrdev_region(dev,1);
    printk("Driver is removed from the kernel\n");
    //return 0;
}
module_init(dev_init);
module_exit(dev_exit);