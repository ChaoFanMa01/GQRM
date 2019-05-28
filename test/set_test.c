#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/set.h"

int cmp(set_data_t, set_data_t);
void map(set_data_t, size_t);

int main(int argc, char* argv[])
{
    int          array[1000];
	size_t       size, i;
	pt_Set       rhs, lhs, res;
	srand((unsigned)time(0));

	if (argc != 2)
	    exit(-1);
	
	size = atoi(argv[1]);

	for (i = 0; i < 1000; i++)
	    array[i] = rand() % 1000;
	
	rhs = Set_Create(cmp, 1);
	lhs = Set_Create(cmp, 2);
	for (i = 0; i < size; i++) {
	    Set_Insert(rhs, &array[rand() % 1000]);
	    Set_Insert(lhs, &array[rand() % 1000]);
	}
    printf("set 1:");
    Set_Map(rhs, map);
    printf("\nset 2:");
    Set_Map(lhs, map);
	printf("\n");

	Set_InSetMinus(rhs, lhs);
	printf("minus size %ld: ", Set_Size(rhs));
	Set_Map(rhs, map);
	printf("\n");

/*
	Set_InSetUnion(rhs, lhs);
    printf("union size %ld: ", Set_Size(rhs));
	Set_Map(rhs, map);
	printf("\n");
*/
/*
	res = Set_Union(rhs, lhs);
	printf("union size %ld:", Set_Size(res));
	Set_Map(res, map);
	printf("\n");
*/
/*
    res = Set_Minus(rhs, lhs);
	printf("union size %ld:", Set_Size(res));
	Set_Map(res, map);
	printf("\n");
*/
return 0;
}

int cmp(set_data_t d1, set_data_t d2)
{
    int*  pi1 = (int*)d1;
	int*  pi2 = (int*)d2;

	return *pi1 - *pi2;
}

void map(set_data_t data, size_t n)
{
    int* pi = (int*)data;
    printf("%4d ", *pi);
}
