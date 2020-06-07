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
  char** files;
  int file_count = 0;
  int counter_size = 0;
  my_time begin;
  my_time end;
  int target;
  int num = 10;
  bool opt_time = false;
  bool opt_begin = false;
  bool opt_end = false;

  line = (char*)malloc(sizeof(char)*1024);
  files = (char**)malloc(sizeof(char*)*argc);
  for ( i = 0; i < argc; i++ ) {
    files[i] = (char*)malloc(sizeof(char)*128);
  }
  for ( i = 0; i < 24; i++ ) {
    counter[i] = (host_counter*)malloc(sizeof(host_counter)*65536);
  }

  for ( i = 1; i < argc; i++ ) {
    if ( strcmp("--time",argv[i]) == 0 || strcmp("-t",argv[i]) == 0 ) {
      sscanf( argv[++i], "%d", &target );
      opt_time = true;
    }
    else if ( strcmp("--begin",argv[i]) == 0 || strcmp("-b",argv[i]) == 0 ) {
      sscanf( argv[++i], "%d/%d/%d", &(begin.year), &(begin.month), &(begin.day));
      opt_begin = true;
    }
    else if ( strcmp("--end",argv[i]) == 0 || strcmp("-e",argv[i]) == 0 ) {
      sscanf( argv[++i], "%d/%d/%d", &(end.year), &(end.month), &(end.day));
      opt_end = true;
    }
    else if ( strcmp("-n",argv[i]) == 0 ) {
      sscanf( argv[++i], "%d", &num );
    }
    else {
      sprintf( files[file_count++], "%s", argv[i] );
    }
  }

  for ( i = 0; i < file_count; i++ ) {
    fp = fopen(files[i], "r");
    if ( fp == NULL ) {
      fprintf(stderr, "cannot open %s\n", files[i]);
      continue;
    }
    else {
      while ( fgets(line, 1024, fp) != NULL ) {
        parse_log(line, &log);
        // print_json(log);
        if ( ( opt_begin == false || date_compare(begin, log.time) >= 0 ) &&
             ( opt_end   == false || date_compare(log.time, end)   >= 0 ) ) {
          count( log, counter[log.time.hour], &counter_size );
        }
      }
    }
    fclose(fp);
  }

  if ( opt_time ) {
    printf("%d 時のアクセス数　上位 %d 件\n", target, num);
    for ( i = 0; i < num; i++ ) {
      printf("%s ... %d 件\n", counter[target][i].host, counter[target][i].count);
    }
  }
  else {
    for ( i = 0; i < 24; i++ ) {
      printf("%d 時のアクセス数　上位 %d 件\n", i, num);
      for ( j = 0; j < num; j++ ) {
        printf("%s ... %d 件\n", counter[i][j].host, counter[i][j].count);
      }
    }
  }

  for ( i = 0; i < 24; i++ ) {
    free(counter[i]);
  }
  for ( i = 0; i < argc; i++ ) {
    free(files[i]);
  }
  free(files);
  free(line);

  return 0;
}

