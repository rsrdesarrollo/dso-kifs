#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kifs.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("R. Sampedro");
MODULE_DESCRIPTION("Modulo contador sobre sistema de ficheros virtual KIFS");
MODULE_LICENSE("GPL");

#define KIFS_ENTRYNAME "modcount"

kifs_entry_t *_entry;
int _count;

inline int digit_count(int a){
	int ret = 0;
	while(a){
		a /= 10;
		ret++;
	}
	return ret;
}

int kifs_read_handler(char __user *buffer, unsigned int maxchars, void *data){
	char kbuff[11];
	int used = 0;
	
	if(maxchars <= (used = snprintf(kbuff,10,"%d",_count)))
		return -ENOSPC;
	used++; //Por el \0
	
	used -= copy_to_user(buffer, kbuff, used);
	
	return used;
}

int kifs_write_handler(const char __user *buffer, unsigned int len, void *data){
	_count ++;
	return len;
}

int _init_modcount(void){
	_entry = create_kifs_entry(KIFS_ENTRYNAME,
			kifs_read_handler,
			kifs_write_handler,
			NULL);
	_count = 0;

	if(_entry == NULL)
		return -EFAULT;

	return 0;
}
module_init(_init_modcount);

void _exit_modcount(void){
	remove_kifs_entry(KIFS_ENTRYNAME);		
}
module_exit(_exit_modcount);
