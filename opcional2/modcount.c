#include <linux/kernel.h>
#include <linux/kifs.h>
#include <asm-generic/uaccess.h>
#include "modcount.h"

int _count;
kifs_entry_t *_count_kifs_entry;

int init_mod_count(void){
	_count = 0;

	_count_kifs_entry = create_kifs_entry(KIFS_COUNT_ENTRYNAME,
			kifs_count_read_handler,
			kifs_count_write_handler,
			NULL);
	return ((_count_kifs_entry == NULL)? -1 : 0);
}

void del_mod_count(void){
	remove_kifs_entry(KIFS_COUNT_ENTRYNAME);
}


int kifs_count_read_handler(char __user *buffer, unsigned int maxchars, void *data){
	char kbuff[11];
	int used = 0;
	
	if(maxchars <= (used = snprintf(kbuff,10,"%d",_count)))
		return -ENOSPC;
	used++; //Por el \0
	
	used -= copy_to_user(buffer, kbuff, used);
	
	return used;
}

int kifs_count_write_handler(const char __user *buffer, unsigned int len, void *data){
	_count ++;
	return len;
}



