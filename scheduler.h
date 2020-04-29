#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_

#include "process.h"

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4
int cal_next_proc(struct process *proc, int num_proc, int policy);
int schedule(struct process *proc, int num_proc, int policy);
#endif
