#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_parser.h"

int main(int argc, char* argv[]) {
  int i;
  FILE* fp;
  char* line;
  my_log log;

  line = (char*)malloc(sizeof(char)*1024);

  for ( i = 1; i < argc; i++ ) {
    fp = fopen(argv[i], "r");
    if ( fp == NULL ) {
      fprintf(stderr, "cannot open %s\n", argv[i]);
      continue;
    }
    else {
      while ( fgets(line, 1024, fp) != NULL ) {
        parse_log(line, &log);
        print_json(log);
      }
    }
    fclose(fp);
  }

  free(line);

  return 0;
}

