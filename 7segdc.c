#include <linux/kernel.h>

#include <linux/module.h>

#include <linux/init.h>

#include <linux/types.h>

#include <linux/ioport.h>

#include <unistd.h>

#include <linux/slab.h>

#include <linux/mm.h>

#include <asm/hardware.h>

#include <asm/uaccess.h>

#include <asm/irq.h>



#define BASE_ADDR 0xF3000000   // 베이스 주소-보드에 따라 다름



#define SEG1_OFFSET       0x0600

#define SEG2_OFFSET       0x0700

#define SEG3_OFFSET       0x0800

#define SEG4_OFFSET       0x0900

#define SEG5_OFFSET       0x0A00

#define SEG6_OFFSET       0x0B00



// '0' ~ '9' 의 폰트, 알파벳을 위한 폰트도 몇 개 추가해보자.

unsigned char num[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F };



unsigned short seg_offset[6] = {

	SEG1_OFFSET, SEG2_OFFSET, SEG3_OFFSET, SEG4_OFFSET, SEG5_OFFSET, SEG6_OFFSET

};



static int SEG_MAJOR = 65;



int seg_open(struct inode *inode, struct file *filp)

{

	MOD_INC_USE_COUNT;

	printk("7-Segment Display open \n");

	return 0;

}



int seg_release(struct inode *inode, struct file *filp)

{

	MOD_DEC_USE_COUNT;

	return 0;

}



static ssize_t seg_write(struct file *file, const char *buffer, size_t length, loff_t *offset)

{

	int i;

	unsigned char *seg_base, *seg_addr;

	size_t len = length;

	int value;



	get_user(value, (int *)buffer);



	//printk("data from user application = %d  7-segment out = 0x%x\n", value, num[value]);



	seg_base = (unsigned char *)(BASE_ADDR);



	for (i = 0; i < 6; i++) {

		seg_addr = (unsigned char *)(seg_base + seg_offset[i]);

		*seg_addr = num[value];

	}



	return len;

}



struct file_operations seg_fops = {

open: seg_open,

	  write : seg_write,

			  release : seg_release,

};



static int __init seg_init(void)

{

	int result;



	result = register_chrdev(SEG_MAJOR, "seg_device", &seg_fops);

	if (result < 0)

	{

		printk(KERN_WARNING "register_chrdev() FAIL !\n");

		return result;

	}

	if (!check_region(BASE_ADDR, 2))

		request_region(BASE_ADDR, 2, "seg_device");

	else

		printk(KERN_WARNING "!check_region() FAIL !\n");



	return result;

}



static void __exit seg_exit(void)

{

	release_region(BASE_ADDR, 2);

	if (unregister_chrdev(SEG_MAJOR, "seg_device"))

		printk(KERN_WARNING "unregister_chrdev() FAIL !\n");

}



module_init(seg_init);

module_exit(seg_exit);
