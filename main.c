#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  int year, month, day;
  int hour, min, sec;
  int timezone;
} my_time;

char month_name[12][4] = 
  {"Jan","Feb","Mar","Apr","May","Jun",
   "Jul","Aug","Sep","Oct","Nov","Dec"};

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
void print_json(my_log);

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

void parse_log(char* line, my_log* log) {
  char time_str[256];
  sscanf(line, "%s %s %s [%[^]]] \"%[^\"]\" %d %d \"%[^\"]\" \"%[^\"]\"", 
    log->host, log->client_id, log->username, time_str,
    log->request, &(log->status), &(log->res_size), 
    log->referer, log->user_agent);
  parse_time(time_str, &(log->time));
}

void parse_time(char* str, my_time* time) {
  char month_str[4];
  int i;
  sscanf(str, "%d/%[^/]/%d:%d:%d:%d %d", 
    &(time->day), month_str, &(time->year),
    &(time->hour), &(time->min), &(time->sec),
    &(time->timezone));
  for ( i = 0; i < 12; i++ ) {
    if ( strcmp(month_str,month_name[i]) == 0 ) {
      time->month = i+1;
      break;
    }
  }
}

void print_json(my_log log) {
  printf("{\n");
  printf("  \"host\":\"%s\",\n", log.host);
  printf("  \"client_id\":\"%s\",\n", log.client_id);
  printf("  \"time\":\"%d/%02d/%02d %02d:%02d:%02d %s%04d\",\n",
    log.time.year, log.time.month, log.time.day,
    log.time.hour, log.time.min, log.time.sec,
    (log.time.timezone<0?"-":log.time.timezone>0?"+":""), abs(log.time.timezone));
  printf("  \"request\":\"%s\",\n", log.request);
  printf("  \"status\":%d,\n", log.status);
  printf("  \"res_size\":%d,\n", log.res_size);
  printf("  \"referer\":\"%s\",\n", log.referer);
  printf("  \"user_agent\":\"%s\"\n", log.user_agent);
  printf("}\n");
}

