#include <linux/kernel.h>
#include <linux/module.h>
#include "modcount.h"
#include "modlist.h"

MODULE_AUTHOR("R. Sampedro");
MODULE_DESCRIPTION("Modulo contador sobre sistema de ficheros virtual KIFS");
MODULE_LICENSE("GPL");


int _init_modcountlist(void){

	if(init_mod_count() || init_mod_list())
		return -EFAULT;

	return 0;
}
module_init(_init_modcountlist);

void _exit_modcountlist(void){
	del_mod_count();
	del_mod_list();	
}
module_exit(_exit_modcountlist);
