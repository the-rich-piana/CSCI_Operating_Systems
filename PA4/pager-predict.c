/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

//Page in policy
void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    
    /* Local vars */
    int procCurr;
    int pageCurr;

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
	    initialized = 1;
    }
    
    /* TODO: Implement Predictive Paging */
    /*For each process */

    //int pages_in_mem = 0;
    for(procCurr = 0; procCurr < MAXPROCESSES; procCurr++ ){        
        /* Inactive */
        if (!q[procCurr].active) continue;

        pageCurr = q[procCurr].pc / PAGESIZE; //curr page

        /* ticks / 128 */
        // 1. 2. Page in current page and next
        // 3. Evict previous
        
        //Page CURR
        if(!(q[procCurr].pages[pageCurr])) 
            pagein(procCurr, pageCurr);
        
        //Page CURR + 1 
        if((pageCurr < MAXPROCPAGES - 1)  && !(q[procCurr].pages[pageCurr + 1])) //Less than 20
                //Page in +1
                pagein(procCurr, pageCurr + 1);          
        
        //Evict CURR - 1 (> 0)
        if(pageCurr != 0 && (q[procCurr].pages[pageCurr - 1]))
                pageout(procCurr, pageCurr - 1);

        /*
        PATTERNS
            Program 1: Page 11
                Branch: 500 to 1402, 1401 to 1533, 1533 to 0
                Ends 1534
            Program 2: Page 8
                Branch: 1129 to 0
                Ends 1130
            Program 3: Page 13
                Branch: 1682 to 1166, 1683 to 0
                Ends 1684
            Program 4: Page 14
                No Branch.
                Ends 1911
            Program 5: Page 3
                Branch: 500 to 503, 501 to 0, 502 to 503, 503 to 0
                Ends 504
        */

       //EVICT front if pc = 0, akin to resetting
        if(pageCurr == 0) { //If first
            for(int i = 2; i < MAXPROCPAGES; i ++)  //+2 from already adding
                if(q[procCurr].pages[i]){           //pageout any in front
                    pageout(procCurr, i);
                }
        }

        /* Classification */
        //P1
        //500 = 3
        //1402 = 10
        //1401 = 10
        //1535 = 11
        //0 = 0

        else if(pageCurr == 3) {
            if(!q[procCurr].pages[0]) pagein(procCurr, 0);
            if(!q[procCurr].pages[4]) pagein(procCurr, 4);
            if(!q[procCurr].pages[10]) pagein(procCurr, 10);
            if(!q[procCurr].pages[11]) pagein(procCurr, 11);
        }
        // else if(pageCurr == 3) {   
        //     if(!q[procCurr].pages[0]) pagein(procCurr, 0);
        //     if(!q[procCurr].pages[10]) pagein(procCurr, 10);
        //     if(!q[procCurr].pages[11]) pagein(procCurr, 11);
        // }


        //P1, page 4, branch evict 10 and 11
        else if(pageCurr == 4) {     
            if(q[procCurr].pages[10]) pageout(procCurr, 10);
            if(q[procCurr].pages[11]) pageout(procCurr, 11);
        }

        //P2 
        //1129 = 8
        //1130 = 8
        //0 = 0
        else if(pageCurr == 8) {   
            if(!q[procCurr].pages[0]) pagein(procCurr, 0);
        }

        //P3
        //1166 = 9
        //Pageout 13 & 14
        else if(pageCurr == 9) { //if at 1166
            //evict 1166 + 1 + 2
            if(q[procCurr].pages[11]) pageout(procCurr, 11);
            if(q[procCurr].pages[12]) pageout(procCurr, 12);
            if(q[procCurr].pages[13]) pageout(procCurr, 13);
            if(q[procCurr].pages[14]) pageout(procCurr, 14);
        }
        //P4
        else if(pageCurr == 10) {  
            if(q[procCurr].pages[3]) pageout(procCurr, 3);
            if(q[procCurr].pages[4]) pageout(procCurr, 4);
        }
        //

        else if(pageCurr == 11) {
            if(!q[procCurr].pages[0]) pagein(procCurr, 0);
        }

        //Before Page 13 (P3) & Page 14 (P4)
        // else if(pageCurr == 12) { 
        //     if(!q[procCurr].pages[0]) pagein(procCurr, 0);
        //     if(!q[procCurr].pages[9]) pagein(procCurr, 9);
        // } 
    }

    /* advance time for next pageit iteration */
    tick++;
}