#ifndef _SYMBOL_H
#define _SYMBOL_H


typedef struct _symbol_table_entry {
	char *id;
	int is_const;
	int type_code;
	int vec;
	int is_init; //VERY IMPORTANT, the implementation for this is not straight forward
	struct _symbol_table_entry *next;
	struct _symbol_table_entry *sib;
} symbol_table_entry;

void scope_enter();
void scope_exit();
int scope_is_in_main();
int symbol_exists_in_this_scope(char *id);
bool is_symobl_const(char *id);
void symbol_add(symbol_table_entry entry);
symbol_table_entry* symbol_find(char *id);

#endif

