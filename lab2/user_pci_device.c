#include <stdio.h>
#include <linux/sched.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <unistd.h>
#include "pci_device.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid usage\ntry: ./a.out <vendor_id> <device_id>\n");
        return -1;
    }
    char *hex_vendor_id = argv[1];
    char *ptr;
    int vendor_id = strtol(hex_vendor_id, &ptr, 16);
    char *hex_device_id = argv[2];
    char *ptr2;
    int device_id = strtol(hex_device_id, &ptr2, 16);
    struct result *result = (struct result *) malloc(sizeof(struct result));

    long retval = syscall(549, vendor_id, device_id, result);
    printf("Syscall reted: %ld\n", retval);
    if (retval != 0) {
        free(result);
        printf("ERROR: SYSCALL RETURN ERROR\n");
        return -1;
    }

    if (!result->size) {
        printf("None");
    } 

    for (int i = 0; i < result->size; i++) {
        printf("\nDevice #%d:\n", i+1);
        printf(" Name %s\n", result->devices[i].name);
        printf(" Vendor ID = %X, Device ID = %X\n", result->devices[i].vendor_id, result->devices[i].device_id);
        printf(" Revision ID = %d\n", result->devices[i].revision_id);
        printf(" Interrupt Line = %d\n", result->devices[i].interrupt_line);
        printf(" Latency Timer = %d\n", result->devices[i].latency_timer);
        printf(" Command = %d\n", result->devices[i].command);
	}
    free(result);
    
    return 0;
}
