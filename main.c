#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct Counts {
  long lines;
  long words;
  long bytes;
  long chars;
};

struct Counts count_all(FILE* file)
{
  struct Counts c = {0};
  bool in_word = false;
  int ch;

  while ((ch = fgetc(file)) != EOF) {
    c.bytes++;

    if ((ch & 0xC0) != 0x80) c.chars++; // Manual UTF-8 continuation char handling

    if (ch == '\n') c.lines++;

    if (isspace(ch)) {
      in_word = false;
    } else if (!in_word) {
      c.words++;
      in_word = true;
    }
  }
  return c;
}

int main(int argc, char* argv[])
{
  bool bytes = false;
  bool lines = false;
  bool words = false;
  bool chars = false;
  char* filename = NULL;

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
    if (strcmp(argv[i], "-m") == 0) {
      chars = true;
      continue;
    }
    filename = argv[i];
  }

  FILE* file;

  if (filename) {
    file = fopen(filename, "r");
    if (!file) {
      printf("Error opening file %s. Ensure the file exists and is readable.\n", filename);
      return -1;
    }
  } else {
    file = stdin;
  }

  struct Counts c = count_all(file);

  if (bytes) {
    printf("%ld %s\n", c.bytes, filename);
  }
  if (lines) {
    printf("%ld %s\n", c.lines, filename);
  }
  if (words) {
    printf("%ld %s\n", c.words, filename);
  }
  if (chars) {
    printf("%ld %s\n", c.chars, filename);
  }

  if (!bytes && !lines && !words && !chars) { // Default printing
    printf(" %ld %ld %ld %s\n", c.lines, c.words, c.bytes, filename ? filename : "");
  }

  if (filename) fclose(file);

  return 0;
}
