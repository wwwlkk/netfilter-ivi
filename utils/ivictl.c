#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include "../modules/ivi_ioctl.h"

int main(int argc, char *argv[]) {
	char v4dev[IVI_IOCTL_LEN], v6dev[IVI_IOCTL_LEN];
	int retval, fd, temp;
	struct in_addr v4addr;
	int mask;
	struct in6_addr v6addr;

	printf("IVI netfilter device controller utility v1.3\n");
	
	if ((fd = open("/dev/ivi", 0)) < 0) {
		printf("Error: cannot open virtual device for ioctl, code %d.\n", fd);
		exit(-1);
	}

	if ((argc == 8) && (strcmp(argv[1], "start") == 0)) {
		// Set dev
		strncpy(v4dev, argv[2], IVI_IOCTL_LEN);
		strncpy(v6dev, argv[3], IVI_IOCTL_LEN);
		if ((retval = ioctl(fd, IVI_IOC_V4DEV, v4dev)) < 0) {
			printf("Error: failed to assign IPv4 device, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V6DEV, v6dev)) < 0) {
			printf("Error: failed to assign IPv6 device, code %d.\n", retval);
			exit(-1);
		}
		
		// Set v4 network
		if ((retval = inet_pton(AF_INET, argv[4], (void*)(&v4addr))) != 1) {
			printf("Error: failed to parse IPv4 network prefix, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V4NET, &(v4addr.s_addr))) < 0) {
			printf("Error: failed to assign IPv4 network prefix, code %d.\n", retval);
			exit(-1);
		}
		temp = atoi(argv[5]);
		mask = 0xffffffff << (32 - temp);
		if ((retval = ioctl(fd, IVI_IOC_V4MASK, &(mask))) < 0) {
			printf("Error: failed to assign IPv4 network prefix length, code %d.\n", retval);
			exit(-1);
		}
		
		// Set v6 network
		if ((retval = inet_pton(AF_INET6, argv[6], (void*)(&v6addr))) != 1) {
			printf("Error: failed to parse IPv6 network prefix, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V6NET, v6addr.s6_addr)) < 0) {
			printf("Error: failed to assign IPv6 network prefix, code %d.\n", retval);
			exit(-1);
		}
		temp = atoi(argv[7]) / 8;  // counted in bytes
		if ((retval = ioctl(fd, IVI_IOC_V6MASK, &(temp))) < 0) {
			printf("Error: failed to assign IPv6 network prefix length, code %d.\n", retval);
			exit(-1);
		}
		
		// Start ivi
		if ((retval = ioctl(fd, IVI_IOC_NONAT, 0)) < 0) {
			printf("Error: failed to disable nat44, code %d.\n", retval);
			exit(-1);
		}
		
		if ((retval = ioctl(fd, IVI_IOC_START, 0)) < 0) {
			printf("Error: failed to start IVI module, code %d.\n", retval);
			exit(-1);
		}
		
		printf("Info: successfully started IVI module.\n");
	}
	else if ((argc == 9) && (strcmp(argv[1], "start") == 0)) {
		// Set dev
		strncpy(v4dev, argv[2], IVI_IOCTL_LEN);
		strncpy(v6dev, argv[3], IVI_IOCTL_LEN);
		if ((retval = ioctl(fd, IVI_IOC_V4DEV, v4dev)) < 0) {
			printf("Error: failed to assign IPv4 device, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V6DEV, v6dev)) < 0) {
			printf("Error: failed to assign IPv6 device, code %d.\n", retval);
			exit(-1);
		}
		
		// Set v4 network
		if ((retval = inet_pton(AF_INET, argv[4], (void*)(&v4addr))) != 1) {
			printf("Error: failed to parse IPv4 network prefix, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V4NET, &(v4addr.s_addr))) < 0) {
			printf("Error: failed to assign IPv4 network prefix, code %d.\n", retval);
			exit(-1);
		}
		temp = atoi(argv[5]);
		mask = 0xffffffff << (32 - temp);
		if ((retval = ioctl(fd, IVI_IOC_V4MASK, &(mask))) < 0) {
			printf("Error: failed to assign IPv4 network prefix length, code %d.\n", retval);
			exit(-1);
		}
		
		// Set v4 public address for nat44
		if ((retval = inet_pton(AF_INET, argv[6], (void*)(&v4addr))) != 1) {
			printf("Error: failed to parse IPv4 public address, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V4PUB, &(v4addr.s_addr))) < 0) {
			printf("Error: failed to assign IPv4 public address, code %d.\n", retval);
			exit(-1);
		}
		
		// Set v6 network
		if ((retval = inet_pton(AF_INET6, argv[7], (void*)(&v6addr))) != 1) {
			printf("Error: failed to parse IPv6 network prefix, code %d.\n", retval);
			exit(-1);
		}
		if ((retval = ioctl(fd, IVI_IOC_V6NET, v6addr.s6_addr)) < 0) {
			printf("Error: failed to assign IPv6 network prefix, code %d.\n", retval);
			exit(-1);
		}
		temp = atoi(argv[8]) / 8;  // counted in bytes
		if ((retval = ioctl(fd, IVI_IOC_V6MASK, &(temp))) < 0) {
			printf("Error: failed to assign IPv6 network prefix length, code %d.\n", retval);
			exit(-1);
		}
		
		// Start ivi
		if ((retval = ioctl(fd, IVI_IOC_NAT, 0)) < 0) {
			printf("Error: failed to enable nat44, code %d.\n", retval);
			exit(-1);
		}
		
		if ((retval = ioctl(fd, IVI_IOC_START, 0)) < 0) {
			printf("Error: failed to start IVI module, code %d.\n", retval);
			exit(-1);
		}
		
		printf("Info: successfully started IVI module.\n");
	}
	else if ((argc == 2) && (strcmp(argv[1], "stop") == 0)) {
		if ((retval = ioctl(fd, IVI_IOC_STOP, 0)) != 0) {
			printf("Error: failed to stop IVI module, code %d.\n", retval);
		}
		else {
			printf("Info: successfully stopped IVI module.\n");
		}
	}
	else {
		printf("Usage: ivictl start [v4_dev] [v6_dev] [v4_prefix] [v4_prefix_len] [v6_prefix] [v6_prefix_len]\n");
		printf("       ivictl start [v4_dev] [v6_dev] [v4_prefix] [v4_prefix_len] [v4_public_addr] [v6_prefix] [v6_prefix_len]\n");
		printf("       ivictl stop\n");
	}

	close(fd);

	return retval;
}