#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include<linux/string.h>
#include<linux/kprobes.h>
#include<linux/kallsyms.h>
#include<linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>

//#include <linux/dynamic_dump_stack.h>


//Head of the linked list of all the kprobes -- dummy head
struct list_head ds_list_head = LIST_HEAD_INIT(ds_list_head);
int dumpeid; 

struct dumpmode_t {
	unsigned int mode;
};

struct dump_stack_struct *node;
struct ds{

	int dumpeid; 
	char symbolname[50];
	struct kprobe kp;
	pid_t pid;   
	pid_t tgid;  
	int dumpmode;  
	struct list_head l;

};


struct dn {
	struct list_head ln;
	struct list_head* add;
};


int remove_kp(pid_t pid) {


    struct list_head* i = NULL;
    struct ds* struct_iter = NULL;

    struct dn *dList, *temp = NULL;
    struct list_head nodes_to_delete = LIST_HEAD_INIT(nodes_to_delete);

    list_for_each(i, &ds_list_head) {
	struct_iter = list_entry(i, struct ds, l);
	if(struct_iter->pid == pid) {
		printk("\nRemoving the follwing dumpid:%d\n",struct_iter->dumpeid);
		unregister_kprobe(&struct_iter->kp);  
		dList = kmalloc(sizeof(struct dn), GFP_KERNEL);
		memset(dList, 0, sizeof(struct dn));
		dList->add = i;
		list_add(&dList->ln, &nodes_to_delete);
	}
    }


   list_for_each(i, &nodes_to_delete) {
	temp = list_entry(i, struct dn, ln);
	list_del(temp->add);
   }


    return 1;
}



int handler_pre(struct kprobe *probe, struct pt_regs *regs)
{


	struct ds *temp;
	struct task_struct* t = current;
	pid_t tgid;
	pid_t pid;
	tgid = t->tgid;
	pid = t->pid;

	temp = container_of(probe, struct ds, kp);

	if(temp->dumpmode > 1){
		printk("\nInside >1");
		printk("\nCurrent pid:%d\nActual pid:%d\nCurrent tgid:%d\nActual tgid:%d",pid,temp->pid,tgid,temp->tgid);
		dump_stack();
		return 0;
	}

	else if(temp->dumpmode == 0 && pid == temp->pid){
		printk("\nInside 0");
		printk("\nCurrent pid:%d\nActual pid:%d\nCurrent tgid:%d\nActual tgid:%d",pid,temp->pid,tgid,temp->tgid);
		dump_stack();
		return 0;
	}

	else if(temp->dumpmode == 1 && (pid == temp->pid || tgid == temp->tgid)){
		printk("\nInside 1");
		printk("\nCurrent pid:%d\nActual pid:%d\nCurrent tgid:%d\nActual tgid:%d",pid,temp->pid,tgid,temp->tgid);
		dump_stack();
		return 0;
	}


	printk("\nCannot do dump_stack()");
	printk("\nCurrent pid:%d\nActual pid:%d\nCurrent tgid:%d\nActual tgid:%d",pid,temp->pid,tgid,temp->tgid);

	return 0;
}



SYSCALL_DEFINE1(rmdump, int, dumpeid)
{

#ifdef CONFIG_DYNAMIC_DUMP_STACK

	struct list_head* i = NULL;
	struct ds* temp = NULL;
	struct task_struct* task = current;
	pid_t pid = task->pid;

	printk("\nInside the rmdump definition");

	list_for_each(i, &ds_list_head) {
		temp = list_entry(i, struct ds, l);
		if(temp->dumpeid == dumpeid && temp->pid == pid) 
		{
			printk("\nRemoving kprobe of this dumpid:%d",temp->dumpeid);
			unregister_kprobe(&temp->kp);
			list_del(i);
			return 1;
		}
	}

	printk("\nCannot remove probe with dumpid:%d",dumpeid);

	return -EINVAL;
#else
	return 0;
#endif

}



SYSCALL_DEFINE2(insdump,const char __user *, symbolname, struct dumpmode_t __user *, dumpmode)
{

#ifdef CONFIG_DYNAMIC_DUMP_STACK
	unsigned long add;	
	struct ds *d;
	char * x;
	struct dumpmode_t i;
	struct task_struct* t;
	pid_t tgid;
	pid_t pid;


	t = current;
	tgid = t->tgid;
	pid = t->pid;

	printk("\nInside the insdump definition");

	//symbolname from the user
	x = kmalloc(sizeof(char)*50, GFP_KERNEL);
	strncpy_from_user((char *)x,symbolname, 50);

	//per kprobe structure
	d = (struct ds *)kmalloc(sizeof(struct ds), GFP_KERNEL);
	memset(d, 0, sizeof(struct ds));


	d->pid = pid;
	d->tgid = tgid;

	//symbolname assigned to the structure
	sprintf(d->symbolname,"%s",x);

	//address for the symbol
	add = kallsyms_lookup_name(d->symbolname);

	//the dumpmode from user
	if (copy_from_user(&i, dumpmode,sizeof(i))){
		return -EFAULT;
	}
	d->dumpmode = i.mode;

	if(add == 0)
	{
		printk(KERN_INFO "SYMBOL NOT FOUND\n");
		return -EINVAL;
	}
	memset(&d->kp, 0, sizeof(struct kprobe));	
	d->kp.pre_handler = handler_pre;
	d->kp.addr = (kprobe_opcode_t *)add;

	if(register_kprobe(&d->kp)){
		printk(KERN_INFO "Error while setting kprobe on address %p\n", (void*)(add));
		return -EINVAL;
	}

	//Incrementing my dumpid
	d->dumpeid = dumpeid++;

	list_add(&d->l, &ds_list_head);
	printk("\nthe new kprobe is added in the list\n");

	return d->dumpeid;
#else
	return 0;
#endif

}

