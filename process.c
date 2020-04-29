#define _GNU_SOURCE
#include "process.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define my_get_time 333
#define my_printk 334
void assign_cpu(int pid, int cpu){
    cpu_set_t assign;
	CPU_ZERO(&assign);
	CPU_SET(cpu, &assign);
    int ret = sched_setaffinity(pid, sizeof(assign), &assign);
	if(ret == -1)
		perror("assign_cpu");
    return;
}
void set_high(int pid){
    struct sched_param param;
	param.sched_priority = 0;
    int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
	if(ret == -1)
		perror("set_high");
    return;
}
void set_low(int pid){
    struct sched_param param;
	param.sched_priority = 0;
    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
	if(ret == -1)
		perror("set_low");
    return;
}
int create_proc(struct process *proc){
    int pid = fork();
    if(pid == 0){
		printf("%s %d\n", proc->name, getpid());
        unsigned long start_sec, start_nsec, end_sec, end_nsec;
		syscall(my_get_time, &start_sec, &start_nsec);
		char dmesg_buffer[70];
		for(int i = 0; i < proc->e_time; i++){
			//if(i % 100 == 0){
			//	printf("%s i = %d\n", proc->name, i);
			//}				
			unit_time();
		}
		syscall(my_get_time, &end_sec, &end_nsec);
		sprintf(dmesg_buffer, "[Project 1] %d %010lu.%09lu %010lu.%09lu", getpid(), start_sec, start_nsec, end_sec, end_nsec);
		syscall(my_printk, dmesg_buffer);
		printf("%s used time %lf\n", proc->name, (end_sec + end_nsec*1e-9) - (start_sec + start_nsec*1e-9));
		exit(0);
    }else{
        assign_cpu(pid, proc_cpu);
        return pid;
    }
}
