#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

#define scheduler_cpu 0
#define proc_cpu 1

/* Running one unit time */
#define unit_time(){volatile unsigned long i; for (i = 0; i < 1000000UL; i++);}

struct process{
    char name[32];
    int r_time;
    int e_time;
	int s_time;
    pid_t pid;
};
void assign_cpu(int pid, int cpu);
void set_high(int pid);
void set_low(int pid);
int create_proc(struct process *proc);
#endif
