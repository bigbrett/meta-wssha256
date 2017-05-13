/**
 * @file   wssha256.c
 * @author Brett Nicholas
 * @date   5/11/17
 * @version 0.1
 * @brief   
 * Linux loadable kernel module (LKM) for sha256 acceleator. This module maps to /dev/wssha256 and
 * comes with a helper C program that can be run in Linux user space to communicate with this LKM.
 * 
 * Based on the BBB LED driver template by Derek Molloy
 */

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
#include <linux/io.h>

// TODO this needs to be changed...we should not be hardcoding these
#define WSSHA256BASEADDR 0x43C00000
#define XSHA256_AXILITES_ADDR_AP_CTRL          0x000
#define XSHA256_AXILITES_ADDR_GIE              0x004
#define XSHA256_AXILITES_ADDR_IER              0x008
#define XSHA256_AXILITES_ADDR_ISR              0x00c
#define XSHA256_AXILITES_ADDR_BASE_OFFSET_DATA 0x200
#define XSHA256_AXILITES_BITS_BASE_OFFSET_DATA 32
#define XSHA256_AXILITES_ADDR_BYTES_DATA       0x208
#define XSHA256_AXILITES_BITS_BYTES_DATA       32
#define XSHA256_AXILITES_ADDR_DATA_BASE        0x100
#define XSHA256_AXILITES_ADDR_DATA_HIGH        0x1ff
#define XSHA256_AXILITES_WIDTH_DATA            8
#define XSHA256_AXILITES_DEPTH_DATA            256
#define XSHA256_AXILITES_ADDR_DIGEST_BASE      0x220
#define XSHA256_AXILITES_ADDR_DIGEST_HIGH      0x23f
#define XSHA256_AXILITES_WIDTH_DIGEST          8
#define XSHA256_AXILITES_DEPTH_DIGEST          32

#define SHA256_DGST_SIZE 32 
#define SHA256_MSG_SIZE  256

#define  DEVICE_NAME "wssha256char"    ///< The device will appear at /dev/wssha256 using this value
#define  CLASS_NAME  "wssha256"        ///< The device class -- this is a character device driver

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Brett Nicholas");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver for wssha256 block in hardware");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static int majorNumber;                  ///< Stores the device number -- determined automatically
static void __iomem *vbaseaddr = NULL;          // void pointer to virtual memory mapped address for the device
static volatile char message[SHA256_MSG_SIZE] = {0}; ///< Memory for the string (message)that is passed from userspace
static volatile char digest[SHA256_DGST_SIZE] = {0}; ///< Memory for the string (digest) that is passed back to userspace
volatile static int numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  wssha256charClass  = NULL; ///< The device-driver class struct pointer
static struct device* wssha256charDevice = NULL; ///< The device-driver device struct pointer


// The prototype functions for the character driver -- must come before the struct definition
static int     wssha256_open(struct inode *, struct file *);
static int     wssha256_release(struct inode *, struct file *);
static ssize_t wssha256_read(struct file *, char *, size_t, loff_t *);
static ssize_t wssha256_write(struct file *, const char *, size_t, loff_t *);


/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
	.open = wssha256_open,
	.read = wssha256_read,
	.write = wssha256_write,
	.release = wssha256_release,
};


/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init wssha256_init(void){
	printk(KERN_INFO "wssha256: Initializing the wssha256 LKM\n");

	// map device into memory and initialize structure
	// TODO dtc support
	vbaseaddr = ioremap(WSSHA256BASEADDR, SZ_64K);
  printk(KERN_INFO "Virtual Address = 0x%X\n",vbaseaddr);

	// Try to dynamically allocate a major number for the device -- more difficult but worth it
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "wssha256 failed to register a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "wssha256: registered correctly with major number %d\n", majorNumber);

	// Register the device class
	wssha256charClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(wssha256charClass)){                // Check for error and clean up if there is
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(wssha256charClass);          // Correct way to return an error on a pointer
	}
	printk(KERN_INFO "wssha256: device class registered correctly\n");

	// Register the device driver
	wssha256charDevice = device_create(wssha256charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(wssha256charDevice)){               // Clean up if there is an error
		class_destroy(wssha256charClass);           // Repeated code but the alternative is goto statements
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(wssha256charDevice);
	}
	printk(KERN_INFO "wssha256: device class created correctly\n"); // Made it! device was initialized
	return 0;
}


