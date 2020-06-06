#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  int year, month, day;
  int hour, min, sec;
  int timezone;
} my_time;

typedef struct {
  char host[16];
  char client_id[32];
  char username[32];
  my_time time;
  char request[256];
  int status;
  int res_size;
  char referer[256];
  char user_agent[256];
} my_log;

typedef struct {
  char host[16];
  int count;
} host_counter;

void debug(char*);
void print_log(my_log);
void parse_log(char*, my_log*);
void parse_time(char*, my_time*);

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
        printf("%s", line);
      }
    }
  }

  free(line);

  return 0;
}


