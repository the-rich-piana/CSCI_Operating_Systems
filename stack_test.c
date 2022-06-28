#include "array.h"

#define NUM_THREADS 2

void *writer (void * arg) {

    int i;
    char a[10] = "TEST";
    char *hostnames[9] = {"facebook.com","youtube.com","yahoo.com","live.com","wikipedia.org","msn.com","blogspot.com","microsoft.com","qq.com"};

    while(i < 50){
        printf("Writing: %s %d\n", hostnames[i % 9], i);
        array_put(arg, hostnames[i % 9]);
        i++;
    }
    return 0;
}

void *reader( void *arg) {
    int i;
    char *g = malloc(sizeof(char) * MAX_NAME_LENGTH);

    while(i < 50){
        array_get(arg, &g);
        printf("Reading: %s \n", g);
        i++;
    }
    return 0;
}    

int main() {
    array my_array;
    char *hostnames[9] = {"facebook.com","youtube.com","yahoo.com","live.com","wikipedia.org","msn.com","blogspot.com","microsoft.com","qq.com"};
    //char ** is a pointer to an array of strings
    int i = 0;
    char *g;
    char *p;

    if (array_init(&my_array) < 0) exit(-1);  // init array, exit if failed

    pthread_t prod_tid, consumer_tid;

    pthread_create(&prod_tid, NULL, writer, &my_array);
    pthread_create(&consumer_tid, NULL, reader, &my_array);

    pthread_join(prod_tid, NULL);
    pthread_join(consumer_tid, NULL);
    
    array_free(&my_array);
    //array_get(&my_array, g);

    // array_put(&my_array, hostnames[1]);
    // array_put(&my_array, hostnames[4]);
    // array_put(&my_array, hostnames[6]);

    // array_get(&my_array, &p);
    // array_get(&my_array, &p);
    // array_get(&my_array, &p);
    

    // printf(" Hostname: %s ", p); // pop and print until empty
    exit(0);
}