
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>


MODULE_LICENSE("GPL");

static struct class *struct_class;
static int dev_major;

static int my_char_device_open(struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING "L%d->%s() major=%d,minor=%d\n", __LINE__, __FUNCTION__, imajor(pinode), iminor(pinode));
    return 0;
}
static ssize_t my_char_device_read(struct file *pfile, char __user *pbuf, size_t count, loff_t *off)
{
    int ret;
    char data[] = "data from my_char_device";
    int len = min(count, sizeof(data));
    ret = copy_to_user(pbuf,data,len);
    printk(KERN_WARNING "L%d->%s()\n", __LINE__, __FUNCTION__);
    return ret;
}
static ssize_t my_char_device_write(struct file *pfile, const char *pbuf, size_t count, loff_t *off)
{
    int ret;
    char data[100];
    int len = min(count, sizeof(data));
    ret = copy_from_user(data,pbuf,len);
    printk(KERN_WARNING "L%d->%s():%s\n", __LINE__, __FUNCTION__, data);
    return count;
}
static int my_char_device_release(struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING "L%d->%s()\n", __LINE__, __FUNCTION__);
    return 0;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_char_device_open,
    .read = my_char_device_read,
    .write = my_char_device_write,
    .release = my_char_device_release,
};


static int my_char_device_init(void)
{
    printk(KERN_INFO "my_char_device init!\n");
    // 查看cat /proc/devices   查看未使用的设备号
    // my_char_device 设备文件名称
    dev_major = register_chrdev(0, "my_char_device", &fops);
    struct_class = class_create(THIS_MODULE, "my_char_class");
    // 自动创建设备文件     也可以通过 `mknod /dev/my_char_device c 200 2` 手动创建设备文件
    device_create(struct_class, NULL, MKDEV(dev_major, 2), NULL, "my_char_device");
    
    return 0;
}
static void my_char_device_exit(void)
{
    printk(KERN_ALERT "my_char_device exit!\n");
    device_destroy(struct_class, MKDEV(dev_major, 2));
    class_destroy(struct_class);
    unregister_chrdev(dev_major, "my_char_device");
    
}

module_init(my_char_device_init);
module_exit(my_char_device_exit);


