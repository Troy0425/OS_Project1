#include <linux/module.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/kernel.h>
asmlinkage int my_get_time(unsigned long *sec, unsigned long *nsec){
	struct timespec nowtime;
	getnstimeofday(&nowtime);
	*sec = nowtime.tv_sec;
	*nsec = nowtime.tv_nsec;
    return 1;
}
