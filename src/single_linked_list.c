/*
 * GQRM
 * Copyright (C) 2019-2025 Chaofan Ma <chaofanma@hotmail.com>
 *
 * This file is part of GQRM.
 *
 * GQRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GQRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GQRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <sys/types.h>

#include "header.h"
#include "single_linked_list.h"

typedef struct SingleLinkedListNode   sll_node;
typedef struct SingleLinkedListNode*  psll_node;

static psll_node CreateNode(sll_data_t);
static psll_node FindPrev(p_sll, size_t);
static psll_node FindNode(p_sll, size_t);
static void swap(sll_data_t*, sll_data_t*);

/* @struct 
 *
 * Node in a single linked list.
 */
struct SingleLinkedListNode {
    /* pointer to data stored in this node */
    sll_data_t     data;
    /* next node in this list */
    psll_node      next;
}; 

/* @struct
 *
 * Single linked list.
 */
struct SingleLinkedList {
    /* the first node with valid data */
    psll_node    head;
    /* the number of nodes in this list */
    size_t      length;
};

/* @fn
 * Initialize a non-initialized single linked list.
 */
ds_stat
SingleLinkedList_Init(p_sll* list) 
{
    /* 
     * if the given list pointing to a non-null
     * pointer, we assume it has been initialized,
     * and we leave it unchanged.
     */
    if (*list != NULL)
        return DS_ERROR;

    *list = malloc(sizeof(struct SingleLinkedList));

    if (!list) 
        return DS_ERROR;

    assert(list);
    (*list)->head   = NULL;
    (*list)->length = 0;

    return DS_OK;
}

/* @fn
 * Create a node with initializing data field to given data
 * and next point to NULL.
 */
static psll_node
CreateNode(sll_data_t data) 
{
    psll_node    nd = malloc(sizeof(sll_node));

    if (!nd)
        return NULL;
    assert(nd);

    nd->data = data;
    nd->next = NULL;

    return nd;
}

/* @fn
 * Find the node before given position.
 */
static psll_node
FindPrev(p_sll list, size_t index) 
{
    psll_node prev;
    size_t    i;

    if (SingleLinkedList_Empty(list) == DS_TRUE)
        return NULL;
    if (index < 0 || index > list->length)
        return NULL;

    for (i = 1, prev = list->head; i < index; 
         i++, prev = prev->next) ;
    
    assert(prev);
    return prev;
}

/* @fn
 * Find node at given position.
 */
static psll_node
FindNode(p_sll list, size_t index) 
{
    psll_node nd;
    size_t    i;

    if (SingleLinkedList_Empty(list) == DS_TRUE)
        return NULL;
    if (index < 0 || index >= list->length )
        return NULL;

    for (i = 0, nd = list->head; i < index; i++, nd = nd->next);

    assert(nd);
    return nd;
}

/* @fn
 * Insert a data to designed position.
 * @param list The list where data will be inserted.
 * @param index The insertion position of the data.
 * @param data Pointer to the data being inserted.
 */
ds_stat
SingleLinkedList_Insert(p_sll list, size_t index, 
                        sll_data_t data) 
{
    psll_node    nd, prev;

    if (!list) return DS_ERROR;
    if (index < 0 || index > list->length || !data)
        return DS_ERROR;
    if (!(nd = CreateNode(data)))
        return DS_ERROR;

    assert(nd);
    assert(list);

    if (!list->head) {
        list->head = nd;
    } else {
        if (index == 0) {
            nd->next = list->head;
            list->head = nd;
        } else {
            prev = FindPrev(list, index);
            
            assert(prev);

            nd->next = prev->next;
            prev->next = nd;
        }
    }

    list->length++;

    return DS_OK;
}

/* @fn
 * Insert a data to the head of a list.
 */
ds_stat
SingleLinkedList_InsertHead(p_sll list, sll_data_t data) 
{
    return SingleLinkedList_Insert(list, 0, data);
}

/* @fn
 * Inset a data to the tail of a list.
 */
ds_stat
SingleLinkedList_InsertTail(p_sll list, sll_data_t data) 
{
    if (!list) return DS_ERROR;

    return SingleLinkedList_Insert(list, list->length, data);
}

/* @fn
 * Delete the node at given position.
 * 
 * @param list From which the node is deleted.
 * @param index The position of the node to be deleted.
 * @param re The data of the deleted node will be returned
 *           through this parameter.
 */
ds_stat
SingleLinkedList_Delete(p_sll list, size_t index, sll_data_t* re) 
{
    psll_node    nd, prev;

    if (SingleLinkedList_Empty(list) == DS_TRUE)
        return DS_ERROR;
    if (index < 0 || index >= list->length)
        return DS_ERROR;

    if (index == 0) {
        nd = list->head;
        list->head = list->head->next;
    } else {
        prev = FindPrev(list, index);

        assert(prev);

        nd = prev->next;
        prev->next = prev->next->next;
    }

    assert(nd);
    if (re)
        *re = nd->data;
    free(nd);

    list->length--;

    return DS_OK;
}

