#include <linux/syscalls.h>
#include <linux/pid.h> 
#include <linux/sched.h>
#include <linux/uaccess.h>
#include "custom_vm_area_info.h"


SYSCALL_DEFINE3(gimme_vm_area_info, int, pid, unsigned long, addr, void*, data)
{
    struct task_struct *task;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_ERROR "NO task found");
        return -1;
    }
    struct vm_area_struct *vm_info = find_vma(task->active_mm, addr);
    if (vm_info == NULL) {
        printk(KERN_ERROR "CANT FIND BY ADDR");
        return -1; 
    }
    struct custom_vm_area_struct *cvmas = kmalloc(sizeof(struct custom_vm_area_info), GFP_KERNEL);
    if (cvmas == NULL) {
        printk(KERN_ERROR "CANT CONVERT");
        return -1;
    }

    cvmas->vm_start = vm_info->vm_start;
    cvmas->vm_end = vm_info->vm_end;
    cvmas->vm_flags = vm_info->vm_flags;
    cvmas->vm_pgoff = vm_info->vm_pgoff;
	printk(KERN_INFO "vm_start = %lu, vm_end = %lu\n", cvmas->vm_start, cvmas->vm_end);

    if (copy_to_user(data, cvmas, sizeof(struct custom_vm_area_info)) != 0) {
        kfree(cvmas);
        printk(KERN_ERROR "CANT SEND TO USER");
        return -1;
    }
    kfree(cvmas);
	
    return 0;
}
