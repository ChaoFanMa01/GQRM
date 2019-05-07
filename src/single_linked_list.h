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

/* @file single_linked_list.h
 *
 * Single linked list.
 *
 * In this file, the prefix sll stands for 
 * single linked list.
 */


#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H

struct SingleLinkedList;

typedef struct SingleLinkedList*    p_sll;
typedef void*                       sll_data_t;
typedef int (*sll_comp_func)(sll_data_t, sll_data_t);
typedef void (*mapfunc)(sll_data_t*, void*);
typedef void (*sll_clear_op)(sll_data_t*);

extern ds_stat SingleLinkedList_Init(p_sll*);
extern ds_stat SingleLinkedList_Insert(p_sll, size_t, sll_data_t);
extern ds_stat SingleLinkedList_InsertHead(p_sll, sll_data_t);
extern ds_stat SingleLinkedList_InsertTail(p_sll, sll_data_t);
extern ds_stat SingleLinkedList_Delete(p_sll, size_t, sll_data_t*);
extern ds_stat SingleLinkedList_DeleteHead(p_sll, sll_data_t*);
extern ds_stat SingleLinkedList_DeleteTail(p_sll, sll_data_t*);
extern size_t SingleLinkedList_Size(p_sll);
extern ds_bool SingleLinkedList_Empty(p_sll);
extern void SingleLinkedList_Map(p_sll, mapfunc, void*);
extern ds_stat SingleLinkedList_Concat(p_sll, p_sll);
extern p_sll SingleLinkedList_Copy(p_sll);
extern ds_stat SingleLinkedList_GetData(p_sll, size_t, sll_data_t*);
extern ds_stat SingleLinkedList_GetHeadData(p_sll, sll_data_t*);
extern ds_stat SingleLinkedList_GetTailData(p_sll, sll_data_t*);
extern ds_stat SingleLinkedList_Replace(p_sll, size_t, sll_data_t);
extern ds_stat SingleLinkedList_Index(p_sll, sll_data_t, size_t*, sll_comp_func);
extern ds_bool SingleLinkedList_Contain(p_sll, sll_data_t, sll_comp_func);
extern void SingleLinkedList_Clear(p_sll, sll_clear_op);
extern void SingleLinkedList_Destroy(p_sll*, sll_clear_op);
extern ds_stat SingleLinkedList_Max(p_sll, sll_comp_func, size_t*);
extern ds_stat SingleLinkedList_Min(p_sll, sll_comp_func, size_t*);
extern ds_stat SingleLinkedList_BubbleSort_A(p_sll, sll_comp_func);
extern ds_stat SingleLinkedList_BubbleSort_D(p_sll, sll_comp_func);
extern ds_stat SingleLinkedList_MaxData(p_sll, sll_comp_func, sll_data_t*);
extern ds_stat SingleLinkedList_MinData(p_sll, sll_comp_func, sll_data_t*);
#endif
