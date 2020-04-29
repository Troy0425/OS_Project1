#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage int my_printk(char *s){
    printk("%s\n",s);
    return 1;
}
