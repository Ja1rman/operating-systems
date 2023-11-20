#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include "pci_device.h"


SYSCALL_DEFINE3(pci_device, int, vendor_id, int, device_id, void*, data) 
{
	struct pci_dev* dev = NULL;
	u16 dval;
	char byte;
	int i = 0;

	struct result *result = vmalloc(sizeof(struct result));
	struct custom_pci_dev *cpd = vmalloc(MAX_NUMBER * sizeof(struct custom_pci_dev));

	while ((dev = pci_get_device(vendor_id, device_id, dev)) && i < MAX_NUMBER) {
		printk(KERN_INFO "Device %d:\n", i);
		strncpy(cpd->name, pci_name(dev), 13);
		pci_read_config_word(dev, PCI_VENDOR_ID, &dval);
		cpd->vendor_id = dval;
		printk(KERN_INFO "Vendor_id %d ", cpd->vendor_id);
		pci_read_config_word(dev, PCI_DEVICE_ID, &dval);
		cpd->device_id = dval;
		printk(KERN_INFO "Device_id %d\n", cpd->device_id);
		pci_read_config_byte(dev, PCI_REVISION_ID, &byte);
		cpd->revision_id = byte;
		pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &byte);
		cpd->interrupt_line = byte;
		pci_read_config_byte(dev, PCI_LATENCY_TIMER, &byte);
		cpd->latency_timer = byte;
		pci_read_config_word(dev, PCI_COMMAND, &dval);
		cpd->command = dval;
		pci_dev_put(dev);
		result->devices[i] = *cpd;
		printk(KERN_INFO "New device %s, i = %d\n", cpd->name, i);
		i++;
	}
	vfree(cpd);
	result->size = i;
	if (i == 0) {
		vfree(result);
		printk("ERROR: SIZE 0");
		return -1;
	}
	if (copy_to_user(data, result, sizeof(struct result)) != 0) {
        vfree(result);
        printk("ERR: COPY ERROR");
        return -1; //  Неверный адрес
    }
	vfree(result);
	
    return 0;
}
