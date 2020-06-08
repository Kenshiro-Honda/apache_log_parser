#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef _MY_PARSER
#define _MY_PARSER

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
void parse_log(char*, my_log*);
void parse_time(char*, my_time*);
void print_json(my_log);
void count(my_log, host_counter*, int*);
void re_sort(host_counter*, int);
int date_compare(my_time, my_time);

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
    (log.time.timezone<0?"-":"+"), abs(log.time.timezone));
  printf("  \"request\":\"%s\",\n", log.request);
  printf("  \"status\":%d,\n", log.status);
  printf("  \"res_size\":%d,\n", log.res_size);
  printf("  \"referer\":\"%s\",\n", log.referer);
  printf("  \"user_agent\":\"%s\"\n", log.user_agent);
  printf("}\n");
}

void count(my_log log, host_counter* counter, int* size) {
  bool contain = false;
  int i;
  host_counter* new_addr;
  for ( i = 0; i < *size; i++ ) {
    if ( strcmp( counter[i].host, log.host ) == 0 ) {
      contain = true;
      counter[i].count++;
      break;
    }
  }
  if ( !contain ) {
    (*size)++;
    if ( (*size)%65536 == 0 ) {
      new_addr = (host_counter*)realloc(counter, (*size)+65536);
      if ( new_addr == NULL ) {
        fprintf(stderr, "count : not enough memory\n");
        exit(1);
      }
      counter = new_addr;
    }
    sprintf( counter[i].host, "%s", log.host );
    counter[i].count = 1;
  }
  re_sort( counter, i );
}

void re_sort(host_counter* counter, int target) {
  int i;
  host_counter temp;
  for ( i = target; i >= 1; i-- ) {
    if ( counter[i].count > counter[i-1].count ) {
      temp = counter[i];
      counter[i] = counter[i-1];
      counter[i-1] = temp;
    }
  }
}

int date_compare(my_time time_a, my_time time_b) {
  int a_hash, b_hash;
  a_hash = time_a.year*12*31 + (time_a.month-1)*31 + (time_a.day-1);
  b_hash = time_b.year*12*31 + (time_b.month-1)*31 + (time_b.day-1);
  if ( a_hash < b_hash ) {
    return 1;
  }
  else if ( a_hash > b_hash ) {
    return -1;
  }
  else {
    return 0;
  }
}

#endif

