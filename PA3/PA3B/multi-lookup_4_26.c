#include "multi-lookup.h"


/*
int getfile counter
    pthread_mutex_lock  
    pthread_mutex_unlock


*/

int * getfilecounter(int *file_index, int file_count) {

    //if less
    if(*file_index < file_count){}
    //if greater

    return 1;
}

void * requester( void * arg){ //places each line on the hostnames array

    input * t = arg;
    FILE *service = t->service_file;
    FILE **fp_array = t->in_file;
    FILE * fp = NULL;

    int file_counter = 0; //# of serviced files

    //size_t len = 0;
    char *hostname = malloc(sizeof(char) * MAX_NAME_LENGTH);        

    while(getfilecounter(*t->file_index, t->file_count)){ //while counter is less than total num of files

    while(*t->file_index < t->file_count){ //while counter is less than total num of files
        if( still less then){
            break;
        }
        //pthread_mutex_lock(t->std_lock);             //lock serviced
        //    printf("Thread %lu is LOCKING  %d INDEX  \n", pthread_self(), *t->file_index);  
        //pthread_mutex_unlock(t->std_lock);  
        
        pthread_mutex_lock(t->fp_lock);
        fp = fp_array[*t->file_index];
        *t->file_index = (*t->file_index + 1); //% (t->file_count);
        pthread_mutex_unlock(t->fp_lock);
       

        pthread_mutex_lock(t->arr_lock);
        while(fscanf(fp, "%1024s", hostname) > 0 ){
            array_put(t->hostnames, hostname); 
        }
        pthread_mutex_unlock(t->arr_lock);
                
       // *t->file_index = *t->file_index + 1; //% (t->file_count);

        pthread_mutex_lock(t->in_lock);             //lock serviced
            fprintf(service, "Thread %lu done with %d.txt\n", pthread_self(), *t->file_index);
        pthread_mutex_unlock(t->in_lock);           //unlock serviced
        
        file_counter++;
    } 
    
    pthread_mutex_lock(t->std_lock);             //lock serviced
        printf("Thread %lu serviced %d files \n", pthread_self(), file_counter);  
    pthread_mutex_unlock(t->std_lock);           //unlock serviced
    
    free(hostname);
    return 0;
}

void * resolver( void * arg){ //resolves hostnames from text in the requester thread pool
    output * t = arg;
       // pthread_mutex_lock(t->std_lock);             //lock serviced
       //     printf("Thread %lu IN RESOLVER  \n", pthread_self());  
       // pthread_mutex_unlock(t->std_lock);  
    FILE * results = t->out_file;
    char *ip = malloc(sizeof(char) * MAX_IP_LENGTH);// [MAX_IP_LENGTH];
    char *hostname = malloc(sizeof(char) * MAX_NAME_LENGTH);
    int dns = 0;
    int count = 0;
    
    //while( requests are active (0) and array is not empty (0))
    //printf("REQUESTS ACtIVE %d \n", array_empty(t->hostnames));
    while((*t->requests_active) && !( array_empty(t->hostnames))){
        //pthread_mutex_lock(t->arr_lock);        
        array_get(t->hostnames, &hostname); //get hostname
        //pthread_mutex_unlock(t->arr_lock);

        dns = dnslookup(hostname, ip, MAX_IP_LENGTH);
        if(dns == -1){
           // printf("COULD NOT RESOLVE: %s \n", hostname);
            pthread_mutex_lock(t->out_lock);
                fprintf(results, "%s, %s\n", hostname, "NOT_RESOLVED");
                count++;
            pthread_mutex_unlock(t->out_lock);

        }else{        
           // printf("DNS resolved: %s \n", ip);
            pthread_mutex_lock(t->out_lock);
                fprintf(results, "%s, %s\n", hostname, ip);
                count++;
            pthread_mutex_unlock(t->out_lock);

        }
    }
    //printf("Resolved host: %s", ip);
    pthread_mutex_lock(t->std_lock);
    printf("thread %lu resolved %d hostnames\n", pthread_self(), count);
    pthread_mutex_unlock(t->std_lock);

    free(hostname);
    free(ip);
    return 0;
}

int main(int argc, char* argv[]){

    if (argc < 6) {
        fprintf(stderr, "Incorrect Number of Arguments \n");
        return -1;
    }

    clock_t start, end;
    double cpu_time_used; 
    start = clock();

    int num_files = argc - 5;
    int num_req = atoi(argv[1]);
    int num_res = atoi(argv[2]);

    if(num_files > MAX_INPUT_FILES){
        fprintf(stderr, "Too many input files, max: 100 \n");
        return -1;
    }
    if(num_req > MAX_REQUESTER_THREADS){
        fprintf(stderr, "Too many requester threads \n");
        return -1;
    }    
    if(num_res > MAX_RESOLVER_THREADS){
        fprintf(stderr, "Too many resolver threads \n");
        return -1;
    }        

    input inputs[num_req];        // array of requester structs
    output outputs[num_res];      // array of resolvers structs

    //printf("Num files %d", num_files);
    FILE * fname_array[num_files];
    FILE* serviced = fopen(argv[3],"w"); //Log of file names read by requester
    FILE* results = fopen(argv[4],"w"); //Log file containing resulting dnslookups

    array arr_hostnames;
    if (array_init(&arr_hostnames) < 0) exit(-1);  // init array, exit if failed
    
    for(int i = 0; i < num_files; i = i + 1){
        if(fopen(argv[i+5], "a") == NULL){
           fprintf(stderr, "invalid file %s, skipping\n", argv[i+5]); 
           num_files = num_files - 1;
           continue;
        }else{        
            fname_array[i] = fopen(argv[i+5], "r");
            //printf("%d \n",fname_array[i]);
        }
    }

    /* Mutex Locks */

    pthread_mutex_t service_lock; 
    pthread_mutex_init(&service_lock, NULL);

    pthread_mutex_t results_lock; 
    pthread_mutex_init(&results_lock, NULL);

    pthread_mutex_t std_lock; 
    pthread_mutex_init(&std_lock, NULL);    

    pthread_mutex_t arr_lock; 
    pthread_mutex_init(&arr_lock, NULL);

    pthread_mutex_t get_lock; 
    pthread_mutex_init(&get_lock, NULL);

    pthread_mutex_t fp_lock; 
    pthread_mutex_init(&fp_lock, NULL);    

    /////////////////

    /* Structs */
    pthread_t request_tid[num_req], resolve_tid[num_res];

    int requests_active = 0;
    int file_index = 0;
    /* Write a loop to test 1 resolver/ 1rq */


    for(int i = 0; i < num_req; i = i+1){ //keep making threads, (under the number of total req)
        //if(file_index < num_files){
        inputs[i].hostnames        = &arr_hostnames;
        inputs[i].service_file     = serviced;
        inputs[i].counter          = 0;
        inputs[i].file_index       = &file_index;
        inputs[i].file_count       = num_files;
        inputs[i].in_file          = fname_array; //
        inputs[i].in_lock          = &service_lock;
        inputs[i].std_lock         = &std_lock;
        inputs[i].arr_lock         = &arr_lock;
        inputs[i].fp_lock          = &fp_lock;

        //file_index = file_index + 1;
        if(pthread_create(&request_tid[i], NULL, requester, (void*)&inputs[i])){
            fprintf(stderr, "Failed creating requester thread \n"); 
                return EXIT_FAILURE;
        }
    }
    
    requests_active = 1; // Kill resolvers, requesters done.

    for(int i = 0; i < num_res; i = i + 1){
        outputs[i].hostnames        = &arr_hostnames;
        outputs[i].out_file         = results; 
        outputs[i].out_lock         = &results_lock;    // results.txt
        outputs[i].std_lock         = &std_lock;        // stderr
        outputs[i].arr_lock         = &get_lock;        // array lock
        outputs[i].requests_active  = &requests_active;  //kill switch

        //outputs[i].requests_active  = &valid_requests;

        if(pthread_create(&resolve_tid[i], NULL, resolver,(void*)&outputs[i])){
            fprintf(stderr, "Failed creating resolver thread \n"); 
            return EXIT_FAILURE;
        }
    }
    

    //** END ROUTINE **//

    for(int i = 0; i < num_req; i = i + 1){
        if(request_tid[i]){
            pthread_join(request_tid[i], NULL);
        }
        else{
           // break;
        }
    }    
    for(int i = 0; i < num_res; i = i +1){
        if(resolve_tid[i]){
            pthread_join(resolve_tid[i], NULL);
        }
        else{
           // break;
        }
    }    
    for(int i = 0; i < num_files; i = i + 1){
        fclose(fname_array[i]);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("./multi-lookup: total time is %f seconds \n", (cpu_time_used));

    array_free(&arr_hostnames);

    pthread_mutex_destroy(&results_lock);
    pthread_mutex_destroy(&service_lock);
    pthread_mutex_destroy(&arr_lock);
    pthread_mutex_destroy(&get_lock);
    pthread_mutex_destroy(&fp_lock);
    pthread_mutex_destroy(&std_lock);

    exit(0);
}