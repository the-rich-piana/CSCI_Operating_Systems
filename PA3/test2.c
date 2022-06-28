#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2

void *writer (void * arg) {
    int i = 0;
    char *test = "PANCAKES";

    for(i=0; i < 10; i++){
        printf("Writing: %s \n", test);
        //array_put(arg, test);
    }
}

void *reader( void *arg) {
    int i = 0;
    char *test = "WAFFLES";
    char *g = "";
    for(i=0; i < 10; i++){
        //array_get(arg, &g);
        printf("Reading: %s \n", g);
    }
}    

int main() {
    array my_array;
    char *hostnames[9] = {"facebook.com","youtube.com","yahoo.com","live.com","wikipedia.org","msn.com","blogspot.com","microsoft.com","qq.com"};
    //char ** is a pointer to an array of strings
    int i = 0;
    char *g;
    char *p;
    //char **test;
    //test = hostnames;
    //printf("double pointer %s", **test);


    if (array_init(&my_array) < 0) exit(-1);  // init array, exit if failed
    
    //array_get(&my_array, g);

    array_put(&my_array, hostnames[0]);
    array_put(&my_array, hostnames[1]);
    array_put(&my_array, hostnames[2]);
    array_put(&my_array, hostnames[3]);

    array_get(&my_array, &p);
    printf(" Hostname: %s ", p); // pop and print until empty

    array_get(&my_array, &p);

    printf(" Hostname: %s ", p); // pop and print until empty
    array_get(&my_array, &p);
    

    printf(" Hostname: %s ", p); // pop and print until empty

    exit(0);    
}