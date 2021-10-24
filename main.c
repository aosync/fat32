#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat32.h"

int read_sectors(uint32_t lba, uint32_t count, uint8_t *buf, void *ctx) {
	int *fd = ctx;
	int i = pread(*fd, buf, count * 512, lba * 512);
	return i != count * 512;
}

int main(int argc, char **argv) {
	if (argc < 3)
		return 1;
	
	FILE *fp = fopen(argv[1], "rb");
	int fd = fileno(fp);
	
	fat32_t fat;
	int i = fat32_init(&fat, read_sectors, 0, &fd);

	fat32_entry_t root;
	fat32_root_dir(&fat, &root);

	fat32_entry_t file = root;
	
	for (int a = 2; a < argc; a++) {
		i = fat32_seek(&fat, &file, argv[a], &file);
		if (i != FAT32_OK) {
			printf("fat32: walk error %u\n", i);
			return 1;
		}
	}

	printf("file info:\n\tfilename: %s\n\tattribute: %u\n\tcluster: %u\n\tsize (bytes): %u\n", file.filename, file.attribute, file.cluster, file.size);

	char *contents = malloc(file.size);
	if (contents == NULL)
		return 1;
	int r = fat32_read(&fat, &file, contents, file.size);
	write(1, contents, r);

	free(contents);
	fclose(fp);
	return 0;
}
