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


/** Find a node that has the given connection. Return the node or NULL if not found.*/
ConnectionList dll_find_node(ConnectionList l, Connection con){
  ConnectionList travel;
  jrb_traverse(travel, l){
    if(travel->val.id == con.id) return travel;
  }
  return NULL;
}

/** Find & delete node that has the given connection. Return 1 (success) or 0 (failed) */
int dll_find_and_delete_node(ConnectionList l, Connection con){
  ConnectionList found = dll_find_node(l, con);
  if(found == NULL) return 0;
  //found
  free(found);
  return 1;
}
