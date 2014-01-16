#include <linux/list.h>

#ifndef __MODLIST_H_
#define __MODLIST_H_

#define KIFS_LIST_ENTRYNAME "modlist"

typedef struct {
      int data;
      struct list_head links;
}list_item_t;

int init_mod_list(void);
void del_mod_list(void);
int kifs_list_read_handler(char __user *buffer, unsigned int buffer_len, void *data);
int kifs_list_write_handler(const char __user *buffer, unsigned int count, void *data);
void clear_list(struct list_head *list);

#endif
