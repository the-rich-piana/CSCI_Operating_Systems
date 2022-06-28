#ifndef ARRAY_H
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define ARRAY_H

#define ARRAY_SIZE 10

/*
Shared Array Buff        s->head = 0;
        s->tail = 0;er,

Should use LIFO, FIFO, or Circular Queue
Linear Array of Memory

Using Circular Queue
*/

typedef struct
{
    int head, tail, num_entries, size, check;
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