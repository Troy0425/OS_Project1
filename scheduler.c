#define _GNU_SOURCE
#include "process.h"
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
static int now_time;
static int running_proc;
static int num_finish;
static int last_time;
int cal_next_proc(struct process *proc, int num_proc, int policy){
    if(policy == FIFO || policy == SJF){ //non-preemptive    
        if(running_proc != -1)
            return running_proc;
    }
    int next = -1;
    if(policy == SJF || policy == PSJF){
        for(int i = 0; i < num_proc; i++){
            if(proc[i].pid != -1 && proc[i].e_time != 0){
                if(next == -1 || proc[i].e_time < proc[next].e_time)
                    next = i;
            }
        }
    }else if(policy == FIFO){
        for(int i = 0; i < num_proc; i++){
            if(proc[i].pid != -1 && proc[i].e_time != 0){
                next = i;
                break;
            }
        }
    }else if(policy == RR){
        if(running_proc == -1){
			for(int i = 0; i < num_proc; i++){
				if(proc[i].pid != -1 && proc[i].e_time != 0){
					next = i;
					break;
				} 
			}
        }else if((now_time - last_time) % 500 == 0){
            next = running_proc;
            for(int i = 1; i < num_proc; i++){
                int tmp = (running_proc + i) % num_proc;
                if(proc[tmp].pid != -1 && proc[tmp].e_time != 0){
                    next = tmp;
                    break;
                }
            }
        }else{
            next = running_proc;
        }
    }
    return next;
}
int schedule(struct process *proc, int num_proc, int policy){
    assign_cpu(getpid(), scheduler_cpu);
    set_high(getpid());
	
	for(int i = 0; i < num_proc; i++){
		proc[i].s_time = -1;
		proc[i].pid = -1;
	}
    now_time = 0;
    running_proc = -1;
    num_finish = 0;
    last_time = 0;
    while(num_finish < num_proc){
        for(int i = 0; i < num_proc; i++){
            if(proc[i].r_time == now_time){
                proc[i].pid = create_proc(&proc[i]);
                set_low(proc[i].pid);
            }
        }
        int next_proc = cal_next_proc(proc, num_proc, policy);
        if(next_proc != -1){
            if(next_proc != running_proc){
				if(proc[next_proc].s_time == -1)
					proc[next_proc].s_time = now_time;
                set_high(proc[next_proc].pid);
				if(running_proc != -1)
               		set_low(proc[running_proc].pid);
                running_proc = next_proc;
                last_time = now_time;
            }
        }
        unit_time();
        if(running_proc != -1){
            proc[running_proc].e_time--;
            if(proc[running_proc].e_time == 0){
				printf("%s theorical used time %d\n", proc[running_proc].name, now_time - proc[running_proc].s_time +1);
                waitpid(proc[running_proc].pid, NULL, 0);
                running_proc = -1;
                num_finish++;
                if(num_finish == num_proc)
                    break;
            }
        }
        now_time++;
    }
	return 0;
}
