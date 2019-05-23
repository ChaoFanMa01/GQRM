#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../src/avl_tree.h"

int cmp(avlt_data_t, avlt_data_t);
void print(avlt_data_t, size_t);

int main(int argc, char* argv[]) 
{
    int           array[1000];
	size_t        size, i;
	pt_AVLTree    avl;
	int           input;
	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	
	size = atoi(argv[1]);

	avl = AVLTree_Create(cmp);

    printf("creat over\n");
	for (i = 0; i < size; i++)
	    array[i] = rand() % 1000;
	
    printf("random over\n");
	for (i = 0; i < size; i++) 
	    AVLTree_Insert(avl, &array[i]);
	
    printf("insert over, size %ld\n", AVLTree_Size(avl));
	AVLTree_InOrderMap(avl, print);
    printf("print over\n");

	printf("delete: ");
	scanf("%d", &input);

	if (AVLTree_Delete(avl, &input) == DS_ERROR)
	    printf("fail to delete\n");
	else {
	    printf("remain size: %ld\n", AVLTree_Size(avl));
	    AVLTree_InOrderMap(avl, print);
	}

	return 0;
}

int cmp(avlt_data_t d1, avlt_data_t d2)
{
    int* i1 = (int*)d1;
	int* i2 = (int*)d2;

	return *i1 - *i2;
}

void print(avlt_data_t d, size_t depth)
{
    size_t   i;
	int*     pi = (int*)d;
    for (i = 0; i < depth; i++)
	    printf("  ");
	printf("%3d\n", *pi);
}
