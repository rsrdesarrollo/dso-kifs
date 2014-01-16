#include <linux/kernel.h>
#include <linux/kifs.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <asm/uaccess.h>

#define MODPREFIX_DEBUG
#ifdef MODPREFIX_DEBUG
#define DBG(format, arg...) do { \
		printk(KERN_DEBUG "%s: " format "\n" , __func__ , ## arg); \
	} while (0)
#else
#define DBG(format, arg...) /* */
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("R. Samp");

unsigned int NumEntry = 1;
char *Prefix = "prefix";
char error = 0;

unsigned int *counts = NULL;

module_param(NumEntry, uint, 0);
MODULE_PARM_DESC(NumEntry, "Num KIFS entries");
module_param(Prefix, charp, 0);
MODULE_PARM_DESC(Prefix, "Prefix of KIFS entries");

int kifs_read_handler(char __user *buffer, unsigned int maxchars, void *data){
	char kbuff[11];
	int used = 0;
	
	DBG("[KIFS] void data at %p", data);
	
	if(maxchars <= (used = snprintf(kbuff,10,"%d",(*((unsigned int*)data)))))
		return -ENOSPC;
	used++; //Por el \0
	
	used -= copy_to_user(buffer, kbuff, used);
	
	return used;
}

int kifs_write_handler(const char __user *buffer, unsigned int len, void *data){
	(*((unsigned int*)data))++;
	return len;
}

void _clean_entries(const int num){
	char kbuff[MAX_KIFS_ENTRY_NAME_SIZE+12];
	int i;
	for (i = 0; i < num; i++){
		sprintf(kbuff,"%s%d",Prefix,i);
		remove_kifs_entry(kbuff);
	}
}

int _init_modprefix(void){
	char kbuff[MAX_KIFS_ENTRY_NAME_SIZE+12];
	int i;
	
	if(NULL == (counts = vmalloc(NumEntry*sizeof(unsigned int))))
		return -ENOMEM;
		
	for(i = 0; i < NumEntry && !error; i++){
		sprintf(kbuff,"%s%d",Prefix,i);
		DBG("[KIFS] Create entry %s, data at %p",kbuff,(counts+i));
		error = !create_kifs_entry(kbuff,kifs_read_handler,kifs_write_handler,(counts+i));
	}
		
	if(error){
		_clean_entries(i-1);
		return -ENOSPC;
	}
	
	memset(counts,0,NumEntry*sizeof(unsigned int));
		
	return 0;
}
module_init(_init_modprefix);

void _exit_modprefix(void){
	if(!error){
		vfree(counts);
		_clean_entries(NumEntry);
	}
}
module_exit(_exit_modprefix);
