#ifndef MULTILOOKUP_H
#define MULTILOOKUP_H
#include "util.h"
#include "array.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define MAX_INPUT_FILES 100            // Maximum number of hostname file arguments allowed
#define MAX_REQUESTER_THREADS 10       //  Maximum number of concurrent requester threads allowed
#define MAX_RESOLVER_THREADS 10        // Maximum number of concurrent resolver threads allowed
#define MAX_IP_LENGTH INET6_ADDRSTRLEN //- INET6_ADDRSTRLEN is the maximum size IP address string util.c will return

typedef struct input{ //Type for requester
	array* hostnames;
	
	int file_count;
	int counter;
	int *file_index;

	FILE ** in_file; 			// Files with names to be read
	FILE * service_file; 		// Serviced.txt 
	int* requests_active; 		// num requests

	pthread_mutex_t* in_lock;   // mutex for serviced.txt
	pthread_mutex_t* arr_lock;  // mutex for shared memory
	pthread_mutex_t* std_lock;  // mutex for std
	pthread_mutex_t* fp_lock;   // mutex for file pointer


} input;

typedef struct output{ //Type for resolver
	array* hostnames;

	FILE * out_file; 			// Results.txt
	int* requests_active;

	pthread_mutex_t* out_lock; 	// mutex for results.txt
	pthread_mutex_t* arr_lock; 	// mutex for shared memory
	pthread_mutex_t* std_lock;  // mutex for shared memory

} output;

#endif