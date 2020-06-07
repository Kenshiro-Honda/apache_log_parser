#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_parser.h"

int main(int argc, char* argv[]) {
  int i,j;
  FILE* fp;
  char* line;
  my_log log;
  host_counter* counter[24]; // 24 時間それぞれにカウンターを用意
  int counter_size = 0;

  line = (char*)malloc(sizeof(char)*1024);
  for ( i = 0; i < 24; i++ ) {
    counter[i] = (host_counter*)malloc(sizeof(host_counter)*65536);
  }

  for ( i = 1; i < argc; i++ ) {
    fp = fopen(argv[i], "r");
    if ( fp == NULL ) {
      fprintf(stderr, "cannot open %s\n", argv[i]);
      continue;
    }
    else {
      while ( fgets(line, 1024, fp) != NULL ) {
        parse_log(line, &log);
        // print_json(log);
        count( log, counter[log.time.hour], &counter_size );
      }
    }
    fclose(fp);
  }

  for ( i = 0; i < 24; i++ ) {
    printf("%d 時のアクセス数　上位 10 件\n", i);
    for ( j = 0; j < 10; j++ ) {
      printf("%s ... %d 件\n", counter[i][j].host, counter[i][j].count);
    }
  }

  for ( i = 0; i < 24; i++ ) {
    free(counter[i]);
  }
  free(line);

  return 0;
}

