
#include<stdlib.h>
#include <string.h>
#include "sort.h"

/** Generic function to exchange two items in an array
 * Given: array pointer, size of a data item, items' index (zero-based)
 * */
void exchange_two_items_in_generic_array (void *arr_ptr, int size_of_item, int item1_index, int item2_index){
    void *item1_prt = arr_ptr + item1_index * size_of_item;
    void *item2_prt = arr_ptr + item2_index * size_of_item;
    void *tmp_ptr = (void *) malloc(size_of_item); //need to initiate void pointer before usage
    
    memcpy(tmp_ptr, item1_prt, size_of_item); //tmp = item1
    memcpy(item1_prt, item2_prt, size_of_item); //item1 = item2
    memcpy(item2_prt,tmp_ptr, size_of_item); //item2 = tmp
    free(tmp_ptr);
    return;
}   

/** Reverse an array given length*/
void reverse_generic_array(void * arr_ptr, int size_of_item, int length){
	for (size_t i = 0; i < length/2; i++)
	{
		exchange_two_items_in_generic_array(arr_ptr, size_of_item, i, length-1-i);
	}
	
}


/** Three-way partition sorting a generic array recursively ascending order
 * a: array buffer
 * size_of_item: sizeof(Class type)
 * l: left index
 * r: right index
 * compare: compare function given, -1/0/1 if 1st element is smaller/equal/larger than 2nd one
*/
void sort_three_way_generic_array_asc(void *a, int size_of_item, int l, int r, int (*compare)(void*, void*)) {
	if (r <= l) return; //base case

	int i = l-1, j = r;
	int p = l-1, q = r;
	while(1){
		//while (a[++i] < a[r]); 
        while(compare(a + (++i) * size_of_item, a + r*size_of_item) < 0);

		//while (a[r] < a[--j])
        while(compare(a + r*size_of_item, a + (--j) * size_of_item ) < 0)
            if (j == l) break;

		if (i >= j) break;
		exchange_two_items_in_generic_array(a, size_of_item, i, j);
		//if (a[i]==a[r]) 
        if (compare(a+i*size_of_item, a + r * size_of_item) == 0) 
            exchange_two_items_in_generic_array(a, size_of_item, ++p, i);
		//if (a[j]==a[r]) 
        if (compare(a+j * size_of_item, a + r * size_of_item) == 0) 
            exchange_two_items_in_generic_array(a, size_of_item, --q, j);
	}

	exchange_two_items_in_generic_array(a, size_of_item, i, r);
	j = i - 1;
	i = i + 1;
	for (int k = l ; k <= p; k++) exchange_two_items_in_generic_array(a,size_of_item, k, j--);
	for (int k = r-1; k >= q; k--) exchange_two_items_in_generic_array(a,size_of_item, k, i++);
	sort_three_way_generic_array_asc(a, size_of_item, l, j, compare);
	sort_three_way_generic_array_asc(a, size_of_item, i, r, compare);
}

/** Three-way partition sorting a generic array recursively descending order
 * a: array buffer
 * size_of_item: sizeof(Class type)
 * l: left index
 * r: right index
 * compare: compare function given, -1/0/1 if 1st element is smaller/equal/larger than 2nd one
*/
void sort_three_way_generic_array_des(void *a, int size_of_item, int l, int r, int (*compare)(void*, void*)){
	sort_three_way_generic_array_asc(a, size_of_item, l, r, compare);
	reverse_generic_array(a, size_of_item, r-l+1);
}