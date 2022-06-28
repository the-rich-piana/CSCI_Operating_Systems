#include "array.h"

sem_t mutex;
sem_t space_avail;
sem_t items_avail;

int  array_init(array *s){ 
    s->top = 0;
    s->i = 0;
    int x;
    char *g = "";
    for (x = 0; x < ARRAY_SIZE; x++){
        s->values[x] = malloc(sizeof(char) * MAX_NAME_LENGTH);
    }

    pthread_mutex_init(&(s->mutex), 0);   
    sem_init(&(s->space_avail),0, (ARRAY_SIZE));                  
    sem_init(&(s->items_avail),0, 0);                  

    return 0;
}

int  array_put(array *s, char *hostname){   // place element into the array
    //if (s->top >= ARRAY_SIZE) return -1;    // return failed if the stack is full
    if(strlen(hostname) > MAX_NAME_LENGTH){
        printf("String overflow \n");
        return -1;
    }
    
    sem_wait(&(s->space_avail));
        pthread_mutex_lock(&(s->mutex));
            s->top++;
            strncpy(s->values[s->top], hostname, MAX_NAME_LENGTH);
        pthread_mutex_unlock(&(s->mutex));
    sem_post(&(s->items_avail));

    return 0;
}

int array_get(array *s, char **hostname){  // remove element from the array
    //if (s->top < 1) return -1;              // return failed if the stack is empty
    sem_wait(&(s->items_avail));
        pthread_mutex_lock(&(s->mutex));
            strncpy(*hostname, s->values[s->top], MAX_NAME_LENGTH);
            s->top--;
        pthread_mutex_unlock(&(s->mutex));
    sem_post(&(s->space_avail));    

    return 0;
}

void array_free(array *s) {      

    int x;
    for (x = 0; x < ARRAY_SIZE; x++){
        free(s->values[x]);
    }    
    pthread_mutex_destroy(&(s->mutex));
    sem_destroy(&(s->space_avail));
    sem_destroy(&(s->items_avail));
    return;           // free the stack's resources
}

