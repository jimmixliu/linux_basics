#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/workqueue.h>

#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/hrtimer.h>

static unsigned long my_data = 0;
struct hrtimer hr_timer;
ktime_t ktime;

static void my_timer(struct hrtimer *timer)
{
	my_data++;
	printk("data = %d\n",my_data);
	ktime = ktime_set( 0, 5000*1000);
	hrtimer_start( timer, ktime, HRTIMER_MODE_REL ); 
}


static int __init timer_workqueue_init(void)
{

	printk("init timer\n");


	ktime = ktime_set( 0, 5000*1000);//5ms	
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = my_timer;
	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );

	return 0;
}

static void __exit timer_workqueue_exit(void)
{
	printk("exit del_timer\n");
	hrtimer_cancel( &hr_timer );
}

module_init(timer_workqueue_init);
module_exit(timer_workqueue_exit);

MODULE_LICENSE("GPL");
