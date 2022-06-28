/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */




//Eviction policy
#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES]; // 20 processes * 20 shared pages
    /*
    Frame # PC#  0 1 2 3 ... 20
    1   |   . 
    2   |   .
    ... |   .
    98  |   .
    99  |   .
    100 |   .
   */

    /* Local vars */
    int proctmp; //index of 1  of 20 processes
    int pagetmp; //to be paged
    
    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		    timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
    }

    int currPage;    //Current Page
    int lruPage;    //Least Recently Used Page
    int lru;        //Minimum timestamp
    
    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++ ){ //for 20 processes
        if(!q[proctmp].active) continue;                            //if it's active
            currPage = q[proctmp].pc / PAGESIZE;

            //printf("Current Page: %i", currPage);
            
            timestamps[proctmp][pagetmp] = tick; 
            if(!q[proctmp].pages[currPage]){ 
            /*If current page not paged in */

                    /* LRU */    
                    for(int i = 0; i < MAXPROCPAGES; i++){
                        /* Exists */
                        if(q[proctmp].pages[i]){ 
                            /* if less than current lru */
                            if( timestamps[proctmp][i] < lru ){
                                lru = timestamps[proctmp][i];
                                lruPage = i;
                                //printf("LRU: %i \n", lru);
                            }
                        }
                        /* Free */
                        /*If empty, skip */
                        else if(!(q[proctmp].pages[pagetmp])) {
                            continue;
                        }
                    }
                if(!pagein(proctmp, currPage)){ //Frame full
                    lru = tick;                 //Set lru to current timestamp
                    /* Fail to page in, evict LRU */
                    if(!pageout(proctmp, lruPage)){
                        printf("Page Eviction Failed\n");
                    }
                }
                //printf("Timestamp: %i\n", timestamps[proctmp][currPage]);
    
            }
    }

    tick++;
} 
