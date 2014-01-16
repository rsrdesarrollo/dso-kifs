
#ifndef __MODCOUNT_H_
#define __MODCOUNT_H_

#define KIFS_COUNT_ENTRYNAME "modcount"

int init_mod_count(void);
void del_mod_count(void);
int kifs_count_read_handler(char __user *buffer, unsigned int buffer_len, void *data);
int kifs_count_write_handler(const char __user *buffer, unsigned int count, void *data);

#endif
