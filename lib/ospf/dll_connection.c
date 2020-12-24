#include "ospf.h"
#include <stdio.h>

/******************************CONNECTION LIST***************************/
/** Implementation: dll_connection.c */

extern ConnectionList new_dllist();
extern void free_dllist(ConnectionList);

extern void dll_append(ConnectionList, Connection);
extern void dll_prepend(ConnectionList, Connection);
extern void dll_insert_b(ConnectionList, Connection);
extern void dll_insert_a(ConnectionList, Connection);

extern void dll_delete_node(ConnectionList);
extern int dll_is_empty(ConnectionList);

extern ConnectionList dll_find_node(ConnectionList, Connection);
extern ConnectionList dll_find_and_delete_node(ConnectionList, Connection);

extern Connection dll_val(ConnectionList);

#define dll_first(d) ((d)->flink)
#define dll_next(d) ((d)->flink)
#define dll_last(d) ((d)->blink)
#define dll_prev(d) ((d)->blink)
#define dll_nil(d) (d)

#define dll_traverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#define dll_rtraverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)