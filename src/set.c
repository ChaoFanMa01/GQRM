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

#include "set.h"

struct _set {
    pt_AVLTree     avl;
	set_cmp        cmp;
	gqrm_id_t      id;
};

static void add(pt_AVLTree, avlt_data_t);
static void minus(pt_AVLTree, avlt_data_t);
static pt_Set copy(pt_Set);

pt_Set
Set_Create(set_cmp cmp, gqrm_id_t id)
{
    pt_Set   ps = malloc(sizeof(Set));

	if (!ps)
	    return NULL;
	
	if ((ps->avl = AVLTree_Create(cmp)) == NULL) {
	    free(ps);
		return NULL;
	}
	ps->id  = id;
	ps->cmp = cmp;
	return ps;
}

ds_stat
Set_Init(pt_Set st, set_data_t array[], size_t n)
{
    size_t   i;

	if (!array)
	    return DS_ERROR;
	
	for (i = 0; i < n ; i++)
	    if (AVLTree_Insert(st->avl, array[i]) == DS_ERROR)
		    return DS_ERROR;
	return DS_OK;
}

ds_stat
Set_Insert(pt_Set st, set_data_t data)
{
    if (!st || !st->avl)
	    return DS_ERROR;
	return AVLTree_Insert(st->avl, data);
}

ds_stat
Set_Delete(pt_Set st, set_data_t data)
{
    if (!st || !st->avl)
	    return DS_ERROR;
	return AVLTree_Delete(st->avl, data);
}

size_t 
Set_Size(pt_Set st)
{
    if (!st || !st->avl)
	    return 0;
	return AVLTree_Size(st->avl);
}

pt_Set
Set_Minus(pt_Set to, pt_Set from)
{
    pt_Set res;

    if (!to || !from)
	    return to;
	
	if ((res = copy(to)) == NULL)
	    return NULL;
	
	AVLTree_InterOpt(res->avl, from->avl, minus);

	return res;
}

pt_Set
Set_Union(pt_Set st1, pt_Set st2)
{
    pt_Set res;

	if (!st1 || !st2)
	    return NULL;
	
	if ((res = copy(st1)) == NULL)
	    return NULL;

    printf("copy over\n");
	AVLTree_InterOpt(res->avl, st2->avl, add);
	printf("insert over\n");

	return res;
}

ds_stat
Set_InSetMinus(pt_Set to, pt_Set from)
{
    if (!to || !from)
	    return DS_ERROR;
	AVLTree_InterOpt(to->avl, from->avl, minus);
	return DS_OK;
}

ds_stat
Set_InSetUnion(pt_Set lhs, pt_Set rhs)
{
    if (!lhs || !rhs)
	    return DS_ERROR;
	AVLTree_InterOpt(lhs->avl, rhs->avl, add);
	return DS_OK;
}

ds_stat
Set_Map(pt_Set st, set_map func)
{
    if (!st)
	    return DS_ERROR;
	return AVLTree_InOrderMap(st->avl, func);
}

static pt_Set
copy(pt_Set st)
{
    pt_Set cpy;

	if (!st)
	    return NULL;
	
	if ((cpy = Set_Create(st->cmp, st->id)) == NULL)
	    return NULL;
	if ((cpy->avl = AVLTree_Copy(st->avl)) == NULL) {
	    free(cpy);
		return NULL;
	}
	return cpy;
}

static void
minus(pt_AVLTree avl, avlt_data_t data)
{
    AVLTree_Delete(avl, data);
}

static void
add(pt_AVLTree avl, avlt_data_t data)
{
    AVLTree_Insert(avl, data);
}
