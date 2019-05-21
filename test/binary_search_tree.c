#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../src/binary_search_tree.h"

int cmp(bst_data_t, bst_data_t);
void print(bst_data_t, size_t);

int main(int argc, char* argv[]) 
{
    int           array[1000];
	size_t        size, i;
	pt_BSTree     bst;
	int           input;
	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	
	size = atoi(argv[1]);

	bst = BSTree_Create(cmp);

    printf("creat over\n");
	for (i = 0; i < size; i++)
	    array[i] = rand() % 1000;
	
    printf("random over\n");
	for (i = 0; i < size; i++) 
	    BSTree_Insert(bst, &array[i]);
	
    printf("insert over, size %ld\n", BSTree_Size(bst));
	BSTree_InOrderMap(bst, print);
    printf("print over\n");

	printf("delete: ");
	scanf("%d", &input);

	if (BSTree_Delete(bst, &input) == DS_ERROR)
	    printf("fail to delete\n");
	else {
	    printf("remain size: %ld\n", BSTree_Size(bst));
	    BSTree_InOrderMap(bst, print);
	}

	return 0;
}

int cmp(bst_data_t d1, bst_data_t d2)
{
    int* i1 = (int*)d1;
	int* i2 = (int*)d2;

	return *i1 - *i2;
}

void print(bst_data_t d, size_t depth)
{
    size_t   i;
	int*     pi = (int*)d;
    for (i = 0; i < depth; i++)
	    printf("  ");
	printf("%3d\n", *pi);
}
