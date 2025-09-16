#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_bytes(char* filename)
{
  FILE* file = fopen(filename, "rb");
  if (file) {
    fseek(file, 0, SEEK_END); // move to the end of the file
    printf("  %ld %s\n", ftell(file), filename); // print the end of the file in bytes, aka the size
  } else {
    printf("Error opening file %s, ensure it exists and is readable.\n", filename);
  }
}

int main(int argc, char* argv[])
{
  bool bytes = false;
  char* filename;

  // command-line flags
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      bytes = true;
      continue;
    }
    filename = argv[i];
  }

  /* printf("%d\n", bytes); */
  /* printf("%s\n", filename); */

  if (bytes) {
    print_bytes(filename);
  }
}
