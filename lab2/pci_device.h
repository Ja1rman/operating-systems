#define MAX_NUMBER 10

struct custom_pci_dev {
	char name[32];
	int vendor_id;
	int device_id;
	char revision_id;
	char interrupt_line;
	char latency_timer;
	int command;
};

struct result {
	int size;
	struct custom_pci_dev devices[MAX_NUMBER];
};
