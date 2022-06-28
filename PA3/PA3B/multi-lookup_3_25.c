    #include "multi-lookup.h"

    void * requester( void * arg){ //places each line on the hostnames array
        //printf("IN REQUESTER \n");
        input * t = arg;

        FILE *service = t->service_file;
        int file_counter = 0;

        size_t len = 0;
        char * hostname =  NULL;
                
        FILE *fp = t->in_file; 
        
        pthread_mutex_lock(t->in_lock);             //lock serviced
            fprintf(service, "Thread %lu done with %d.txt\n", pthread_self(), t->file_num);
        pthread_mutex_unlock(t->in_lock);           //unlock serviced
        
        while(getline(&hostname, &len, fp) != -1){
            hostname[strcspn(hostname,  "\n")] = 0;
            array_put(t->hostnames, hostname); 
                    //fprintf(service, "%s\n", hostname);   //output file
        }

        file_counter = file_counter + 1;
        printf("Thread %lu serviced %d files \n", pthread_self(), file_counter);  

        return 0;
    }

    void * resolver( void * arg){ //resolves hostnames from text in the requester thread pool
        //printf("IN RESOLVER \n");

        output * t = arg;
        FILE * results = t->out_file;
        char *ip = malloc(sizeof(char) * MAX_IP_LENGTH);// [MAX_IP_LENGTH];
        char *hostname = malloc(sizeof(char) * MAX_NAME_LENGTH);
        int dns = 0;
        int count = 0;


        while(1){
            array_get(t->hostnames, &hostname); //add hostname
            //printf("Array popped: %s \n", hostname);
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
        printf("thread %lu resolved %d hostnames\n", pthread_self(), count);
        return 0;
    }

    int main(int argc, char* argv[]){

        clock_t start, end;
        double cpu_time_used;
        
        start = clock();

        array arr_hostnames;

        int num_files = argc - 5;
        int num_req = atoi(argv[1]);
        int num_res = atoi(argv[2]);

        input inputs[num_req];        // array of requester structs
        output outputs[num_res];      // array of resolvers structs

        //printf("Num files %d", num_files);
        FILE * fname_array[num_files];

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

        FILE* serviced = fopen(argv[3],"w"); //Log of file names read by requester
        
        FILE* results = fopen(argv[4],"w"); //Log file containing resulting dnslookups

        /* Mutex Locks */
    //    pthread_mutex_t array_mutex;
    //    pthread_mutex_t results_mutex;

        pthread_mutex_t service_lock; 
        pthread_mutex_init(&service_lock, NULL);

        pthread_mutex_t results_lock; 
        pthread_mutex_init(&results_lock, NULL);

        pthread_mutex_t hostnames_lock; 
        pthread_mutex_init(&hostnames_lock, NULL);    

        pthread_mutex_t std_lock; 
        pthread_mutex_init(&std_lock, NULL);    

        /////////////////

        /* Structs */
        
        pthread_t request_tid[num_req], resolve_tid[num_res];
        

        int file_index = 0;

        int r = 0;
        while(file_index < num_files){
            for(r = 0; r < num_req; r = r+1){
                if(file_index < num_files){
                    //printf("WORKING ON FILE %d \n", file_index);                
                    FILE * curr = fname_array[file_index];
                    inputs[r].hostnames        = &arr_hostnames;
                    inputs[r].service_file     = serviced;
                    inputs[r].arr_lock         = &hostnames_lock;
                    inputs[r].in_file          = curr;
                    inputs[r].in_lock          = &service_lock;
                    inputs[r].file_num         = file_index + 1;
                    file_index = file_index + 1;

                    if(pthread_create(&request_tid[r], NULL, requester, (void*)&inputs[r])){
                        fprintf(stderr, "Failed creating requester thread \n"); 
                        return EXIT_FAILURE;
                    }
                }
            }
            for(r = 0; r < num_req; r = r +1){
                if(request_tid[r]){
                    pthread_join(request_tid[r], 0);
                }
            }        
        }

        for(int i = 0; i < num_res; i = i + 1){
            outputs[i].hostnames        = &arr_hostnames;
            outputs[i].out_file         = results;
            outputs[i].out_lock         = &results_lock;
            //outputs[i].requests_active  = &valid_requests;

            if(pthread_create(&resolve_tid[i], NULL, resolver,(void*)&outputs[i])){
                fprintf(stderr, "Failed creating resolver thread \n"); 
                return EXIT_FAILURE;
            }
        }

        //** END ROUTINE **//
        for(int i = 0; i < num_req; i = i +1){
            pthread_join(request_tid[i], 0);
        }
        for(int i = 0; i < num_res; i = i +1){
            pthread_join(resolve_tid[i], 0);
        }    
        for(int i = 0; i < num_files; i = i + 1){
            fclose(fname_array[i]);
        }

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("./multi-lookup: total time is %f seconds \n", (cpu_time_used));

        array_free(&arr_hostnames);
        exit(0);
    }