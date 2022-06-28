#ifndef STACK_H
#define STACK_H
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>

#define ARRAY_SIZE 8                       // max elements in stack
#define MAX_NAME_LENGTH 50

typedef struct {    
    pthread_mutex_t mutex;
    sem_t space_avail;
    sem_t items_avail;
    int i;
    int top;                                // array index indicating where the top is
    char* values[ARRAY_SIZE];
} array;

//int  array_print(array *s);                  // print
int  array_full(array *s);                   // check if full
int  array_empty(array *s);                  // check if empty
int  array_init(array *s);                   // initialize the array
int  array_put(array *s, char *hostname);   // place element into the array
int  array_get(array *s, char **hostname);  // remove element from the array

void array_free(array *s);                   // free the array's resources

#endif