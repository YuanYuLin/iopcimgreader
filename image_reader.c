#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BLOCK_SIZE	128
#define OPS_READ	"read"
#define OPS_WRITE	"write"
void main_usage()
{
    printf("prog <ops> <dev> <offset> <size>\n");
    printf("\tops:\n");
    printf("\t\tread\n");
    printf("\t\twrite\n");
    printf("\tsize <= %d\n", BLOCK_SIZE);
}

int read_bytes(uint8_t* dev_name, uint32_t dev_offset, uint32_t dev_size)
{
    int fd = -1;
    uint8_t block[BLOCK_SIZE] = {0};
    if(dev_size > BLOCK_SIZE) {
        main_usage();
	return 2;
    }
    fd = open(dev_name, O_RDONLY, 0644);
    if(fd < 0) {
        printf("open [%s] failed\n", dev_name);
        return 1;
    }
    lseek(fd, dev_offset, SEEK_SET);
    read(fd, &block[0], dev_size);
    close(fd);
    return 0;
}

int write_header(uint8_t* dev_name, uint32_t dev_offset)
{
    int fd = -1;
    uint8_t block[BLOCK_SIZE] = { 0 };
    fd = open(dev_name, O_RDWR, 0644);
    if(fd < 0) {
        printf("open [%s] failed\n", dev_name);
	return 1;
    }
    lseek(fd, dev_offset, SEEK_SET);
    write(fd, block, BLOCK_SIZE);
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    uint8_t *dev_ops = NULL;
    uint8_t *dev_name = NULL;
    uint32_t dev_offset = 0;
    uint32_t dev_size = 0;
    if(argc < 5) {
        main_usage();
        return 1;
    }

    dev_ops = argv[1];
    dev_name = argv[2];
    dev_offset = strtoul(argv[3], NULL, 10);
    dev_size = strtoul(argv[4], NULL, 10);
    if(strncmp(dev_ops, OPS_READ, strlen(OPS_READ))) {
        printf("Dev[%s] read off %ld size %ld\n", dev_name, dev_offset, dev_size);
        read_bytes(dev_name, dev_offset, dev_size);
	return 0;
    }
    if(strncmp(dev_ops, OPS_WRITE, strlen(OPS_WRITE))) {
        printf("Dev[%s] write off %ld\n", dev_name, dev_offset);
        write_header(dev_name, dev_offset);
	return 0;
    }
    printf("Dev[%s] unknown off %ld size %ld\n", dev_name, dev_offset, dev_size);
    return 1;
}

