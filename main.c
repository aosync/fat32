#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include "fat32.h"

int read_sectors(uint32_t lba, uint32_t count, uint8_t *buf, void *ctx) {
	int *fd = ctx;
	int i = pread(*fd, buf, count * 512, lba * 512);
	return i != count * 512;
}

int main() {
	FILE *fp = fopen("fat.fs", "rb");
	int fd = fileno(fp);
	
	fat32_t fat;
	int i = fat32_init(&fat, read_sectors, 0, &fd);

	fat32_entry_t root;
	fat32_root_dir(&fat, &root);

	fat32_entry_t kernel;
	i = fat32_seek(&fat, &root, "INITRD", &kernel);

	printf("kernel info:\n\tfilename: %s\n\tattribute: %u\n\tcluster: %u\n\tsize (bytes): %u\n", kernel.filename, kernel.attribute, kernel.cluster, kernel.size);

	char contents[kernel.size + 1];
	int r = fat32_read(&fat, &kernel, contents, kernel.size);
	contents[r] = '\0';

	printf("\tcontents:\n%s", contents);

	fclose(fp);
	return 0;
}
