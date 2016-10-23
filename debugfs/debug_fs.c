#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static struct dentry *my_root_debugfs;
static u8 a = 0;
char debug_data[32] = "This is my debugfs data";
struct debugfs_blob_wrapper b;



int c_open (struct inode *inode, struct file *filp)
{
	filp->private_data = inode->i_private;
	return 0;
}

ssize_t c_read (struct file *filp, char __user *buffer, size_t count, loff_t *ppos)
{
	if ( *ppos >= 32 )
		return 0;

	if ( *ppos + count >32 )
		count = 32 - *ppos;

	if(copy_to_user(buffer, debug_data + *ppos, count))
		return -EFAULT;

	*ppos += count;

	return count;
}

ssize_t c_write (struct file *filp, const char __user *buffer, size_t count, loff_t *ppos)
{
	if ( *ppos >= 32 )
		return 0;

	if ( *ppos + count > 32)
		count = 3 - *ppos;

	if(copy_from_user(debug_data + *ppos, buffer, count))
		return -EFAULT;

	*ppos += count;
	return count;
}

static int d_show(void *data, u64 *val)
{
	*val = (u64)a + (*(u64*)data);
	return 0;
}

static int d_set(void *data, u64 val)
{
	a = (u8)val;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(d_fops,d_show,d_set,"%llu");


struct file_operations c_ops = {
	.owner = THIS_MODULE,
	.open = c_open,
	.read = c_read,
	.write = c_write,
};

static int __init  debug_fs(void)
{
	struct dentry *r_a,*r_b,*s_c,*sub_dir,*d_c;

	printk(KERN_INFO "debug_fs init\n");

	my_root_debugfs = debugfs_create_dir("my_debug",NULL);
	if ( !my_root_debugfs )
		return -ENOENT;
		
	r_a = debugfs_create_u8("a",0644,my_root_debugfs,&a);
	if ( !r_a )
		goto Fail;

	b.data = debug_data;
	b.size = strlen(debug_data)+1;
	r_b = debugfs_create_blob("b",0644,my_root_debugfs,&b);
	if ( !r_b )
		goto Fail;

        sub_dir = debugfs_create_dir("sub_dir",my_root_debugfs);
	if ( !sub_dir ) 
		goto Fail;
	s_c = debugfs_create_file("c",0644,sub_dir,NULL,&c_ops);

	d_c = debugfs_create_file("d",0644,my_root_debugfs,(void *)&a,&d_fops);

	return 0;
Fail:
	debugfs_remove_recursive(my_root_debugfs);
	my_root_debugfs = NULL;
	return -ENOENT;
}

static void __exit debug_exit(void)
{
	printk(KERN_INFO "debug_exit\n");
	debugfs_remove_recursive(my_root_debugfs);
}


module_init(debug_fs);
module_exit(debug_exit);
MODULE_AUTHOR("debug_fs");
MODULE_DESCRIPTION("check debug fs");
MODULE_LICENSE("GPL");
