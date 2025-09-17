#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>

long print_bytes(FILE* file)
{
  fseek(file, 0, SEEK_END); // move to the end of the file
  return ftell(file); // current position is equal to byte count, since we're at EOF
}

long print_chars(FILE* file)
{
  setlocale(LC_CTYPE, "");
  long counter = 0;
  wint_t wch;
  while ((wch = fgetwc(file)) != WEOF) {
    counter++;
  }
  return counter;
}

long print_lines(FILE* file)
{
  long counter = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') counter++;
  }
  return counter;
}

long print_words(FILE* file)
{
  long counter = 0;
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
  return counter;
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

  /* if (!filename) { */
  /*   printf("Proper usage: %s [-flags] [filename]", argv[0]); */
  /*   return -1; */
  /* } */
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

  if (bytes) {
    printf("%ld %s\n", print_bytes(file), filename);
  }
  if (lines) {
    printf("%ld %s\n", print_lines(file), filename);
  }
  if (words) {
    printf("%ld %s\n", print_words(file), filename);
  }
  if (chars) {
    printf("%ld %s\n", print_chars(file), filename);
  }

  if (!bytes && !lines && !words && !chars) { // Default printing
    long lc = print_lines(file);
    fseek(file, 0, SEEK_SET); // reset to beginning of file
    long wc = print_words(file);
    fseek(file, 0, SEEK_SET);
    long bc = print_bytes(file);
    printf(" %ld %ld %ld %s\n", lc, wc, bc, filename ? filename : "");
  }

  if (filename) fclose(file);

  return 0;
}
