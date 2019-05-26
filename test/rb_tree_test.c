#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../src/rb_tree.h"

int cmp(rbt_data_t, rbt_data_t);
void print(rbt_data_t, size_t);

int main(int argc, char* argv[]) 
{
    int           array[1000];
	size_t        size, i;
	pt_RBTree    rb;
	int           input;
	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	
	size = atoi(argv[1]);

	rb = RBTree_Create(cmp);

    printf("creat over\n");
	for (i = 0; i < size; i++)
	    array[i] = rand() % 1000;
	
    printf("random over\n");
	for (i = 0; i < size; i++) {
	    printf("Insert %d\n", array[i]);
	    RBTree_Insert(rb, &array[i]);
	}
	
    printf("insert over, size %ld\n", RBTree_Size(rb));
	RBTree_InOrderMap(rb, print);
    printf("print over\n");
/*
	printf("delete: ");
	scanf("%d", &input);

	if (RBTree_Delete(rb, &input) == DS_ERROR)
	    printf("fail to delete\n");
	else {
	    printf("remain size: %ld\n", RBTree_Size(rb));
	    RBTree_InOrderMap(rb, print);
	}
*/
	return 0;
}

int cmp(rbt_data_t d1, rbt_data_t d2)
{
    int* i1 = (int*)d1;
	int* i2 = (int*)d2;

	return *i1 - *i2;
}

void print(rbt_data_t d, size_t depth)
{
    size_t   i;
	int*     pi = (int*)d;
    for (i = 0; i < depth; i++)
	    printf("  ");
	printf("%3d\n", *pi);
}
