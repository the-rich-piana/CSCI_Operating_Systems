#include "array.h"
//anything that is not zero, is true

int array_full(array *s){
    if ((s->tail+1 == s->head) || (s->head == 0 && s->tail == ARRAY_SIZE -1)){  //if head is one away from tail, if tail is at the size and head is still at 0
        printf("array FULL\n");
        return 1;
    } 
    else {
        return 0;
    }    
}

int array_empty(array *s){ //if equal
    if( s->head == s->tail){
        return 1;
    }
    else return 0;
}

int array_init(array *s) {                  // init the array
    s->size = ARRAY_SIZE;                   //size
    s->num_entries = 0;                     //empty 
    s->head = 0; 
    s->tail = 0;
    return 0;
}

int array_put(array *s, char *hostname) {     // place element on the top of the array
    
    //pass a string
    if(array_full(s)){  // if array is full return failure
        printf("Array full. can't add \n");
        return -1;                              
    }
    if(array_empty(s)){ //if empty, head and tail at 0, copy
        printf("Array Empty, adding \n ");
    	strcpy(&s->values[0], hostname);
    }

    s->tail = (s->tail++ ) % s->size; //move tail pointer 
    printf("Tail: %d ", s->tail);
    //strcpy(s->values[s->tail], hostname);

    //printf("PUT : %s \n", s->values[s->tail]);

    s->num_entries++;    //increment size

    return 0;
}

int array_get(array *s, char **hostname) {     // remove element from the top of the array
    char **result;
    //take a pointer to an array, edit that pointer
    if (array_empty(s)){ // return failed if the array is empty
        printf("ARRAY EMPTY \n");
        return -1;
    }

    result = s->values[s->head];
    printf("GET: %s\n", s->values[s->head]);
    s->head = (s->head + 1) % s->size; //move head pointer 
    s->num_entries--;
    hostname = result;
    return 0;
}

void array_free(array *s) {    
    char *remove;             // free the array's resources
    while(!array_empty(s)){
        array_get(s, remove);
    }
    free(remove);
    free(s->values);

}