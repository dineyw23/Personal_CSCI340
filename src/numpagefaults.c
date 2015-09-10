/**
 * @file numpagefaults.c	
 * @author Diney Wankhede
 * @desc: Loadable Kernel Module to make in entry to /proc filesystem
 *        Fetches the page fault stats from vmstat 
 *        Referred to torvalds GIT repo.
 * Link: 
 * https://github.com/torvalds/linux/blob/master/fs/proc/version.c
 */

#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h> 		//basic filsesystem 
#include <linux/proc_fs.h>	// /proc filesystem
#include <linux/seq_file.h> 	//sequence files
#include <linux/mm.h>		// vmstat events

MODULE_AUTHOR("Diney W");
MODULE_DESCRIPTION("Number of page faults to /proc filesystem");

/*
	Definig what to display when _show is called (proc file is listed)
*/
static int num_pf_proc_show(struct seq_file *m, void *v)
{
  //No. of event items as the size of temp array type unsigned long
	unsigned long temp[NR_VM_EVENT_ITEMS]; 
	all_vm_events(temp); 
	seq_printf(m, "%lu\n",temp[PGFAULT]);
	return 0;
}
/*
   Open the /proc file which we created
*/
static int num_pf_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, num_pf_proc_show, NULL);
}
/*
   Declared in fs.h, this structure defines various fields of the file
*/
static const struct file_operations num_pf_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= num_pf_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
/*
	Function is called on module entry.
*/
static int __init proc_pf_init(void)
{
	if (!proc_create("num_pagefaults", 0, NULL, &num_pf_proc_fops)) {
	
		return -ENOMEM;
	}
	printk(KERN_INFO "Initializing the num_pagefaults module");
	return 0;
}
/*
	Cleaning up module.
*/
static void __exit proc_pf_exit(void)
{
	printk(KERN_INFO "Cleaning up the num_pagefaults module");
	remove_proc_entry("num_pagefaults", NULL);
}

module_init(proc_pf_init);  //Calling the __init
module_exit(proc_pf_exit);  //Calling the __exit

MODULE_LICENSE("GPL");
