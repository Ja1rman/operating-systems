#include <stdio.h>
#include <linux/sched.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <unistd.h>
#include "custom_vm_area_info.h"


int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Invalid usage\ntry: ./a <pid> <addr>\n");
        return -1;
    }

    int pid = atoi(argv[1]);
    unsigned long addr = strtoul(argv[2], NULL, 0);
    struct custom_vm_area_info* data = malloc(sizeof(struct custom_vm_area_info));
    if (data == NULL) {
        printf("ERROR: CANT ALLOCATE MEMORY\n");
        return -1;
    }

    long int retval = syscall(550, pid, addr, data);
    if (retval != 0) {
        free(data);
        printf("ERROR: SYSCALL RETURN EROR\n");
        return -1;
    }
  
    printf("vm_area_struct:\n");
    printf("    vm_start = %lu\n", data->vm_start);
    printf("    vm_end = %lu\n",data->vm_end);
    printf("    vm_flags = %lu\n",data->vm_flags);
    printf("    vm_pgoff = %lu\n",data->vm_pgoff);

    free(data);

    return 0;
}