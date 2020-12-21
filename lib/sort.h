#ifndef SORT_H
#define SORT_H

//Library for sorting a generic array

/** Generic function to exchange two items in an array
 * Given: array pointer, size of a data item, items' index (zero-based)
 * */
void exchange_two_items_in_generic_array (void * arr_ptr, int size_of_item, int item1_index, int item2_index);   

/** Reverse an array given length*/
void reverse_generic_array(void * arr_ptr, int size_of_item, int length);


/** Three-way partition sorting a generic array recursively ascending order
 * a: array buffer
 * size_of_item: sizeof(Class type)
 * l: left index
 * r: right index
 * compare: compare function given, -1/0/1 if 1st element is smaller/equal/larger than 2nd one
*/
void sort_three_way_generic_array_asc(void *a, int size_of_item, int l, int r, int (*compare)(void*, void*));



/** Three-way partition sorting a generic array recursively descending order
 * a: array buffer
 * size_of_item: sizeof(Class type)
 * l: left index
 * r: right index
 * compare: compare function given, -1/0/1 if 1st element is smaller/equal/larger than 2nd one
*/
void sort_three_way_generic_array_des(void *a, int size_of_item, int l, int r, int (*compare)(void*, void*));



//EXAMPLE OF AN COMPARER

/** Compare 2 integer pointers
 * Return 0 if equal
 * Return -1 if num_1 < num_2
 * Return 1 if num_1 > num 2
 * */
// int int_comparer(void const* number_pointer_1, void const* number_pointer_2){
//     int n1 = *((int*) number_pointer_1);
//     int n2 = *((int*) number_pointer_2);
//     if(n1 == n2) return 0;
//     else{
//         if(n1 > n2) return 1;
//         else return -1;
//     }
// }

#endif
