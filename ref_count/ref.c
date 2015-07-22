/*
* refcont function 
*20150720
* http://blog.jobbole.com/88279/
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>//v/kmalloc v/kree
#include <linux/kthread.h> //kthread

struct my_obj{
	int val;
	struct kref refcnt;
};

struct my_obj *obj;

void obj_release(struct kref *ref)
{
	struct my_obj *obj = container_of(ref, struct my_obj, refcnt);
	printk(KERN_INFO "obj_release\n");
	kfree(obj);
}

void more_data_handling(void *cb_data)
{
	struct my_obj *obj = cb_data;
	printk(KERN_INFO "more_data_handling\n");

	kref_put(&obj->refcnt,obj_release);
}

static int __init kreftest_init(void)
{
	int rv = 0;
	int count = 0;
	struct task_struct *task;
	printk(KERN_INFO "kreftest_init\n");
	obj = kmalloc(sizeof(*obj), GFP_KERNEL);
	if(!obj)
		return -ENOMEM;

	kref_init(&obj->refcnt);
	count = atomic_read(&obj->refcnt);
	printk(KERN_INFO "kreftest_init 1 count %d\n",count);

	kref_get(&obj->refcnt);
	count = atomic_read(&obj->refcnt);
	printk(KERN_INFO "kreftest_init 2 count %d\n",count);
	task = kthread_run(more_data_handling,&obj->refcnt,"more_data_handling");
	if(task == ERR_PTR(-ENOMEM)){
		rv = -ENOMEM;
		goto out;
	}
	
	printk(KERN_INFO "kreftest_init done\n");
out:
	kref_put(&obj->refcnt,obj_release);
	count = atomic_read(&obj->refcnt);
	printk(KERN_INFO "kreftest_init 3 count %d\n",count);
	return rv;
}

static void __exit kreftest_exit(void)
{
	int count = 0;
	count = atomic_read(&obj->refcnt);
	printk(KERN_INFO "kreftest_exit\n");
	printk(KERN_INFO "kreftest_exit 4 count %d\n",count);
	kref_put(&obj->refcnt, obj_release);
}

module_init(kreftest_init);
module_exit(kreftest_exit);

MODULE_LICENSE("GPL");
