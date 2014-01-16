#include <linux/kernel.h>
#include <linux/kifs.h>
#include <asm-generic/uaccess.h>
#include <linux/types.h>
#include "modlist.h"

#define streq(a,b) (strcmp((a),(b)) == 0)

LIST_HEAD(mylist);
kifs_entry_t *_list_kifs_entry;

int init_mod_list(void){

	_list_kifs_entry = create_kifs_entry(KIFS_LIST_ENTRYNAME,
			kifs_list_read_handler,
			kifs_list_write_handler,
			NULL);
	return ((_list_kifs_entry == NULL)? -1 : 0);
}

void del_mod_list(void){
	clear_list(&mylist);
	remove_kifs_entry(KIFS_LIST_ENTRYNAME);
}

/*
* FUNCIONES DE MANEJO DE LISTA
* --------------------------------------
*/

inline void remove_singel_item(list_item_t *elem){
	list_del(&(elem->links));
	vfree(elem);
}

void clear_list(struct list_head *list){
	list_item_t *aux, *elem = NULL;

	list_for_each_entry_safe(elem, aux, list, links){
		remove_singel_item(elem);
	}
}

void add_item_list(struct list_head *list, int data){
	list_item_t *item = vmalloc(sizeof(list_item_t));
	item->data = data;
	list_add(&(item->links),list);
}

void remove_items_list(struct list_head *list, int data){
	list_item_t *aux, *elem = NULL;

	list_for_each_entry_safe(elem, aux, list, links){
		if(elem->data == data)
		remove_singel_item(elem);
	}
}

/*
* FUNCIONES DE MANEJO DE EVENTOS DE E/S
* ------------------------------------------
*/
int kifs_list_read_handler(char __user *buffer, unsigned int buffer_len, void *data) {

	list_item_t *elem = NULL;
	char kbuff[15];
	int need = 0;
	int tot_used = 0;
	
	list_for_each_entry(elem, &mylist, links){
		need = sprintf(kbuff, "%d\n", elem->data);
		if(tot_used + need >= buffer_len)
			return -ENOSPC;
				
		tot_used += need - copy_to_user(buffer + tot_used, kbuff, need);
	}
	copy_to_user(buffer + tot_used, "\0", 1);

	return tot_used;
}

int kifs_list_write_handler(const char __user *buffer, unsigned int count, void *data) {
	char line[25] = "";
	char command[8] = "";
	int num = 0;

	copy_from_user(line, buffer, min(count,24U));
	line[min(count,24U)] = 0;
	
	sscanf(line,"%7s %d", command, &num);

	if(streq(command,"add")){
		add_item_list(&mylist, num);
	}else if(streq(command,"cleanup")){
		clear_list(&mylist);
	}else if(streq(command,"remove")){
		remove_items_list(&mylist, num);
	}
	
	return min(count,24U);
}