/* @fn
 * Delete the first node of a list.
 */
ds_stat
SingleLinkedList_DeleteHead(p_sll list, sll_data_t* re) 
{
    return SingleLinkedList_Delete(list, 0, re);
}

/* @fn
 * Delete the last node of a list.
 */
ds_stat
SingleLinkedList_DeleteTail(p_sll list, sll_data_t* re)
{
    return SingleLinkedList_Delete(list, list->length - 1, re);
}

/* @fn
 * Clear a list, but not destroy itself.
 */
void
SingleLinkedList_Clear(p_sll list, sll_clear_op op)
{
    sll_data_t    tmp;

    if (!list)
        return;

    while (SingleLinkedList_Empty(list) == DS_FALSE) {
        SingleLinkedList_DeleteHead(list, &tmp);
        if (op)
            op(&tmp);
    }
}

/* @fn
 * Clear a list, and destroy itself.
 */
void
SingleLinkedList_Destroy(p_sll* list, sll_clear_op op)
{
    if (!list)
        return;

    SingleLinkedList_Clear(*list, op);
    free(*list);
    *list = NULL;
}

/* @fn
 * Check whether the given list is empty.
 */
ds_bool
SingleLinkedList_Empty(p_sll list) 
{
    if (!list || list->length == 0)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn
 * Return the length of this list.
 */
size_t
SingleLinkedList_Size(p_sll list) 
{
    if (!list) return 0;

    return list->length;
}

/* @fn
 * Do something to data of each node in list according
 * to the given function.
 */
void
SingleLinkedList_Map(p_sll list, mapfunc map, void* vp) 
{
    psll_node nd;

    if (!list) return;
    assert(map);

    for (nd = list->head; nd; nd = nd->next)
        map(&nd->data, vp);
}

/* @fn
 * Concatenate two lists, i.e., joining list2 to the end of 
 * list1 without varying their internal order.
 */
ds_stat
SingleLinkedList_Concat(p_sll list1, p_sll list2) 
{
    psll_node iter;

    if (!list1 || !list2)
        return DS_ERROR;

    for (iter = list2->head; iter; iter = iter->next)
        SingleLinkedList_InsertTail(list1, iter->data);

    return DS_OK;
}

/* @fn
 * Perform a deep copy of given list, and return new list.
 */
p_sll
SingleLinkedList_Copy(p_sll list) 
{
    p_sll     cpy = NULL;
    psll_node iter;

    if (SingleLinkedList_Init(&cpy) == DS_ERROR)
        return NULL;

    assert(cpy);

    for (iter = list->head; iter; iter = iter->next)
        SingleLinkedList_InsertTail(cpy, iter->data);

    return cpy;
}

/* @fn
 * Get the data stored at the node designed by parameter "index",
 * and return this data through parameter "re".
 */
ds_stat
SingleLinkedList_GetData(p_sll list, size_t index, 
                         sll_data_t* re) 
{
    psll_node   nd = FindNode(list, index);
    if (!nd || !re)
        return DS_ERROR;

    assert(nd);
    assert(re);
    *re = nd->data;

    return DS_OK;
}

/* @fn
 * Get the data stored at the first node.
 */
ds_stat
SingleLinkedList_GetHeadData(p_sll list, sll_data_t* re) 
{
    return SingleLinkedList_GetData(list, 0, re);
}

/* @fn
 * Get the data stored at the last node.
 */
ds_stat
SingleLinkedList_GetTailData(p_sll list, sll_data_t* re) 
{
    if (!list)
        return DS_ERROR;

    return SingleLinkedList_GetData(list, list->length - 1, re);
}

/* @fn
 * Replace the data of the node with designed "index" with
 * given "data".
 */
ds_stat
SingleLinkedList_Replace(p_sll list, size_t index, sll_data_t data)
{
    psll_node nd = FindNode(list, index);

    if (!nd)
        return DS_ERROR;
    assert(nd);

    nd->data = data;

    return DS_OK;
}

/* @fn
 * Compute the index of the node with the same data as given 
 * "data".
 *
 * @param list The list to be checked.
 * @param data The given data to compare.
 * @param index Through which we return the resulting index.
 * @param cmp The function used to compare two data, the
 *        return value of this function should meet:
 *        0                - if two data are equal
 *        positive integer - if the first data is greater than 
 *                           the second
 *        negtive integer  - if the first data is smaller than
 *                           the second
 */
ds_stat
SingleLinkedList_Index(p_sll list, sll_data_t data, 
                       size_t* index, sll_comp_func cmp)
{
    psll_node iter;
    size_t    cnt;

    if (SingleLinkedList_Empty(list) == DS_TRUE)
        return DS_ERROR;
    if (!cmp)
        return DS_ERROR;

    assert(cmp);
    for (cnt = 0, iter = list->head; iter; cnt++, iter = iter->next)
        if (cmp(data, iter->data) == 0) {
            if (index)
                *index = cnt;
            return DS_OK;
        }
    return DS_ERROR;
}

/* @fn 
 * Check whether the given "data" is in a list.
 */
ds_bool
SingleLinkedList_Contain(p_sll list, sll_data_t data,
                         sll_comp_func cmp)
{
    if (SingleLinkedList_Index(list, data, NULL, cmp) == DS_OK)
        return DS_TRUE;
    return DS_FALSE;
}

/* @fn
 * Find the maximum data from list.
 *
 * @param list 
 * @param cmp Function used compare two data
 * @param index Through which return the index of the node
 *        with maximum data
 */
ds_stat
SingleLinkedList_Max(p_sll list, sll_comp_func cmp, size_t* index)
{
    psll_node iter, max;
    size_t    max_index, cnt;

    if (SingleLinkedList_Empty(list) == DS_TRUE ||
        !cmp)
        return DS_ERROR;
    
    iter      = list->head->next;
    max       = list->head;
    max_index = 0;
    for (cnt = 1; iter; iter = iter->next, cnt++)
        if (cmp(iter->data, max->data) > 0) {
            max       = iter;
            max_index = cnt;
        }
    *index = max_index;
    return DS_OK;
}

/* @fn
 * Get the maximum data from list, and the maximum data is
 * returned through parameter "re".
 */
ds_stat
SingleLinkedList_MaxData(p_sll list, sll_comp_func cmp, 
                         sll_data_t* re)
{
    psll_node iter, max;

    if (SingleLinkedList_Empty(list) == DS_TRUE ||
        !cmp)
        return DS_ERROR;
    
    iter      = list->head->next;
    max       = list->head;
    for (; iter; iter = iter->next)
        if (cmp(iter->data, max->data) > 0) {
            max       = iter;
        }
    *re = max->data;
    return DS_OK;
}

/* @fn
 * Find the minimum data from list.
 */
ds_stat
SingleLinkedList_Min(p_sll list, sll_comp_func cmp, size_t* index)
{
    psll_node iter, min;
    size_t    min_index, cnt;

    if (SingleLinkedList_Empty(list) == DS_TRUE ||
        !cmp)
        return DS_ERROR;
    
    iter      = list->head->next;
    min       = list->head;
    min_index = 0;
    for (cnt = 1; iter; iter = iter->next, cnt++)
        if (cmp(iter->data, min->data) < 0) {
            min       = iter;
            min_index = cnt;
        }
    *index = min_index;
    return DS_OK;
}

/* @fn
 * Get the minimum data from list, and the minimum data is
 * returned through parameter "re".
 */
ds_stat
SingleLinkedList_MinData(p_sll list, sll_comp_func cmp, 
                         sll_data_t* re)
{
    psll_node iter, min;

    if (SingleLinkedList_Empty(list) == DS_TRUE ||
        !cmp)
        return DS_ERROR;
    
    iter      = list->head->next;
    min       = list->head;
    for (; iter; iter = iter->next)
        if (cmp(iter->data, min->data) < 0) {
            min       = iter;
        }
    *re = min->data;
    return DS_OK;
}

/* @fn
 * Sort a list in ascending order through bubble sort method.
 */
ds_stat
SingleLinkedList_BubbleSort_A(p_sll list, sll_comp_func cmp) 
{
    psll_node iter, end = NULL;

    if (SingleLinkedList_Empty(list) == DS_TRUE || !cmp)
        return DS_ERROR;

    while (end != list->head) {
        for (iter = list->head; iter->next != end; iter = iter->next) 
            if (cmp(iter->data, iter->next->data) > 0) 
                swap(&(iter->data), &(iter->next->data));
        end = iter;
    }

    return DS_OK;
}

/* @fn
 * Sort a list in descending order through bubble sort method.
 */
ds_stat
SingleLinkedList_BubbleSort_D(p_sll list, sll_comp_func cmp) 
{
    psll_node iter, end = NULL;

    if (SingleLinkedList_Empty(list) == DS_TRUE || !cmp)
        return DS_ERROR;

    while (end != list->head) {
        for (iter = list->head; iter->next != end; iter = iter->next) 
            if (cmp(iter->data, iter->next->data) < 0) 
                swap(&(iter->data), &(iter->next->data));
        end = iter;
    }

    return DS_OK;
}

static void
swap(sll_data_t* d1, sll_data_t* d2)
{
    if (!d1 || !d2)
        return;

    sll_data_t   tmp = *d1;

    *d1 = *d2;
    *d2 = tmp;
}
