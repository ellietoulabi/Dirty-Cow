#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

void *map;
int offset;


int main(int argc, char *argv[])
{
 
  
  struct stat st;
  int file_size;
  int f=open("/etc/passwd", O_RDONLY);
  // Map the file to COW memory using MAP_PRIVATE.
  fstat(f, &st);
  file_size = st.st_size;
  map=mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, f, 0);

  // Find the offset to the target area
  char *start = strstr(map, "test:x:1001:1001"); 
  printf("%s",start);
  offset = start - (char *)map;
  printf("distance: %d\n", offset);

 
  return 0;
}

