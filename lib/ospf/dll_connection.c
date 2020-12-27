#include "ospf.h"
#include <stdio.h>

/******************************CONNECTION LIST***************************/
/** Implementation: dll_connection.c */

ConnectionList new_dllist(){
  ConnectionList d;

  d = malloc(sizeof(struct ConnectionNode));
  d->flink = d;
  d->blink = d;
  return d;
}
void free_dllist(ConnectionList l){
  while (!dll_is_empty(l)) {
    dll_delete_node(dll_first(l));
  }
  free(l);
}

/** Inserts at the end of the list. */
void dll_append(ConnectionList l, Connection con){
  dll_insert_b(l, con);
}

/** Inserts at the beginning of the list. */
void dll_prepend(ConnectionList l, Connection con){
  dll_insert_b(l->flink, con);
}

/** Inserts before a given node. */ 
void dll_insert_b(ConnectionList list, Connection con){
  ConnectionList newnode;

  newnode = (ConnectionList) malloc (sizeof(struct ConnectionNode));
  newnode->val = con;

  newnode->flink = list;
  newnode->blink = list->blink;
  newnode->flink->blink = newnode;
  newnode->blink->flink = newnode;
}


/** Inserts after a given node. */
void dll_insert_a(ConnectionList n, Connection con){
  dll_insert_b(n->flink, con);
}

/** Deletes an arbitrary item. */
extern void dll_delete_node(ConnectionList node){
  node->flink->blink = node->blink;
  node->blink->flink = node->flink;
  free(node);
}


extern int dll_is_empty(ConnectionList l){
  return (l->flink == l);
}

extern Connection dll_val(ConnectionList con){
  return con->val;
}

/** Find the next node given start, stop endpoint. Return NULL if not found*/
extern ConnectionList dll_find_next_connection_by_start_stop(ConnectionList list, int start, int stop){
  ConnectionList node;
  dll_traverse(node, list){
    if((node->val.router1 == start) && (node->val.router2 == stop)){
      return node;
    }
  }

  return NULL;
}