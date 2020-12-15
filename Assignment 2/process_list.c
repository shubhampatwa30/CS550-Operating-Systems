#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched/signal.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/slab.h>
#define BUF_SIZE 256
#define DEVICE_NAME "process_list"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shubham");
MODULE_DESCRIPTION("Process List Character Device");

//Declaring the functions
static ssize_t process_list_read(struct file *, char __user *, size_t size, loff_t *);
static int process_list_open(struct inode*,struct file *);
static int process_list_close(struct inode*, struct file *);
static struct task_struct * task;
//File operations
static struct file_operations process_list_fops = {
	.owner = THIS_MODULE,	
	.open = process_list_open,
	.read = process_list_read,
	.release = process_list_close
};
//Registering Device
static struct miscdevice process_list_device  = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &process_list_fops
};
//Writing Init and exit functions:
static int __init process_list_init(void){
  printk(KERN_INFO "Initiating the Process List! \n");
	if(misc_register(&process_list_device)!=0){
		printk(KERN_ALERT "Registration FAILED!");
     	}
	return 0;
}

static void __exit process_list_exit(void) {
	printk(KERN_INFO "Exiting from Process List: \n");
	misc_deregister(&process_list_device);
}


// Writing the open and close functions
static int process_list_open(struct inode * inode,struct file * file) {
	printk(KERN_INFO "Character Device Opened\n");
	task = &init_task;
	return 0;
}


static int process_list_close(struct inode * inode,struct file *file) {
	printk(KERN_INFO "Character Device Closed!\n");
        return 0;
}



//Writing the read function:
static ssize_t process_list_read(struct file *file, char *out, size_t size, loff_t * off)
{
	int ret;
	char process[BUF_SIZE];
	memset(process,0,sizeof(char)*BUF_SIZE);
	printk(KERN_INFO "Getting Process Info\n");
	while(next_task(task) != &init_task)
	  {
	    memset(process,0,sizeof(char)*BUF_SIZE);
			sprintf(process, "PID=%d PPID=%d CPU=%d STATE=%ld",task->pid,task->parent->pid,task_cpu(task),task->state);
			printk(KERN_INFO "Copying to User: %s",process);
			ret = copy_to_user(out, process, strlen(process));
			if(ret !=0)
			{	
				printk(KERN_ALERT "Copy to User Error !");
				return -1;
			}
			task=next_task(task);
			break;
	  }
	return strlen(process);
	  }



//Register init and exit process
module_init(process_list_init);
module_exit(process_list_exit);
