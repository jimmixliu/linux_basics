#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/workqueue.h>

#include <linux/jiffies.h>
#include <linux/time.h>

static unsigned long my_data = 0;
struct work_struct work;
struct timer_list timer;

static void my_work(struct work_struct *work)
{
	printk("my_work\n");
	mod_timer(&timer,jiffies+ 2 * HZ);
}

static void my_timer(unsigned long data)
{
	my_data++;
	printk("data = %d\n",my_data);
	schedule_work(&work);
}


static int __init timer_workqueue_init(void)
{

	printk("init timer\n");

	INIT_WORK(&work,my_work);

	init_timer(&timer);
	timer.expires = jiffies + 2 * HZ;
	timer.function = my_timer;
	timer.data = (unsigned long)my_data;
	add_timer(&timer);

	return 0;
}

static void __exit timer_workqueue_exit(void)
{
	printk("exit del_timer\n");
	del_timer(&timer);
	cancel_work_sync(&work);
}

module_init(timer_workqueue_init);
module_exit(timer_workqueue_exit);

MODULE_LICENSE("GPL");
