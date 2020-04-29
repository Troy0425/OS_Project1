#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "process.h"
#include "scheduler.h"

int main(int argc, char* argv[]){
    char policy_buffer[32];
    int num_proc;
    scanf("%s", policy_buffer);
    scanf("%d", &num_proc);
    struct process *proc = (struct process*)malloc(num_proc * sizeof(struct process));
    for(int i = 0; i < num_proc; i++){
        scanf("%s", proc[i].name);
        scanf("%d%d", &proc[i].r_time, &proc[i].e_time);
    }
    if(strcmp(policy_buffer, "FIFO") == 0){
       schedule(proc, num_proc, FIFO); 
    }else if(strcmp(policy_buffer, "RR") == 0){
       schedule(proc, num_proc, RR); 
    }else if(strcmp(policy_buffer, "SJF") == 0){
       schedule(proc, num_proc, SJF); 
    }else if(strcmp(policy_buffer, "PSJF") == 0){
       schedule(proc, num_proc, PSJF); 
    }
    return 0;
}
