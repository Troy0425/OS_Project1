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
static int queue[25];
static int front;
static int back;
void push(int i){
    queue[back] = i;
    back = (back+1) % 25;
}
void pop(){
    front = (front+1) % 25;
}
int top(){
    return queue[front];
}
int empty(){
    return (front == back);
}
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
            if(!empty()){
                next = top();
                pop();
            }
        }else if((now_time - last_time) % 500 == 0){
            if(empty())
                next = running_proc;
            else{
                next = top();
                pop();
                push(running_proc);
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
	if(policy == RR){
        front = 0;
        back = 0;
    }
	for(int i = 0; i < num_proc; i++){
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
                if(policy == RR)
                    push(i);
            }
        }
        int next_proc = cal_next_proc(proc, num_proc, policy);
        if(next_proc != -1){
            if(next_proc != running_proc){
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