/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit wssha256_exit(void){
	iounmap(vbaseaddr); // unmap device IO memory 
	device_destroy(wssha256charClass, MKDEV(majorNumber, 0));     // remove the device
	class_unregister(wssha256charClass);                          // unregister the device class
	class_destroy(wssha256charClass);                             // remove the device class
	unregister_chrdev(majorNumber, DEVICE_NAME);              // unregister the major number
	printk(KERN_INFO "wssha256: Exiting\n");
}


/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int wssha256_open(struct inode *inodep, struct file *filep){
	if (numberOpens > 0)
	{
	  printk(KERN_INFO "Error: device already open\n");
	  return -EBUSY;
	}
	numberOpens++;
	printk(KERN_INFO "wssha256: Device has been opened %d time(s)\n", numberOpens);
	return 0;
}


/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the buffer
 *  @param offset The offset if required
 */
static ssize_t wssha256_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
  
	// Read digest from hardware 
  memcpy_fromio(digest, vbaseaddr+XSHA256_AXILITES_ADDR_DIGEST_BASE, SHA256_DGST_SIZE);

  // Copy digest back into userspace (*to,*from,size)
  error_count = copy_to_user(buffer, digest, len);

	if (error_count==0){            // if true then have success
		printk(KERN_INFO "wssha256: Sent %d characters to the user\n", len);
		return len;  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "wssha256: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
	}
}


/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the copy_from_user() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t wssha256_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{  
  printk(KERN_INFO "initializing parameters \n\t0x%X <-- %d\n\t0x%X <-- %d\n",
                                    vbaseaddr+XSHA256_AXILITES_ADDR_BYTES_DATA, 
                                    SHA256_MSG_SIZE,
                                    vbaseaddr+XSHA256_AXILITES_ADDR_BASE_OFFSET_DATA,
                                    0);
            
  // init hardware parameters 
  iowrite32(SHA256_MSG_SIZE, vbaseaddr+XSHA256_AXILITES_ADDR_BYTES_DATA);// set bytes to 256
	printk(KERN_INFO "1 iowrite done\n");
  iowrite32(0, vbaseaddr+XSHA256_AXILITES_ADDR_BASE_OFFSET_DATA); // set offset to 0 
	printk(KERN_INFO "2 iowrites done\n");

	printk(KERN_INFO "writing data recevied from user\n");
	// copy data from userspace into kernel message buffer
	copy_from_user(message, buffer, len);
  printk(KERN_INFO "after copy\n");
  
  // write data from kernel message buffer to PL peripheral "message" register region
  memcpy_toio(vbaseaddr+XSHA256_AXILITES_ADDR_DATA_BASE, message, SHA256_MSG_SIZE);
  printk(KERN_INFO "after memcpy_toio\n");

  // start AES block using read-modify-write on ap_ctrl register
  unsigned int ctrl_reg = ioread32(vbaseaddr + XSHA256_AXILITES_ADDR_AP_CTRL) & 0x80; // read and get bit
  iowrite32(ctrl_reg | 0x01, vbaseaddr + XSHA256_AXILITES_ADDR_AP_CTRL);

	printk(KERN_INFO "wssha256: Received %zu characters from the user\n", len);
	return len;
}


/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int wssha256_release(struct inode *inodep, struct file *filep){
	//printk(KERN_INFO "wssha256: Device successfully closed\n");
	return 0;
}



/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(wssha256_init);
module_exit(wssha256_exit);
