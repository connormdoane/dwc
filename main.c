#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void print_bytes(FILE* file, char* filename)
{
  fseek(file, 0, SEEK_END); // move to the end of the file
  printf("%ld %s\n", ftell(file), filename); // print the end of the file in bytes, aka the size
}

void print_lines(FILE* file, char* filename)
{
  int counter = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') counter++;
  }
  printf("%d %s\n", counter, filename);
}

void print_words(FILE* file, char* filename)
{
  int counter = 0;
  bool in_word = false;
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if (isspace((unsigned char)ch)) {
      in_word = false;
    } else if (!in_word) {
      in_word = true;
      counter++;
    }
  }
  printf("%d %s\n", counter, filename);
}

int main(int argc, char* argv[])
{
  bool bytes = false;
  bool lines = false;
  bool words = false;
  char* filename;

  // command-line flags
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      bytes = true;
      continue;
    }
    if (strcmp(argv[i], "-l") == 0) {
      lines = true;
      continue;
    }
    if (strcmp(argv[i], "-w") == 0) {
      words = true;
      continue;
    }
    filename = argv[i];
  }

  FILE* file = fopen(filename, "r");

  if (!file) {
    printf("Error opening file %s. Ensure the file exists and is readable.\n", filename);
    return -1;
  }

  if (bytes) {
    print_bytes(file, filename);
  }
  if (lines) {
    print_lines(file, filename);
  }
  if (words) {
    print_words(file, filename);
  }

  fclose(file);

  return 0;
}
